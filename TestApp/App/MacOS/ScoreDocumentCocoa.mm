// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#import "ScoreDocumentCocoa.h"

// Frameworks
#import <ZipArchive.h>

// Notation
#import "Clef.h"
#import "Clipboard.h"
#import "FileMediatorFactory.h"
#import "InstrumentConfig.h"
#import "KeySignature.h"
#import "Measure.h"
#import "Note.h"
#import "Preferences.h"
#import "Score.h"
#import "ScoreCreationInfo.h"
#import "StaffSystem.h"
#import "Selection.h"
#import "UndoManager.h"

// Local
#import "Directories.h"
#import "ScoreManager.h"
#import "ScoreViewCocoa.h"

@interface ScoreDocumentCocoa()
{
    komp::ScoreUPtr _score;
}

@property (nonatomic, strong) NSString* data;
@property (nonatomic, strong) NSWindowController* modalWindowController;

@property (nonatomic, assign) BOOL isEdited;
@property (nonatomic, assign) BOOL isZipped;
@property (nonatomic, assign) NSURL* documentURL;

@end


@implementation ScoreDocumentCocoa

- (instancetype)init {
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)
dealloc {
    komp::ScoreManager::getInstance().remove(_score.get());
}

static komp::ScoreInfo defaultScoreInfo()
{
    komp::ScoreInfo info;
    info.metadata.title = "Untitled";
    info.measuresPerLine = 3;
    info.linesPerPage = 5;
    info.pages = 1;
    info.systemInset = komp::ScoreInfo::SYSTEM_INSET;

    komp::Instrument instrument;
    instrument.name = "Piano";
    komp::TrackInfo track(instrument);
    track.setName("Piano");
    track.setSoundID(komp::SoundID::KEYBOARD_PIANO);
    track.addStaffInfo(komp::ClefType::G);
    track.addStaffInfo(komp::ClefType::F);
    info.trackInfoList.emplace_back(std::move(track));
    
    komp::SystemBracketInfo bracket(komp::SystemBracketType::BRACE, 0, 0, 0);
    info.systemBracketInfoList.emplace_back(std::move(bracket));
    
    return info;
}

- (void)
windowControllerDidLoadNib:(NSWindowController *)aController {
    
    [super windowControllerDidLoadNib:aController];
        
    if (!_score) {
        _score = komp::Score::create(defaultScoreInfo());
        _score->layout();
    }
    
    [self setScore: std::move(_score)];
}

- (NSString *)
windowNibName {
    return @"ScoreDocumentCocoa";
}

- (void)
windowDidBecomeMain:(NSNotification *)notification {
}

// MARK: - File status

+ (BOOL)
autosavesInPlace {
    return NO;
}

- (BOOL)
isDocumentEdited {
    return self.isEdited;
}

- (IBAction)
saveDocument:(nullable id)sender {
    [self saveDocumentWithDelegate:self
                   didSaveSelector:@selector(didSave:didSave:contextInfo:)
                       contextInfo:NULL];
}

- (void)
didSave:(ScoreDocumentCocoa *)doc didSave:(BOOL)didSave contextInfo:(void  *)contextInfo {
    
    // We need to check the result of the operation.
    // IF we did not succeed so something...

}

// MARK: - Menu Actions

- (BOOL)
validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)inItem {
    
    NSMenuItem* menuItem = (NSMenuItem*) inItem;

    const bool hasSelection = !_score->getSelection()->isEmpty();
     
    const SEL action = inItem.action;

    //
    // File Menu
    //

    if (action == @selector(printDocument:)
     || action == @selector(saveDocument:)) {
        return YES;
    }
    
    //
    // Edit Menu
    //

    if (action == @selector(kompUndo:)) {
        BOOL enable = NO;
        menuItem.title = NSLocalizedString(@"Undo", @"Undo");
        if (_score.get()->getUndoManager().canUndo()) {
            const std::string title = "Undo " + _score.get()->getUndoManager().undoTitle();
            menuItem.title = [NSString stringWithUTF8String:title.c_str()];
            enable = YES;
        }
        return enable;
    }

    if (action == @selector(kompRedo:)) {
        BOOL enable = NO;
        menuItem.title = NSLocalizedString(@"Redo", @"Redo");
        if (_score.get()->getUndoManager().canRedo()) {
            const std::string title = "Redo " + _score.get()->getUndoManager().redoTitle();
            menuItem.title = [NSString stringWithUTF8String:title.c_str()];
            enable = YES;
        }
        return enable;
    }

    if (action == @selector(cut:)
    ||  action == @selector(copy:)
    ||  action == @selector(deleteSelection:)) {
        return hasSelection;
    }

    if (action == @selector(paste:)) {
        return NO;
    }
    
    if (action == @selector(selectAll:)) {
        return YES;
    }
    
    // Options menu handled below in validateMenuItem in order to set checked state
    return [super validateUserInterfaceItem:inItem];
}


