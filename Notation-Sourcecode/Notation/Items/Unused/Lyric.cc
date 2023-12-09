// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Lyric.h"

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "MIDIDynamic.h"
#include "MIDITypes.h"
#include "Preferences.h"
#include "Score.h"
#include "Temporal.h"

namespace
{
    static struct
    {
        int	LyricState;
        int	LyricX;
        int	LyricY;
        int	LyricLeft;
        int	LyricRight;
    } sMelismaInfo[MAX_VOICES];
    
    static const std::string HYPHEN = "-";
    static const std::string UNDERSCORE = "_";
    
    static const std::string MELISMA = "_";
    static const int MELISMA_SPACE = 2;
    static const int MELISMA_LEFT = 4;
}

namespace komp
{
    Lyric::~Lyric() = default;
    
    Lyric::Lyric(RelativePoint inRelativePoint)
    : komp::AttachedItem(inRelativePoint)
    , mDrawable()
    {
    }


    Lyric::Lyric(const Lyric& other)
    : komp::AttachedItem(other)
    {

    }


    Lyric&
    Lyric::operator=(const Lyric& other)
    {
        copyState(other);
        return *this;
    }


    Lyric::Lyric(Lyric&& other) = default;


    Lyric&
    Lyric::operator=(Lyric&& other) = default;


    const ItemTraits&
    Lyric::getItemTraits() const
    {
        return ItemTraits::getItemTraits(ItemType::Lyric);
    }

    void
    Lyric::copyState(const Lyric& other)
    {
        AttachedItem::operator=(other);
        //mMarkData = other.mMarkData;
    }
    
    ItemUPtr
    Lyric::getMemento() const
    {
        return std::make_unique<Lyric>(*this);
    }
    
    void
    Lyric::restoreMemento(const Item& inItem)
    {
        const auto wasQuiescent = getIsQuiescent();
        const auto itemP = &inItem;
        const Lyric* downcastedP = dynamic_cast<const Lyric*>(itemP);
        if (!downcastedP) {
            KOMP_THROW("incorrect type was passed");
        }
        copyState(*downcastedP);
        setIsQuiescent(wasQuiescent);
    }

    void
    Lyric::update()
    {
    }

    void
    Lyric::observedItemChanged(Item& observedItem,
                              ItemChanges inChanges)
    {
        if (ItemObserver::contains(inChanges, ItemChangeType::Slot) ||
            ItemObserver::contains(inChanges, ItemChangeType::StemDirection)) {
        }
    }

    Rect
    Lyric::getFrame() const
    {
        Rect result;
        
        const double x = getScoreX();
        const double y = getScoreY();
        
        // Get the expression font
        const auto& font = Preferences::instance().LyricFont;
        
        // Get the text metrics
        const CGSize size = MeasureText(font.Font,
                                        font.Size,
                                        "lyric");
        
        result.left = x;
        result.top = y;
        result.setWidth(size.width);
        result.setHeight(size.height);
        
        return result;
    }
    
    Region
    Lyric::getRegion() const
    {
        // Create the region to return
        CGMutablePathRef path = CGPathCreateMutable();
        CGPathAddRect(path, NULL, getFrame());
        return path;
    }
    
    void
    Lyric::inserted()
    {
        AttachedItem::inserted(inLocation);
        
        auto item& = getRelativePoint().getItem();
        item->registerObserver(*this);
        mDrawable.setFontSize(getFontSize());
        mDrawables.restoreParent();
    }

    void
    Lyric::removed()
    {
        auto item& = getRelativePoint().getItem();
        item->unregisterObserver(*this);
        AttachedItem::removed();
        mDrawables.removeFromParent();
    }


    double
    Lyric::getX() const
    {
        return AttachedItem::getX();
    }


    void
    Lyric::setX(double inValue)
    {
        AttachedItem::setX(inValue);
    }


    double
    Lyric::getScoreX() const
    {
        return AttachedItem::getScoreX();
    }


    double
    Lyric::getScoreY() const
    {
        return AttachedItem::getScoreY();
    }


    int
    Lyric::getSlot() const
    {
        return AttachedItem::getSlot();
    }


    void
    Lyric::setSlot(int inValue)
    {
        AttachedItem::setSlot(inMeasure, inValue);
    }


    void
    Lyric::beginDrag(const CurrentState& inState)
    {
        if (getRelativePoint().getItem()->selected()) {
            return;
        }

        select();
    }


    void
    Lyric::drag(const CurrentState& inState)
    {
        if (getRelativePoint().getItem()->selected()) {
            return;
        }

        auto delta = dragDelta();
        if(delta.x == 0 && delta.y == 0) {
            return;
        }
        
        auto& relativePoint = getRelativePoint();
        relativePoint.addX(delta.x);
        relativePoint.addYPixels(delta.y);
    }


    void
    Lyric::endDrag(const CurrentState& inState)
    {
        draw(inState.getContext());
    }

#pragma mark - Melismas
    
