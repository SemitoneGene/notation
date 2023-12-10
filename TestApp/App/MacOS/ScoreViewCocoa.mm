// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#import "ScoreViewCocoa.h"

// Notation
#import "CurrentState.h"
#import "Engraver.h"
#import "Score.h"
#import "ScoreColors.h"
#import "Selection.h"

// MARK: - Constants

static const CGFloat MIN_ZOOM = .5;
static const CGFloat MAX_ZOOM = 12.0;
static const CGFloat ZOOM_INCREMENT = .25;

// Zooming
static NSString* const ZOOM_NORMAL = @"z";
static NSString* const ZOOM_IN_EQUAL = @"=";
static NSString* const ZOOM_OUT_MINUS = @"-";

enum class DragType
{
    NONE,
    ITEM,
    SELECT
};

@interface ScoreViewCocoa()
{
    komp::Score* _score;
    komp::Item* _foundItem;
    std::set<komp::Measure*> _dragMeasures;
    komp::ItemUPtr _dragMemento;
}

@property (nonatomic, weak) IBOutlet NSScrollView* scrollView;

@property (nonatomic, assign) BOOL layerAdded;
@property (nonatomic, assign) BOOL changed;
@property (nonatomic, assign) BOOL dragging;
@property (nonatomic, assign) CGPoint startDrag;
@property (nonatomic, assign) CGPoint lastDrag;
@property (nonatomic, assign) DragType dragType;
@property (nonatomic, strong) NSView* selectionView;
@property (nonatomic, assign) CGFloat zoomScale;
@property (nonatomic, assign) int activeHand;
@property (nonatomic, assign) int lastDragSlot;

@end

// MARK: - ScoreViewCocoa

@implementation ScoreViewCocoa

- (instancetype)
initWithCoder:(NSCoder *)coder {
    
    self = [super initWithCoder:coder];
    if (self) {

        self.wantsLayer = YES;
        
        // Initialize properties
        self.dragType = DragType::NONE;
        self.zoomScale = 2.0;
        self.activeHand = 0;
        self.lastDragSlot = -500;
    }
    return self;
}

- (void)
dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (void)
setNeedsDisplay {
    self.needsDisplay = YES;
}

- (BOOL)
isFlipped {
    return YES;
}

- (BOOL)
acceptsFirstResponder {
    return YES;
}

- (void)
viewDidEndLiveResize {
    [super viewDidEndLiveResize];
    [self updateForZoom];
}


// MARK: - Mouse Input

- (void)
mouseDown:(NSEvent *)inEvent {
    
    [super mouseDown:inEvent];
    
    switch ([inEvent clickCount])
    {
        case 1:
            [self handleSingleClick:inEvent];
            break;
            
        case 2:
            [self handleDoubleClick:inEvent];
            break;
            
        default:
            break;
    }
}

- (void)
mouseMoved:(NSEvent*)inEvent {
    
    [super mouseMoved:inEvent];
    
    if (self.dragType != DragType::NONE) {
        return;
    }
    
    // Get the current mouse position
    komp::Point currentDrag = [self convertPoint:[inEvent locationInWindow] fromView:nil];
    
    // Flip the origin to take into scores top left origin.
    NSRect updateRect = [self bounds];
    currentDrag.y = (updateRect.size.height - currentDrag.y);
    
    // Look for an item under the cursor
    const auto state = _score->getCurrentState(currentDrag);
    const auto& staff = state.getStaff();
    const auto& foundItem = staff.locateItem<komp::Item>(state.getMeasure(),
                                                         state.getHand(),
                                                         currentDrag);
    if (foundItem) {
        [[NSCursor pointingHandCursor] set];
    } else {
        [[NSCursor arrowCursor] set];
    }
}


- (void)
mouseDragged:(NSEvent*)inEvent {
    
    if (self.dragging) {
        [self handlePressChanged:inEvent];
    } else {
        [self handlePressBegan:inEvent];
    }
}

- (void)
mouseUp:(NSEvent *)inEvent {
    
    if (self.dragging) {
        [self handlePressEnded:inEvent];
    }
}

// MARK: - Clicks

- (void)
handleSingleClick:(NSEvent*)inEvent {
    
    _foundItem = [self handleClickShared:inEvent];
    if (_foundItem) {
        _score->getSelection()->addItem(*_foundItem);
    }

    // Reset the current drag type
    self.dragType = DragType::NONE;

    // Update the drag changes
    [self setNeedsDisplay];
}

static const int STAFF_SELECT_PAD = 3;