- (BOOL)
validateMenuItem:(NSMenuItem *)inItem {
    
    if (inItem.action == @selector(performClose:)) {
        return YES;
    }
    
    return [super validateMenuItem:inItem];
}

// MARK: - Edit Menu

- (IBAction)
kompUndo:(id)sender {
    _score.get()->getUndoManager().undo();
    [_scoreView setNeedsDisplay];
}

- (IBAction)
kompRedo:(id)sender {
    _score.get()->getUndoManager().redo();
    [_scoreView setNeedsDisplay];
}

- (IBAction)
cut:(id)sender {
}

- (IBAction)
copy:(id)sender {
}

- (IBAction)
paste:(id)sender {
}

- (IBAction)
deleteSelection:(id)sender {
    _score->getSelection()->deleteItems();
    _score->clearSelection();
    [self.scoreView setNeedsDisplay];
}

- (IBAction)
selectAll:(id)sender {
    
    const auto& selection = _score->getSelection();

    for (const auto& measure : _score->getMeasures()) {
        for (komp::Item& item : measure->allItems()) {
            if ([self itemIsSelectable:item]) {
                selection->addItem(item);
                //komp::ItemImpl::setNeedsDisplay(item);
            }
        }
    }
}

// MARK: - Debug Menu

- (IBAction)
toggleOutlines:(id)sender {
    
    auto& prefs = komp::Preferences::instance();
    prefs.showOutlines(!prefs.getShowGlyphOutlines());
    _score->draw();
    [_scoreView setNeedsDisplay];
    
    NSMenuItem* menu = sender;
    [menu setState: prefs.getShowGlyphOutlines() ? NSControlStateValueOn : NSControlStateValueOff];
}

- (IBAction)
toggleItemTimes:(id)sender {
    auto& prefs = komp::Preferences::instance();
    prefs.toggleItemTimes();
    _score->draw();
    [_scoreView setNeedsDisplay];
    
    NSMenuItem* menu = sender;
    [menu setState: prefs.getShowItemTimes() ? NSControlStateValueOn : NSControlStateValueOff];
}

- (IBAction)
redraw:(id)sender {
    _score->draw();
    [_scoreView setNeedsDisplay];
}

- (IBAction)
justify:(id)sender {    
    _score->layout();
    _score->draw();
    [_scoreView setNeedsDisplay];
}

- (IBAction)
addDynamic:(id)sender {
    
    const auto& selection = _score->getSelection();
    if (selection->isEmpty()) {
        return;
    }
    
    auto notes = selection->getItems<komp::Note>();
    if (notes.empty()) {
        return;
    }
    komp::sortItems(notes);
        
    // Get the first note of each track in the sorted selection
    komp::NoteRefVec dynamicNotes;
    std::map<komp::Track*, komp::Note*> trackMap;
    for (komp::Note& note : notes) {
        const auto& location = note.getLocation();
        auto& track = location.getTrack();
        if (!trackMap.contains(&track)) {
            trackMap[&track] = &note;
            dynamicNotes.emplace_back(note);
        }
    }
        
    auto& factory = _score->getCommandFactory();
    auto parentCommand = factory.genericCommand("Add Mark");
    for (komp::Note& item : dynamicNotes) {
        const auto& note = item.as<komp::Note>();
        auto addMarkCommand = factory.insertMarkCommand(note.getItemID(),
                                                        komp::MarkType::DYNAMIC_F,
                                                        komp::PlacementType::AUTOMATIC);
        addMarkCommand->execute();
        parentCommand->addSubCommand(std::move(addMarkCommand));
    }
    _score->getUndoManager().addCommand(std::move(parentCommand));
}

// MARK: - File Handling

