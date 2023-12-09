// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "Library.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "Staff.h"
#include "Text.h"


namespace komp
{    
    Text::Text(Score* inScore,
               Track* inTrack,
               const std::string& inText,
               PlacementType inPlacement)
    : Spatial(inScore, inTrack)
    , mText(inText)
    , mPlacement(inPlacement)
    , mFontSize(14)
    , mFontAlign(0)
    {
        switch (mPlacement)
        {
            case PlacementType::ABOVE_ITEM:
                setSlot(18);
                break;
                
            case PlacementType::BELOW_ITEM:
                setSlot(-2);
                break;
                
            default:
                setSlot(0);
        }
    }
    
    Text::Text(const Text& inItem)
    : Spatial(inItem)
    , mText(inItem.mText)
    , mPlacement(inItem.mPlacement)
    {
    }
    
    Text&
    Text::operator=(const Text& other)
    {
        copyState(other);
        return *this;
    }

    const ItemTraits&
    Text::getItemTraits() const
    {
        return ItemTraits::getItemTraits(ItemType::Text);
    }
    
    void
    Text::copyState(const Text& other)
    {
        if (this != &other) {
            mText = other.mText;
            mPlacement = other.mPlacement;
        }
    }
    
    ItemUPtr
    Text::getMemento() const
    {
        return std::make_unique<Text>(*this);
    }
    
    void
    Text::restoreMemento(const Item& inItem)
    {
        const auto wasQuiescent = getIsQuiescent();
        const auto itemP = &inItem;
        const Text* downcastedP = dynamic_cast<const Text*>(itemP);
        if (!downcastedP) {
            KOMP_THROW("incorrect type was passed");
        }
        copyState(*downcastedP);
        setIsQuiescent(wasQuiescent);
    }
    
    Rect
    Text::getFrame() const
    {
        // Get the text metrics
        const auto size = MeasureText(mFont,
                                      mFontSize,
                                      mText.c_str());
        
        // Use ActualX, not DataX as the text can be placed at an arbitrary position in the measure?
        const auto posX = getMeasure()->getX() + getX();
        const auto posY = getStaff()->slotToPixel(getSlot());
        
        Rect result;
        result.left = posX;
        result.top = posY;
        result.setWidth(size.width);
        result.setHeight(size.height);
        
        return result;
    }

    void
    Text::draw()
    {
        setColorForState();

        // Set the text color
        const auto& color = ScoreColors::instance().TextColor;

        const auto textFrame = getFrame();
        inContext->drawText(mFont,
                            mFontSize,
                            textFrame.left,
                            textFrame.top,
                            mText.c_str(),
                            color);
    }
    
    //---------------------------------------------------------------------------
    // applyOffsetX()
    //
    /// Given the value, calculate the offset from the default position.
    /// The value passed in is 10ths of a staff line space.  Convert this
    /// value to pixels and apply it to our horizonal position.
    ///
    /// \param inValue The offset value in 10th of a staff line
    //---------------------------------------------------------------------------
    void
    Text::applyOffsetX(int inValue)
    {
        Engraver& engraver = Engraver::instance();
        const int pixels = inValue * engraver.scaleFactor();
        setX(getX() + pixels);
    }
    
    //---------------------------------------------------------------------------
    // applySlotOffsetY()
    //
    /// Given the value, calculate the offset from the default position.
    /// The value passed in is 10ths of a staff line space.
    /// Apply this value to our default positioning value.
    ///
    /// \param inValue The offset value in 10th of a staff line
    //---------------------------------------------------------------------------
    void
    Text::applySlotOffsetY(int inValue)
    {
        // Get the current y position in pixels
        const int slots = inValue / 10;
        setSlot(getSlot() + slots);
    }
    
    //---------------------------------------------------------------------------
    // fitItem()
    //
    /// Force the text item to hold the text it contains.
    /// We can optionally reset the SOFT_CRs in the data stream to match
    /// the current size of the text item. This is used when resizing
    /// text. But we still need it to snap to size around the text even so
    ///
    /// NOTE: CalcGraphicPos() and Graphic::getRects() are both
    /// used by this function. They require that gCurrent->Page and
    /// getMeasure() be set for the graphic we are fitting and that
    /// Score::layoutPage() has been called for that page.
    //
    /// \param keepSoftCRs - don't change the line breaks
    //---------------------------------------------------------------------------
    void
    Text::fitItem(bool keepSoftCRs)
    {
#if 0
        Rect r, r2;
        int w, h, saveZoom;
        
        // Find the current text dimensions
        text->getRects(getMeasure(), &r, &r2, false, gCurrent->Page);
        
        // Build a text edit record
        te = BuildTERec(GetTextItemPtr(text->Index), &r, keepSoftCRs);
        
        // Find the dimensions of the text
        TextEditMetrics(te, &w, &h);
        if (r.right - r.left != w || r.bottom - r.top != h)
        {
            Point p1, p2;
            Text temp = *text;
            int m;
            
            // Find the new coordinates of the lower right corner. Use a temp variable so
            // the other fields (time, X1, etc.) won't be changed.
            p1.x = r.left; p1.y = r.top;
            p2.x = r.left + w; p2.y = r.top + h;
            m = CalcGraphicPos( &temp, p1, p2);
            
            // We have to treat these return values carefully. CalcGraphicPos()
            // can return a Y2 relative to an entirely different Y1 (in a different
            // TrackHand) so just use the total difference to get Y2.
            text->Y2 = text->Y1 + temp.Y2 - temp.Y1;
            
            // And if the measure changes we can't get anything meaningful out of
            // the values for our X2 so bail.
            if (m == getMeasure()) {
                text->M2 = temp.M2;
                text->X2 = temp.X2;
            }
            // else we're screwed; leave it alone
        }
        
        // If we need to reset the line starts (SOFT_CRs) then do so
        if (!keepSoftCRs) {
            //DeleteTextItem(text->Index);
            text->Index = AddTextItem((char *) bloopBuffer, false);
            if (text->Index == -1)
                SetItemState(text, DELETE_STATE);
        }
#endif
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