- (void)
handleDoubleClick:(NSEvent *)inEvent {

    _score->clearSelection();

    _foundItem = [self handleClickShared:inEvent];
    if (_foundItem) {
        // Other routines will handle double click of found item
    } else {
        komp::Point scorePt = [self convertPoint:[inEvent locationInWindow]
                                        fromView:nil];
        scorePt.x /= self.zoomScale;
        scorePt.y /= self.zoomScale;

        const auto state = _score->findPosition(scorePt);
        const auto& system = state.getMeasure().getStaffSystem();
        const auto& staff = state.getStaff();
        
        const auto systemY = system->getY();
        const komp::Rect staffRect(systemY + staff.getY(),
                                   system->getX(),
                                   systemY + staff.bottom(),
                                   system->getRight());

        const auto& selection = _score->getSelection();

        // Look outside the staff
        // Select the current staf
        if (scorePt.x < staffRect.left) {
            if (scorePt.y >= staffRect.top - STAFF_SELECT_PAD
             && scorePt.y <= staffRect.bottom + STAFF_SELECT_PAD) {
                
                const auto firstMeasure = system->getFirstMeasure();
                const auto measureCount = system->getMeasureCount();
                const auto scoreMeasureCount = _score->getMeasureCount();
                
                for (int i = 0; i < measureCount; ++i) {
                    const auto measureIndex = firstMeasure + i;
                    if (measureIndex < scoreMeasureCount) {
                        const auto& staffMeasure = _score->getMeasure(static_cast<int>(measureIndex));
                        const auto& items = staffMeasure->allItems();
                        for (const auto& item : items) {
                            selection->addItem(item);
                        }
                    }
                }
            }
        } else {
            const auto& measure = state.getMeasure();
            const auto& items = measure.allItems();
            for (const auto& item : items) {
                selection->addItem(item);
            }
        }
    }

    self.dragType = DragType::NONE;

    [self setNeedsDisplay];
}


- (komp::Item*)
handleClickShared:(NSEvent *)inEvent {
    
    _score->clearSelection();
    
    // Reset the drag state
    self.dragging = false;
    self.changed = NO;
    _foundItem = nullptr;
    
    const auto localPoint = [self convertPoint:[inEvent locationInWindow] fromView:nil];
    
    komp::Point scorePt = localPoint;
    scorePt.x /= self.zoomScale;
    scorePt.y /= self.zoomScale;
    
    return _score->handleTap(scorePt);
}

- (void)
handlePressBegan:(NSEvent *)inEvent {
    
    self.dragging = true;
        
    self.startDrag = [self convertPoint:[inEvent locationInWindow] fromView:nil];
    const komp::Point scoreLocation(self.startDrag);
    
    _foundItem = [self handlePressShared:inEvent];
    if (_foundItem) {
        
        [NSCursor hide];
        
        self.dragType = DragType::ITEM;
        
        _dragMemento = _foundItem->getMemento();
        
        _score->getSelection()->addItem(*_foundItem);
        [self setNeedsDisplay];
        
        komp::Point scorePt = scoreLocation;
        scorePt.x /= self.zoomScale;
        scorePt.y /= self.zoomScale;
        
        const auto location = _foundItem->getLocation();
        _dragMeasures.insert(&location.getMeasure());
        
        const auto state = _score->getCurrentState(scorePt);
        _foundItem->prepareToDrag(state);
    } else {
        self.dragType = DragType::SELECT;
    }
}

-(void)
handlePressChanged:(NSEvent*)inEvent {
    
    self.changed = YES;
    
    const komp::Point currentLocation = [self convertPoint:[inEvent locationInWindow]
                                                  fromView:nil];
    
    switch (self.dragType) {

        case DragType::ITEM:
        {
            komp::Point scoreLocation = currentLocation;
            scoreLocation.x /= self.zoomScale;
            scoreLocation.y /= self.zoomScale;

            const auto state = _score->getCurrentState(scoreLocation);
            _foundItem->updateDrag(state);
            
            if (self.lastDragSlot != _foundItem->getSlot()) {
                self.lastDragSlot = _foundItem->getSlot();
            }
            
            _score->layout();
            break;
        }
                        
        case DragType::SELECT:
        {
            const komp::Rect frame(self.startDrag, currentLocation);
            
            if (!self.selectionView) {
                self.selectionView = [[NSView alloc] initWithFrame:frame];
                self.selectionView.wantsLayer = YES;
                self.selectionView.layer.backgroundColor = [NSColor colorWithRed:0.0
                                                                           green:0.643
                                                                            blue:1.0
                                                                           alpha:0.3].CGColor;
                [self addSubview:self.selectionView];
            }
            self.selectionView.frame = frame;
            self.selectionView.hidden = NO;
            break;
        }
            
        default:
            break;
    }
    
    self.lastDrag = currentLocation;
    
    [self setNeedsDisplay];
}