- (NSData *)
dataOfType:(NSString *)typeName error:(NSError **)outError {
    
    if (!_score) {
        [NSException raise:@"NullScore" format:@"%@ was called on a null score", NSStringFromSelector(_cmd)];
    }
    auto fileMediator = komp::FileMediatorFactory::create(komp::FileMediatorType::MusicXML);
    std::ostringstream oss;
    komp::FileSaveParams fparams{*_score.get()};
    fileMediator->saveStream(fparams, oss);
    std::string dataString = oss.str();
    NSData* data = [NSData dataWithBytes:(const void *)dataString.c_str() length:sizeof(char)*(dataString.size())];
    return data;
}

- (BOOL)
isValidFileType:(NSString *)typeName {
    
    if ([typeName isEqualToString:@"Uncompressed MusicXML File"]) {
        return YES;
    }
    
    if ([typeName isEqualToString:@"Compressed MusicXML File"]) {
        return YES;
    }
    
    return NO;
}


- (BOOL)
readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError {
    
    if (![self isValidFileType:typeName]) {
        if (outError) {
            *outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
        }
        return NO;
    }
    
    self.isZipped = [self dataIsZipped:data];
    if (self.isZipped) {
        
        NSError* error;

        // Get the path to the temp file
        NSString* zippedPath = [NSTemporaryDirectory() stringByAppendingPathComponent:[[NSUUID UUID] UUIDString]];
        [data writeToFile:zippedPath options:NSDataWritingAtomic error:&error];
        if (error) {
            NSLog(@"Error writing temp file: %@", error.localizedDescription);
            return NO;
        }

        // Get the path to the destination file
        NSString* unzippedPath = [NSTemporaryDirectory() stringByAppendingPathComponent:[[NSUUID UUID] UUIDString]];

        if (![SSZipArchive unzipFileAtPath:zippedPath toDestination:unzippedPath]) {
            NSLog(@"Error unzipping temp file: %@", error.localizedDescription);
            return NO;
        }
        
        // Get the xml container
        NSString* containerPath = [unzippedPath stringByAppendingPathComponent:@"META-INF/container.xml"];
        NSFileManager* fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:containerPath]) {
            NSLog(@"Container file does not exist");
            return NO;
        }
        
        // Ignore the container file and grab the first xml file found
        NSArray* dirFiles = [fileManager contentsOfDirectoryAtPath:unzippedPath error:&error];
        if (error) {
            NSLog(@"Error finding xml files: %@", error.localizedDescription);
            return NO;
        }
        
        if (dirFiles.count < 1) {
            NSLog(@"No files found");
            return NO;
        }
        
        NSArray* xmlFiles = [dirFiles filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:@"self ENDSWITH '.xml'"]];
        if (xmlFiles.count < 1) {
            NSLog(@"No XML files found");
            return NO;
        }
        
        // Load the musicxml file
        NSString* xmlFile = xmlFiles[0];
        NSString* xmlFilePath = [unzippedPath stringByAppendingPathComponent:xmlFile];
        data = [[NSFileManager defaultManager] contentsAtPath:xmlFilePath];
        self.documentURL = [NSURL fileURLWithPath:xmlFilePath];
        
    } else {
        self.documentURL = [self fileURL];
    }
    
    // Load the MusicXML data
    NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:NSPlainTextDocumentType,
                          NSDocumentTypeDocumentAttribute,
                          nil];
    NSDictionary* attributes;
    NSError* error = nil;
    NSAttributedString* attributedString = [[NSAttributedString alloc] initWithData:data
                                                                            options:dict
                                                                 documentAttributes:&attributes
                                                                              error:&error];
    if (error) {
        if (outError) {
            *outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];
        }
        
        NSLog(@"Error readFromData: %@", error.localizedDescription);
        return NO;
    }
    
    self.data = attributedString.string;
    
    if (self.documentURL) {
        
        try {
            auto fileMediator = komp::FileMediatorFactory::create(komp::FileMediatorType::MusicXML);
            NSURL* url = self.documentURL;
            NSString* filePathNsString = [url path];
            std::string filePath = [filePathNsString UTF8String];

            komp::InstrumentConfig cfg{};
            const auto dir = komp::getConfigDirectory();
            const auto configFilePath = dir + std::string{"/instruments.json"};
            cfg.loadFromFile(configFilePath.c_str());

            _score = fileMediator->loadFile(filePath, cfg);

            return YES;
            
        } catch (std::exception& cppEx) {
            
            std::stringstream errorMessage;
            errorMessage << "ScoreDocumentCocoa.mm readFromData - exception occured during fileMediator->loadFile - " << cppEx.what();
            NSString *nsError = [NSString stringWithUTF8String:errorMessage.str().c_str()];
            *outError = [NSError errorWithDomain:nsError code:unimpErr userInfo:NULL];
            return NO;
        }
    }
    
    _score = komp::Score::create(komp::defaultScoreInfo());

    return YES;
}

