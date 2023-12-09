// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ItemImpl.h"

// System
#include <iomanip>
#include <sstream>
#include <string>

// Local
#include "BeamIntf.h"
#include "DrawableContext.h"
#include "DrawableText.h"
#include "Engraver.h"
#include "FontManager.h"
#include "Item.h"
#include "Note.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Track.h"
#include "Preferences.h"
#include "SpatialPointIntf.h"
#include "Staff.h"

namespace komp
{
bool
ItemImpl::contains(const Item& inItem,
                   const Point& inPoint,
                   bool inPrecise)
{
    if (inPrecise) {
        
        // Check the region first
        const auto itemRegion = inItem.getRegion();
        if (itemRegion.isValid()) {
            return itemRegion.contains(inPoint);
        } else {
            // Check the rect if no region is available
            const auto itemRect = inItem.getFrame();
            if (itemRect.getIsEmpty()) {
                return false;
            } else {
                const auto result = itemRect.contains(inPoint);
                return result;
            }
        }
        
    } else {
        
        // Start by checking the frame
        const auto itemRect = inItem.getFrame();
        if (itemRect.getIsEmpty()) {
            
            // The item cannot provide a bounding rect.
            // Get the more accurate region.
            const auto itemRegion = inItem.getRegion();
            return itemRegion.contains(inPoint);
        } else {
            return itemRect.contains(inPoint);
        }
    }
}

void
ItemImpl::registerObserver(ItemObserverRefVec& outObservers,
                           ItemObserver& inObserver)
{
    auto predicate = [&](ItemObserverRef& observer) {
        return &observer.get() == &inObserver;
    };
    
    auto iter = std::find_if(outObservers.begin(),
                             outObservers.end(),
                             predicate);
    
    if (iter == outObservers.end()) {
        outObservers.emplace_back(inObserver);
    }
}

void
ItemImpl::unregisterObserver(ItemObserverRefVec& outObservers,
                             ItemObserver& inObserver)
{
    for (auto it = outObservers.cbegin(); it != outObservers.cend(); ++it) {
        auto observer = *it;
        if (&observer.get() == &inObserver) {
            it = outObservers.erase(it);
            --it;
            continue;
        }
    }
}


void
ItemImpl::notifyObservers(const Item& item,
                          const ItemObserverRefVec& outObservers,
                          const ItemChanges& inChanges)
{
    for (const auto& observer : outObservers) {
        observer.get().observedItemChanged(item, inChanges);
    }
}


void
ItemImpl::notifyObservers(const Item& item,
                          const ItemObserverRefVec& outObservers,
                          const ItemChanges& inChanges,
                          int& outObserverSpinCount)
{
    if (item.getIsQuiescent()) {
        return;
    }
    
    if (outObserverSpinCount < ItemImpl::MAX_OBSERVER_SPIN_COUNT) {
        ++outObserverSpinCount;
        ItemImpl::notifyObservers(item, outObservers, inChanges);
        --outObserverSpinCount;
    } else {
        LOG_F(WARNING, "Spin count exceeded: %d", outObserverSpinCount);
    }
}


bool
ItemImpl::selected(bool inCurrentSelectionState)
{
    return inCurrentSelectionState == true;
}


void
ItemImpl::select(bool& outCurrentSelectionState)
{
    outCurrentSelectionState = true;
}


void
ItemImpl::deselect(bool& outCurrentSelectionState)
{
    outCurrentSelectionState = false;
}


void
ItemImpl::toggleSelection(bool& outCurrentSelectionState)
{
    if (outCurrentSelectionState) {
        outCurrentSelectionState = false;
    } else {
        outCurrentSelectionState = true;
    }
}


double
ItemImpl::getSpaceSize(const Item& inItem)
{
    return Staff::getSpacing();
}


double
ItemImpl::getFontSize(const Item& inItem)
{
    const auto& loc = inItem.getLocation();
    const auto& staff = loc.getStaff();
    return staff.getFontSize();
}

double
ItemImpl::characterWidth(const Item& inItem, const unsigned short inChar)
{
    const auto size = measureCharacter(inItem, inChar);
    return size.width;
}


double
ItemImpl::characterHeight(const Item& inItem,const unsigned short inChar)
{
    const auto size = measureCharacter(inItem, inChar);
    return size.height;
}


Rect
ItemImpl::characterFrame(const Item& inItem,const unsigned short inChar)
{
    const auto& loc = inItem.getLocation();
    const auto fontSize = loc.getStaff().getFontSize();
    return CharacterFrame(fontSize, inChar);
}


Size
ItemImpl::measureCharacter(const Item& inItem, const unsigned short inChar)
{
    const auto& loc = inItem.getLocation();
    const auto fontSize = loc.getStaff().getFontSize();
    return MeasureCharacter(fontSize, inChar);
}


void
ItemImpl::removeSelf(Item& inItem)
{
    const auto& loc = inItem.getLocation();
    auto& measure = loc.getMeasure();
    measure.removeItem(inItem);
}


int
ItemImpl::getSlot(const SpaceTimePoint& inSpaceTimePoint)
{
    return static_cast<int>(std::ceil(inSpaceTimePoint.getSlot() - 0.5));
}

Measure&
ItemImpl::getMeasure(const Item& inItem)
{
    const auto& loc = inItem.getLocation();
    return loc.getMeasure();
}

int
ItemImpl::getRoundedTime(const Item& inItem)
{
    return static_cast<int>(std::ceil(inItem.getTime() - 0.5));
}

void
ItemImpl::setTime(Item& outItem, SpaceTimePoint& outSpaceTimePoint, double inValue)
{
    if (inValue == outItem.getTime()) {
        return;
    }
    outSpaceTimePoint.setTime(inValue);
    outItem.notifyObservers({ItemChangeType::Time});
}

void
ItemImpl::drawItemTime(const Item& inItem,
                       const DrawableContext& inContext,
                       DrawableText& inText)
{
    inText.restoreParent();
    
    std::stringstream ss;
    ss << std::fixed;
    ss << std::setprecision(0);
    ss << inItem.getTime();
    std::string str = ss.str();
    DrawableChars uchars;
    for (const auto& c : str) {
        uchars.emplace_back(static_cast<unsigned short>(c));
    }
    
    inText.setText(std::move(uchars));
    auto fontSize = inItem.getFontSize();
    fontSize *= 0.25;
    inText.setFontSize(fontSize);
    const auto& fontName = FontManager::instance().getTextFontName();
    inText.setFontName(fontName);
    
    DrawableColors textColors;
    textColors.fillColor = {1.0, 0.0, 0.0};
    textColors.strokeColor = {1.0, 0.0, 0.0};
    
    const DrawParams params {
        inContext,
        Point {
            inItem.getScoreX(),
            inItem.getFrame().top - 10.0
        },
        textColors
    };
    
    inText.draw(params);
}

void
ItemImpl::setX(Item& outItem,
               double inValue,
               SpaceTimePoint& outSpaceTimePoint)
{
    if (inValue == outItem.getX()) {
        return;
    }
    outSpaceTimePoint.setMeasureRelativeX(inValue);
    outItem.notifyObservers({ItemChangeType::X});
}


void
ItemImpl::setSlot(Item& outItem,
                  int inValue,
                  SpaceTimePoint& outSpaceTimePoint)
{
    if (inValue == outItem.getSlot()) {
        return;
    }
    outSpaceTimePoint.setSlot(static_cast<double>(inValue));
    outItem.notifyObservers({ItemChangeType::Slot});
}


Point
ItemImpl::getDrawPoint(const Item& inItem,
                       const SpatialPointIntf& inSpatialPoint)
{
    Point p;
    p.x = inItem.getScoreX();
    p.y = inItem.getScoreY();
    return p;
}


/// \brief Update the item so that it will be drawn at the next screen refresh
void
ItemImpl::setNeedsDisplay(Item& inItem)
{
    const auto& loc = inItem.getLocation();
    auto& score = loc.getMeasure().getScore();
    const auto selected = score.getSelection()->contains(inItem);
    const auto voice = loc.getVoice();
    const auto drawPoint = inItem.getDrawPoint();
    const DrawParams dparams{
        score.getContext(),
        drawPoint,
        ScoreColors::instance().getColorsForState(selected, voice)
    };
        
    const auto drawable = ItemImpl::getDrawable(inItem);
    drawable->draw(dparams);
    
    // TODO: How can we properly propagate the update request?
    if (inItem.getItemTraits().getItemType() == ItemType::Note) {
        
        const auto& note = inItem.as<Note>();
        if (note.getIsBeamed()) {
            ItemImpl::setNeedsDisplay(*note.getBeam());
        }
        
        if (note.getIsChordMember()) {
            auto& chord = *note.getChord();
            ItemImpl::setNeedsDisplay(chord);

            if (chord.getIsBeamed()) {
                ItemImpl::setNeedsDisplay(*chord.getBeam());
            }
        }
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