-(void)
handlePressEnded:(NSEvent *)inEvent
{
    self.dragging = false;
    
    komp::Point scoreDragStart = self.startDrag;
    scoreDragStart.x /= self.zoomScale;
    scoreDragStart.y /= self.zoomScale;

    komp::Point scoreDragEnd = [self convertPoint:[inEvent locationInWindow] fromView:nil];
    scoreDragEnd.x /= self.zoomScale;
    scoreDragEnd.y /= self.zoomScale;
    
    switch (self.dragType) {
            
        case DragType::ITEM:
        {
            const auto state = _score->getCurrentState(scoreDragEnd);
            _foundItem->completeDrag(state);
                        
            auto& commandFactory = _score->getCommandFactory();
            auto stateCommand = commandFactory.itemStateCommand(std::move(_dragMemento),
                                                                _foundItem->getMemento());
            _score->getUndoManager().addCommand(std::move(stateCommand));
            _score->layout();

            // Show the cursor and the score insertion point
            [NSCursor unhide];
            
            break;
        }
            
        case DragType::SELECT:
        {
            const auto state = _score->getCurrentState(scoreDragEnd);
            const auto& staff = state.getStaff();

            self.selectionView.hidden = YES;
            [self handleSelection:staff
                            frame:komp::Rect(scoreDragStart, scoreDragEnd)];
            break;
        }
            
        default:
            break;
    }
    
    self.changed = NO;
    self.dragType = DragType::NONE;
    _foundItem = nullptr;
        
    _dragMeasures.clear();

    [self setNeedsDisplay];
}

- (komp::Item*)
handlePressShared:(NSEvent *)inEvent {
    
    self.changed = NO;
    _foundItem = nullptr;
    
    komp::Point scoreLocation = [self convertPoint:[inEvent locationInWindow] fromView:nil];
    scoreLocation.x /= self.zoomScale;
    scoreLocation.y /= self.zoomScale;
    
    return _score->handleTap(scoreLocation);
}

// MARK: - Keyboard Input

- (void)
keyDown:(NSEvent *)inEvent {
    
    // Get the input key
    NSString* const input = [inEvent charactersIgnoringModifiers];
        
    // Zoom
    if ([input isEqualToString:ZOOM_NORMAL]
        || [input isEqualToString:ZOOM_IN_EQUAL]
        || [input isEqualToString:ZOOM_OUT_MINUS]) {
        [self handleZoom:inEvent];
        return;
    }

    // Zoom
    if ([input isEqualToString:ZOOM_NORMAL]) {
        [self handleZoom:inEvent];
        return;
    }
}

// MARK: - Printing


// Return the number of pages available for printing
- (BOOL)
knowsPageRange:(NSRangePointer)range {
    
    *range = NSMakeRange(1, 1);
    return YES;
}

// Return the drawing rectangle for a particular page number
- (NSRect)
rectForPage:(NSInteger)inPage {
    
    return NSMakeRect(0,
                      0,
                      0,
                      0);
}

- (double)
calculatePrintWidth {
    
    // Obtain the print info object for the current operation
    NSPrintInfo* printInfo = [[NSPrintOperation currentOperation] printInfo];
    
    // Calculate the page height in points
    const auto paperSize = [printInfo paperSize];
    const auto pageWidth = paperSize.width - printInfo.leftMargin - printInfo.rightMargin;
    
    // Convert height to the scaled view
    const auto scale = [[[printInfo dictionary] objectForKey:NSPrintScalingFactor] floatValue];
    
    return pageWidth / scale;
}

// MARK: - Insertion

- (void)
insertNote {
    
//    // Get the insertion location
//    const auto& measure = _score->focusMeasure();
//    komp::Point insertionPoint;
//    insertionPoint.x = measure->getDataX() + _score->focusX();
//    insertionPoint.y = _score->focusStaff()->slotToPixel(_score->focusSlot(), false);
//    
//    const auto& item = _score->insertNote(insertionPoint, self.temporalType);
//    if (!item) {
//        // TODO: Unable to insert the item. What should we do?
//        LOG_F(WARNING, "Unable to insert note");
//        return;
//    }
}