- (BOOL)
canBecomeMainWindow {
    return YES;
}

- (BOOL)
canBecomeKeyWindow {
    return YES;
}

- (void)performClose:(id)sender {
    [self canCloseDocumentWithDelegate:self shouldCloseSelector:@selector(document:shouldClose:contextInfo:) contextInfo:NULL];
}

- (void)document:(NSDocument*)doc shouldClose:(BOOL)shouldClose contextInfo:(void*)contextInfo {
    if (shouldClose)
        [doc close];
}

// MARK: - Printing

- (NSPrintOperation *)
printOperationWithSettings:(NSDictionary<NSString *,id> *)printSettings error:(NSError * _Nullable *)outError {
    
    NSPrintInfo* printInfo = [self printInfo];
    [printInfo setTopMargin:0.0];
    [printInfo setBottomMargin:0.0];
    [printInfo setLeftMargin:0.0];
    [printInfo setRightMargin:0.0];
    [printInfo setHorizontalPagination:NSPrintingPaginationModeAutomatic];
    [printInfo setVerticalPagination:NSPrintingPaginationModeAutomatic];
    
    NSPrintOperation* printOperation = [NSPrintOperation printOperationWithView:self.scoreView
                                                                      printInfo:printInfo];
    return printOperation;
}

- (void)
saveToPDF:(NSString*)inPath {
    
    // Set the print options to save the score out as a PDF file
    NSURL* url = [NSURL fileURLWithPath:inPath];
    NSDictionary* printOptions = @{
                                   NSPrintJobDisposition:NSPrintSaveJob,
                                   NSPrintJobSavingURL:url,
                                   NSPrintFirstPage:@(1),
                                   NSPrintLastPage:@(1)
                                   };

    // Configure the print options
    NSPrintInfo* printInfo = [[NSPrintInfo alloc] initWithDictionary:printOptions];
    printInfo.topMargin = 0.0;
    printInfo.bottomMargin = 0.0;
    printInfo.leftMargin = 0.0;
    printInfo.rightMargin = 0.0;
    printInfo.horizontalPagination = NSPrintingPaginationModeAutomatic;
    printInfo.verticalPagination = NSPrintingPaginationModeAutomatic;
    
    // Print the file without prompting the user
    NSPrintOperation* printOperation = [NSPrintOperation
                                        printOperationWithView:self.scoreView
                                        printInfo:printInfo];
    
    printOperation.showsPrintPanel = NO;
    printOperation.showsProgressPanel = NO;
    [printOperation runOperation];
}

// MARK: - Selection

- (BOOL)
itemIsSelectable:(komp::Item&)inItem {
    
    const auto& itemTraits = inItem.getItemTraits();
    const auto itemType = itemTraits.getItemType();
    
    switch (itemType) {
        
    case komp::ItemType::Clef:
    case komp::ItemType::KeySignature:
        return NO;
    default:
        return YES;
    }
}


// MARK: - Accessors
    
- (void)
setScore:(komp::ScoreUPtr)inScore {
    
    _score = std::move(inScore);
        
    komp::ScoreManager::getInstance().add(_score.get());

    [self.scoreView setScore:_score.get()];
    
    _score->loaded();
    
    _score->layout();
}

- (komp::Score*)
getScore {
    return _score.get();
}

- (BOOL)
dataIsZipped:(NSData*)inData {
    
    if ([inData length] > 4) {
        const char* bytes = static_cast<const char*>([inData bytes]);
        if (bytes[0] == 'P' && bytes[1] == 'K' && bytes[2] == 3 && bytes[3] == 4) {
            return YES;
        }
    }
    return NO;
}

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