    static void
    DrawMelisma(const DrawableContext& inContext,
                short inStartX,
                short inStartY,
                short inEndX,
                short inEndY,
                short inWidth)
    {
        inContext->setLineWidth(inWidth);
        inContext->drawLine(inStartX, inStartY, inEndX, inEndY);
    }
    
    bool
    Lyric::getStartsMelisma() const
    {
        return mLyricFlags[MELISMA_START];
    }
    
    void
    Lyric::setMelismaStart(bool inValue)
    {
        mLyricFlags[MELISMA_START] = inValue;
    }
    
    bool
    Lyric::getEndsMelisma() const
    {
        return mLyricFlags[MELISMA_END];
    }
    
    void
    Lyric::setMelismaEnd(bool inValue)
    {
        mLyricFlags[MELISMA_END] = inValue;
    }
    
    
    bool
    Lyric::hasRightTie() const
    {
        return mLyricFlags[LYRIC_TIED_RIGHT];
    }
    
    
    void
    Lyric::draw(const DrawableContext& inContext)
    {
        KOMP_LOGW("Lyric::draw is for losers.");
//        setColorForState();
//        
//        const auto& score = getScore();
//        const auto& measure = getMeasure();
//        //const auto textFrame = getFrame();
//        const auto fontSize = getFontSize();
//
//        // Get the lyric color
//        const auto& colors = ScoreColors::instance();
//        const auto& color = selected() ? colors.SelectedItemColor : colors.TextColor;
//
//        const auto noteWidth = fmax(1, characterWidth(FontTheme::instance().NoteheadTypeMap[TemporalType::QUARTER]));
//        const int lyricVoice = getVoice();
//        
//        const Rect lyricFrame = getFrame();
//        
//        int melismaX;
//        if (mLineOne == UNDERSCORE && getEndsMelisma()) {
//            
//            // Draw the text and melisma below
//            melismaX = lyricFrame.left;
//            
//        } else {
//            
//            // Draw the basic text
//            auto textWidth = TextWidth("", fontSize, mLineOne.c_str());
//            
//            
//            if (getStartsMelisma() || hasRightTie()) {
//                inContext->drawText("",
//                                    fontSize,
//                                    lyricFrame.left,
//                                    lyricFrame.top,
//                                    mLineOne.c_str(),
//                                    color);
//            } else {
//                textWidth /= 2;
//                inContext->drawText("",
//                                    fontSize,
//                                    lyricFrame.left,
//                                    lyricFrame.top,
//                                    mLineOne.c_str(),
//                                    color);
//            }
//            melismaX = lyricFrame.left + MELISMA_SPACE + textWidth;
//        }
//        
//        if (getEndsMelisma()) {
//            
//            // Draw the melisma line
//            const double width = 1;
//            const double offsetY = 0;
//            
//            // Set left coordinate if start of this melisma on previous system
//            if (sMelismaInfo[lyricVoice].LyricState) {
//                sMelismaInfo[lyricVoice].LyricState = 0;
//            } else {
//                const auto dataLeft = measure->getDataX();
//                sMelismaInfo[lyricVoice].LyricX = dataLeft + MELISMA_LEFT;
//            }
//            
//            if (!mLineTwo.empty()) {
//                const auto four = 4;
//                const auto two = 2;
//                
//                // Make room for following text (the -4)
//                DrawMelisma(inContext,
//                            sMelismaInfo[lyricVoice].LyricX,
//                            lyricFrame.top - offsetY,
//                            lyricFrame.left - four,
//                            lyricFrame.top - offsetY, width);
//                
//                // Set the text color
//                const auto& prefs = komp::Preferences::instance();
//                const auto& color = prefs.Colors.TextColor;
//                inContext->drawText("",
//                                    fontSize,
//                                    lyricFrame.left,
//                                    lyricFrame.top,
//                                    mLineTwo.c_str(),
//                                    color);
//                melismaX = lyricFrame.left + TextWidth("", fontSize, mLineTwo.c_str()) + two;
//                
//            } else {
//
//                // TODO: Add 1 so melisma gets to end of the note
//                const int one = 1;
//                melismaX = lyricFrame.left + noteWidth + one;
//                DrawMelisma(inContext,
//                            sMelismaInfo[lyricVoice].LyricX,
//                            lyricFrame.top - offsetY,
//                            melismaX,
//                            lyricFrame.top - offsetY,
//                            width);
//            }
//        }
//        
//        if (getStartsMelisma()) {
//            sMelismaInfo[lyricVoice].LyricY = lyricFrame.top;
//            sMelismaInfo[lyricVoice].LyricX = melismaX;
//            const int dataRight = measure->getX() + measure->getWidth();
//            sMelismaInfo[lyricVoice].LyricRight = dataRight - (noteWidth / 2);
//            sMelismaInfo[lyricVoice].LyricState = true;
//        }
    }
    
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