- (void)
insertRest {
    
//    // Get the insertion location
//    const auto& measure = _score->focusMeasure();
//    komp::Point insertionPoint;
//    insertionPoint.x = measure->getDataX() + _score->focusX();
//    insertionPoint.y = _score->focusStaff()->slotToPixel(_score->focusSlot(), false);
//    
//    const auto& item = _score->insertRest(insertionPoint, self.temporalType);
//    if (!item) {
//        // TODO: Unable to insert the item. What should we do?
//        LOG_F(WARNING, "Unable to insert rest");
//        return;
//    }
}

// MARK: - Zooming

- (void)
handleZoom:(NSEvent*)inEvent {
    
    // Get the input key
    NSString* const input = [inEvent charactersIgnoringModifiers];
    
    if ([input isEqualToString:ZOOM_NORMAL]) {
        [self resetZoom];
        return;
    }
    
    if ([input isEqualToString:ZOOM_IN_EQUAL]) {
        [self zoomIn];
        return;
    }
    
    if ([input isEqualToString:ZOOM_OUT_MINUS]) {
        [self zoomOut];
        return;
    }
}

- (void)
zoomIn {
    // Calculate the new zoom amount, capping the sizes
    self.zoomScale += ZOOM_INCREMENT;
    [self updateForZoom];
    [self setNeedsDisplay];
}

- (void)
zoomOut {
    // Calculate the new zoom amount, capping the sizes
    self.zoomScale -= ZOOM_INCREMENT;
    [self updateForZoom];
    [self setNeedsDisplay];
}

- (void)
resetZoom {
}

- (CGFloat)
zoomScale {
    return _zoomScale;
}

- (void)
updateForZoom {
    
    self.zoomScale = fmax(MIN_ZOOM, self.zoomScale);
    self.zoomScale = fmin(self.zoomScale, MAX_ZOOM);
    
    if (_score) {
        auto scoreLayer = (__bridge CALayer*)_score->getContext().getLayer();
        scoreLayer.transform = CATransform3DMakeScale(self.zoomScale,
                                                      self.zoomScale,
                                                      1.0f);
    }
    [self updateContentSize];
}

- (void)
updateContentSize {
    
    if (!_score) {
        LOG_F(WARNING, "Invalid score.");
        return;
    }
    
    // Update the scroll content size.  If the content size is less
    // than the window size, use the window size to the scroll bars
    // will update properly
    auto contentSize = _score->getLayout()->getContentSize();
    contentSize.width *= self.zoomScale;
    contentSize.height *= self.zoomScale;
    
    [self.scrollView.documentView setFrame:CGRectMake(0,
                                                      0,
                                                      std::max(contentSize.width, self.superview.bounds.size.width),
                                                      std::max(contentSize.height, self.superview.bounds.size.height))];
}

- (CGFloat)
getZoomScale {
    return self.zoomScale;
}

- (void)
setScore:(komp::Score*)inScore {
    _score = inScore;
    [self updateForZoom];
}


// MARK: - Drawing

- (void)
updateLayer
{
    if (!self.layerAdded) {

        self.layer.backgroundColor = [NSColor whiteColor].CGColor;
                      
        auto scoreLayer = (__bridge CALayer*)_score->getContext().getLayer();
        [self.layer addSublayer:scoreLayer];

        _score->draw();
        
        self.layerAdded = YES;
    }
}

- (BOOL)
wantsUpdateLayer {
    return YES;
}

// MARK: - Selection

- (void)
handleSelection:(const komp::Staff&)inStaff
          frame:(const komp::Rect&)inSelectionFrame {
    
    // Get a rect representing a staff slot
    static const auto staffSpacing = inStaff.getSpacing();
    komp::Rect slotFrame = komp::Rect(0, 0, staffSpacing, staffSpacing);
    
    // Expand it larger
    slotFrame = slotFrame.grow(5.0, 5.0);
    
    // Filter out closed paths that are too small.
    const komp::Rect selectionFrame = inSelectionFrame;
    if (selectionFrame.getWidth() <= slotFrame.getWidth()
     || selectionFrame.getHeight() <= slotFrame.getHeight()) {
        return;
    }
    
    for (const auto& measure : _score->getMeasures()) {
        
        const auto& items = measure->allItems();
        for (komp::Item& item : items) {
            const auto itemFrame = item.getFrame();
            if (inSelectionFrame.intersects(itemFrame)
             || inSelectionFrame.contains(itemFrame)) {
                _score->getSelection()->addItem(item);
            }
        }
    }
}

// MARK: - Notification Methods

- (void)
receivedRefreshScoreNotification:(NSNotification*)notification {
    self.needsDisplay = YES;
}

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

