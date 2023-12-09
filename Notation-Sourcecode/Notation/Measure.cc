// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Measure.h"

// System
#include <limits.h>
#include <sstream>

// Local
#include "AttachedItem.h"
#include "ChordSymbol.h"
#include "Clef.h"
#include "Command.h"
#include "Curve.h"
#include "DrawableColor.h"
#include "DrawableContext.h"
#include "Engraver.h"
#include "Item.h"
#include "ItemImpl.h"
#include "KeySignature.h"
#include "MeasureObserver.h"
#include "Note.h"
#include "NoteSpacerIntf.h"
#include "Octave.h"
#include "Preferences.h"
#include "Rest.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Selection.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "TempoMark.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "TimeMap.h"
#include "Track.h"

namespace
{
static constexpr const double DEFAULT_RIGHT_GAP = 2.0;

// Default measure width of 1.5"
static constexpr const double DEFAULT_NOTE_SPACE = (3 * PIXELS_PER_INCH) / 2;

}

namespace komp
{
Measure::Measure(Score& inScore)
: mScore(inScore)
, mBarlineLeft{BarlineType::LEFT_NORMAL}
, mBarlineRight{BarlineType::RIGHT_NORMAL}
, mBarlineSelection(BarlineLocation::NONE)
, mMeterVisible{false}
, mNoteSpace(DEFAULT_NOTE_SPACE)
, mRightGap(DEFAULT_RIGHT_GAP)
, mTrackMap()
{
}

Measure::~Measure()
{
    for (const auto& item : mLeftGapMap) {
        item.second->removeFromParent();
    }

    for (const auto& item : mNoteSpaceMap) {
        item.second->removeFromParent();
    }

    for (const auto& item : mRightGapMap) {
        item.second->removeFromParent();
    }
}

// MARK: - Notifications

bool
Measure::getIsQuiescent() const
{
    for (const Item& item : mItems) {
        if (!item.getIsQuiescent()) {
            return false;
        }
    }
    return true;
}


void
Measure::setIsQuiescent(bool inValue)
{
    for (Item& item : mItems) {
        item.setIsQuiescent(inValue);
    }
}

// MARK: - Geometry

void
Measure::setX(double inX)
{
    mX = inX;
    //write_lock lock(mMutex);
    for (Item& item : mItems) {
        // TODO: Improve item invalidation here.
        item.notifyMeasureXChanged();
        item.update();
    }
}

static Rect
getPreciseItemFrame(const Item& inItem)
{
    auto itemRect = inItem.getFrame();
    if (itemRect.getIsEmpty()) {
        // Get a more precise region. The rect may be invalid
        // for items that span multiple measures or lines.
        const Region itemRegion = inItem.getRegion();
        if (itemRegion.isValid()) {
            itemRect = itemRegion.getBounds();
        } else {
            LOG_F(WARNING, "Unable to get frame or region");
        }
    }
    
#ifdef DEBUG
        if (std::isinf(itemRect.top) || std::isnan(itemRect.top) ||
            std::isinf(itemRect.bottom) || std::isnan(itemRect.bottom) ||
            std::isinf(itemRect.left) || std::isnan(itemRect.left) ||
            std::isinf(itemRect.right) || std::isnan(itemRect.right)) {
            KOMP_THROW("Bad Rect");
        }
#endif
    
    return itemRect;
}

/// \brief Calculate the frame
/// \param inStaff The staff to use for coordinates
/// \result The frame around the given measure
Rect
Measure::getFrame(const Staff& inStaff) const
{
    // Initialize the enclosing rect to be the barlines
    auto outRect = getTightFrame(inStaff);
    
    if (getIndex() == 0 && mScore.getShowInstrumentNames()) {
        outRect.left = 0;
    }
    
    auto& staffTrack = inStaff.getTrack();
    const auto hand = inStaff.getHand();
    
    const auto itemsList = items<Item>(staffTrack,
                                       [&](Item& filterItem) {
        return filterItem.getLocation().getHand() == hand;
    });
    
    for (const Item& item : itemsList) {
        const auto itemRect = getPreciseItemFrame(item);
        if (!itemRect.getIsEmpty()) {
            outRect.unionRect(itemRect);
        }
    }
    
    return outRect;
}


/// \discussion Calculate the frame of the measure which just includes the width
/// from barline to barline of unpopulated staff lines, including
/// estimated clefs and key signatures sizes (two slots).
/// \param inStaff The staff to be used in the calculation
/// \result The tight frame of the measure
Rect
Measure::getTightFrame(const Staff& inStaff) const
{
    const auto& engraver = Engraver::instance();

    const auto staffY = inStaff.getScoreY();
    const auto top = staffY;
    const auto staffHeight = inStaff.getHeight();
    
    Rect measureFrame;
    measureFrame.setLeft(getX() - engraver.lightBarlineWidth());
    measureFrame.setTop(top);
    measureFrame.setRight(measureFrame.left + getWidth());
    measureFrame.setBottom(top + staffHeight);
        
    return measureFrame;
}

/// \brief Calculate the frame of the measure items above the top barline
/// \param inStaff The staff to be used in the calculation
/// \result The frame of the items above the top barline
Rect
Measure::getInterstaffUpperFrame(const Staff& inStaff) const
{
    auto outRect = getTightFrame(inStaff);
    const auto staffTop = outRect.top;
    outRect.bottom = staffTop;
    
    auto& staffTrack = inStaff.getTrack();
    const auto hand = inStaff.getHand();
    
    const auto itemsList = items<Item>(staffTrack,
                                       [&](Item& filterItem) {
        if (filterItem.getLocation().getHand() != hand) {
            return false;
        }

        const auto& traits = filterItem.getItemTraits();
        const auto itemType = traits.getItemType();
        if (itemType == komp::ItemType::Clef) {
            return false;
        }

        if (itemType == komp::ItemType::TempoMark) {
            return false;
        }

        return true;
    });
    
    for (const Item& item : itemsList) {
        const auto itemRect = getPreciseItemFrame(item);
        if (!itemRect.getIsEmpty() && itemRect.top <= staffTop) {
            outRect.top = std::min(outRect.top, itemRect.top);
        }
    }
    
    return outRect;
}

/// \brief Calculate the frame of the measure items below the bottom barline
/// \param inStaff The staff to be used in the calculation
/// \result The frame of the items below the bottom barline
Rect
Measure::getInterstaffLowerFrame(const Staff& inStaff) const
{
    auto outRect = getTightFrame(inStaff);
    const auto staffBottom = outRect.bottom;
    outRect.top = staffBottom;
    
    auto& staffTrack = inStaff.getTrack();
    const auto hand = inStaff.getHand();
    
    const auto itemsList = items<Item>(staffTrack,
                                       [&](Item& filterItem) {
        if (filterItem.getLocation().getHand() != hand) {
            return false;
        }
        
        const auto& traits = filterItem.getItemTraits();
        const auto itemType = traits.getItemType();
        if (itemType == komp::ItemType::Clef) {
            return false;
        }
        
        if (itemType == komp::ItemType::TempoMark) {
            return false;
        }
        
        return true;
    });
    
    for (const Item& item : itemsList) {
        const auto itemRect = getPreciseItemFrame(item);
        if (!itemRect.getIsEmpty() && itemRect.bottom >= staffBottom) {
            outRect.bottom = std::max(outRect.bottom, itemRect.bottom);
        }
    }
    
    return outRect;
}

double
Measure::getWidth() const
{
    return getNoteSpace() + getGap();
}

// MARK: - Draw

void
Measure::draw()
{
    const auto& context = mScore.getContext();
    const auto& staffSystem = getStaffSystem();
    const auto& staves = staffSystem->getStaves();
    for (const auto& staff : staves) {
        draw(context, *staff.get());
    }
}

void
Measure::draw(const DrawableContext& inContext, Staff& inStaff)
{
    const auto& engraver = Engraver::instance();
    const auto& spacing = engraver.spacing();

    const auto barlineWidth = getLeftBarlineWidth();
    const auto clefOffset = Staff::getSpacing() * spacing.ClefSpace;
    
    auto& staffTrack = inStaff.getTrack();

    // Clef placement
    const auto clefs = items<Clef>(staffTrack);
    double clefWidth = 0;
    double courtesyGap = 0;
    
    for (Clef& clef : clefs) {
        
        ItemImpl::setNeedsDisplay(clef);
        
        if (clef.getTime() == 0) {
            
            const auto frameWidth = clef.getFrame().getWidth();
            if (clef.isSmall()) {
                // Position the clef to the left of the right barline
                const auto clefInset = clefOffset + frameWidth;
                clef.setX(-clefInset);
                courtesyGap = std::max(courtesyGap, clefInset);
            } else {
                clefWidth = std::max(clefWidth, frameWidth);
                clef.setX(barlineWidth + clefOffset);
            }
        }
    }
    
    // Update courtesy gap if we placed any clefs to the left of the right barline
    const auto measureIndex = getIndex();
    if (courtesyGap && measureIndex != 0) {
                
        const auto& previous = mScore.getMeasure(measureIndex - 1);
        previous->setCourtesyGap(courtesyGap);
        
        const auto newX = previous->getX() + previous->getWidth();
        setX(newX);
    }
    
    auto offsetX = barlineWidth;
    if (clefWidth) {
        offsetX += clefWidth + clefOffset;
    }
    
    // Get the maximum width of all key signatures
    auto keyWidth = 0.0;
    const auto allKeys = items<KeySignature>();
    for (KeySignature& key : allKeys) {
        const auto frameWidth = key.getFrame().getWidth();
        keyWidth = std::max(keyWidth, frameWidth);
    }
    
    const auto staffHand = inStaff.getHand();
    
    // Key Signature placement
    const auto keys = items<KeySignature>(staffTrack,
                                          [&](Item& filterItem) {
        return filterItem.getLocation().getHand() == staffHand;
    });
    
    const auto keySpace = Staff::getSpacing() * spacing.KeySignatureSpace;
    for (KeySignature& key : keys) {
        ItemImpl::setNeedsDisplay(key);
        key.setX(offsetX + keySpace);
    }
    
    if (keyWidth > 0) {
        offsetX += keyWidth + keySpace;
    }
    
    if (getMeterVisible() && mMeterMap.contains(&inStaff)) {
        
        const auto meterPix = Staff::getSpacing() * spacing.MeterSpace;
        const auto meterX = getX() + meterPix +  + offsetX;
        const auto meterY = inStaff.slotToPixel(mTimeSignatureParams.slot());
        const auto& color = ScoreColors::instance().textColor();
        
        const DrawParams dparams {
            mScore.getContext(),
            { meterX, meterY },
            { color, color }
        };
        mMeterMap.at(&inStaff)->draw(dparams);
    }
    
    drawBarlines(inContext, inStaff);
    
    // Items for the track and hand placement
    const auto trackItems = items<Item>(staffTrack,
                                        [&](Item& filterItem) {
        return filterItem.getLocation().getHand() == staffHand;
    });
    
    const auto& drawables = mScore.getDrawableRegistry();
    for (Item& trackItem : trackItems) {
        const auto selected = mScore.getSelection()->contains(trackItem);
        const auto voice = trackItem.getLocation().getVoice();
        const DrawParams dparams{
            inContext,
            trackItem.getDrawPoint(),
            ScoreColors::instance().getColorsForState(selected, voice)
        };

        auto& drawable = drawables.at(trackItem.getItemID());
        drawable->draw(dparams);
    }
    
    if (Preferences::instance().getShowItemFrames()) {
        
        const auto measureRect = getTightFrame(inStaff);
        
        const auto posY = inStaff.getScoreY();
        
        // Set the left gap
        auto gapRect = measureRect;
        const auto leftGap = getLeftGap();
        gapRect.setWidth(leftGap);
        
        if (!mLeftGapMap.contains(&inStaff)) {
            auto drawable = std::make_unique<DrawableRectangle>();
            mLeftGapMap[&inStaff] = std::move(drawable);
        }
        const auto& leftGapDrawable = mLeftGapMap.at(&inStaff);
        leftGapDrawable->setBounds(Rect(0,
                                        0,
                                        gapRect.getHeight(),
                                        gapRect.getWidth()));
        {
            // Left gap is blue
            static const auto color = DrawableColor(0.0, 0.0, 1.0, 0.15);
            const DrawParams dparams{
                inContext,
                {mX, posY},
                {color, color}
            };
            leftGapDrawable->draw(dparams);
        }
        
        // Set the notespace
        auto noteRect = measureRect;
        noteRect.left = gapRect.right;
        noteRect.setWidth(getNoteSpace());
        
        if (!mNoteSpaceMap.contains(&inStaff)) {
            auto drawable = std::make_unique<DrawableRectangle>();
            mNoteSpaceMap[&inStaff] = std::move(drawable);
        }
        const auto& noteSpaceDrawable = mNoteSpaceMap.at(&inStaff);
        noteSpaceDrawable->setBounds(Rect(0,
                                          0,
                                          noteRect.getHeight(),
                                          noteRect.getWidth()));
        {
            // Note space is green
            static const auto color = DrawableColor(0.0, 1.0, 0.0, 0.15);
            const DrawParams dparams{
                inContext,
                {mX + mLeftGap, posY},
                {color, color}
            };
            noteSpaceDrawable->draw(dparams);
        }
        
        // Set the right gap
        gapRect = measureRect;
        const auto rightGap = mRightGap;
        gapRect.setWidth(rightGap);
        gapRect.left = measureRect.right - rightGap;
        gapRect.right = gapRect.left + rightGap;
        
        if (!mRightGapMap.contains(&inStaff)) {
            auto drawable = std::make_unique<DrawableRectangle>();
            mRightGapMap[&inStaff] = std::move(drawable);
        }
        const auto& rightGapDrawable = mRightGapMap.at(&inStaff);
        rightGapDrawable->setBounds(Rect(0,
                                         0,
                                         gapRect.getHeight(),
                                         gapRect.getWidth()));
        {
            // Right gap is blue
            static const auto color = DrawableColor(1.0, 0.0, 0.0, 0.15);
            const DrawParams dparams{
                inContext,
                {gapRect.left, posY},
                {color, color}
            };
            rightGapDrawable->draw(dparams);
        }
    } else {
        if (mLeftGapMap.contains(&inStaff)) {
            mLeftGapMap[&inStaff]->removeFromParent();
        }
        if (mNoteSpaceMap.contains(&inStaff)) {
            mNoteSpaceMap[&inStaff]->removeFromParent();
        }
        if (mRightGapMap.contains(&inStaff)) {
            mRightGapMap[&inStaff]->removeFromParent();
        }
    }
}

/// \brief Add the measure meter to the staff
/// \param inStaff The staff to draw in
void
Measure::addMeter(Staff& inStaff)
{
    auto meter = std::make_unique<DrawableTimeSignature>();
    meter->update(&inStaff,
                  mTimeSignatureParams.type,
                  mTimeSignatureParams.beatsPerBar,
                  mTimeSignatureParams.beatUnit);
    mMeterMap[&inStaff] = std::move(meter);
}

void
Measure::configureBarlines(Staff& inStaff)
{
    const auto drawLeft = getLeftBarline() != BarlineType::LEFT_NONE;
    const auto drawRight = getRightBarline() != BarlineType::RIGHT_NONE;
    if (!drawLeft && !drawRight) {
        return;
    }
    
    const auto staffHeight = inStaff.getHeight();
    
    if (drawLeft) {
        
        auto update = true;
        
        if (mLeftBarlines.count(&inStaff)
            && mLeftBarlines.at(&inStaff)->getType() == getLeftBarline()) {
            update = false;
        }
        
        if (update) {
            auto barline = std::make_unique<DrawableBarline>(getLeftBarline(),
                                                             staffHeight);
            barline->setZOrder(-1);
            mLeftBarlines[&inStaff] = std::move(barline);
        }
    }
    
    if (drawRight) {
        
        auto update = true;
        
        if (mRightBarlines.count(&inStaff)
            && mRightBarlines.at(&inStaff)->getType() == getRightBarline()) {
            update = false;
        }
        
        if (update) {
            auto barline = std::make_unique<DrawableBarline>(getRightBarline(),
                                                             staffHeight);
            barline->setZOrder(-1);
            mRightBarlines[&inStaff] = std::move(barline);
        }
    }
}

void
Measure::drawBarlines(const DrawableContext& inContext, Staff& inStaff)
{
    configureBarlines(inStaff);
    
    const auto drawLeft = getLeftBarline() != BarlineType::LEFT_NONE;
    const auto drawRight = getRightBarline() != BarlineType::RIGHT_NONE;
    if (!drawLeft && !drawRight) {
        return;
    }
    
    const auto posX = getX();
    const auto& system = getStaffSystem();
    const auto systemY = system->getY();
    const auto posY = systemY + inStaff.getY();
    
    const auto& color = ScoreColors::instance().defaultColor();
    
    if (drawLeft) {
        
        const DrawParams dparams{
            inContext,
            {posX, posY},
            {color, color}
        };
        mLeftBarlines[&inStaff]->draw(dparams);
    }
    
    if (drawRight) {
        
        const DrawParams dparams{
            inContext,
            {posX + getWidth(), posY},
            {color, color}
        };
        mRightBarlines[&inStaff]->draw(dparams);
    }

}

void
Measure::setBarlineSelection(BarlineLocation inLocation)
{
    if (mBarlineSelection != inLocation) {
        mBarlineSelection = inLocation;
    }
}

// MARK: - Insertion

/// \brief Called by the score afer the measure has been added to it
void
Measure::inserted()
{
    const auto& items = allItems();
    for (auto item : items) {
        item.get().inserted();
    }
    
    for (const auto& left : mLeftBarlines) {
        const auto& barline = left.second;
        barline->restoreParent();
    }

    for (const auto& right : mRightBarlines) {
        const auto& barline = right.second;
        barline->restoreParent();
    }
    
    for (const auto& meterItem : mMeterMap) {
        meterItem.second->restoreParent();
    }
    
    for (const auto& keyItem : mKeyMap) {
        keyItem.second->restoreParent();
    }
}

/// \brief Called by the score afer the measure has been removed from it
void
Measure::removed()
{
    const auto& items = allItems();
    for (auto item : items) {
        item.get().removed();
    }
    
    for (const auto& left : mLeftBarlines) {
        const auto& barline = left.second;
        barline->removeFromParent();
    }

    for (const auto& right : mRightBarlines) {
        const auto& barline = right.second;
        barline->removeFromParent();
    }
    
    for (const auto& meterItem : mMeterMap) {
        meterItem.second->removeFromParent();
    }

    for (const auto& keyItem : mKeyMap) {
        keyItem.second->removeFromParent();
    }
}

/// \brief Insert an item to the measure at the specified time.
/// \param inItem The item to insert
/// \param inTime The time within the measure to insert at
void
Measure::insertItem(Item& inItem, double inTime)
{
    //write_lock lock(mMutex);

    // Search for where we can insert the item
    int index = 0;
    for (const Item& item : mItems) {
        if (inTime <= item.getTime()) {
            break;
        }
        ++index;
    }
    inItem.setTime(inTime);
    
    mScore.getItemRegistry().setMeasure(inItem, this);
    
    const auto& loc = inItem.getLocation();
    const auto hand = loc.getHand();
    const auto voice = loc.getVoice();
    auto& track = loc.getTrack();
    
    const auto findTrackIter = mTrackMap.find(&track);
    if (findTrackIter == mTrackMap.cend()) {
        mTrackMap[&track] = std::make_unique<HandMap>();
    }
    
    // Get the HandMap for the item
    auto& handMap = mTrackMap[&track];
    const auto findHandMapIter = handMap->find(hand);
    if (findHandMapIter == handMap->cend()) {
        (*handMap)[hand] = std::make_unique<VoiceMap>();
    }
    
    // Get the VoiceMap for the item
    auto& voiceMap = handMap->at(hand);
    const auto findVoiceMapIter = voiceMap->find(voice);
    if (findVoiceMapIter == voiceMap->cend()) {
        (*voiceMap)[voice] = std::make_unique<ItemList>();
    }
    
    // Add the item and make sure their are no duplicates
    auto& voiceItems = voiceMap->at(voice);
    const auto duplicateIter = std::find_if(voiceItems->cbegin(),
                                            voiceItems->cend(),
                                            [&](const Item& item) {
        return &item == &inItem;
    });
    
    if (duplicateIter != voiceItems->cend()) {
        LOG_F(WARNING, "Item is already in measure");
    } else {
        
        // Locate the insertion point based on the items time
        int voiceIndex = 0;
        for (const Item& item : *voiceItems) {
            if (inTime <= item.getTime()) {
                break;
            }
            ++voiceIndex;
        }
        
        // Insert the item into the item list at the index
        const auto voiceIter = voiceItems->cbegin() + voiceIndex;
        voiceItems->insert(voiceIter, inItem);
    }
    
    // Insert the item into the item list at the index
    const auto iter = mItems.cbegin() + index;
    mItems.insert(iter, inItem);
    
    inItem.inserted();
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().itemAdded(*this, inItem);
    }
}

void
Measure::insertItem(Item& inItem)
{
    insertItem(inItem, inItem.getTime());
}

/// \brief Insert a clef in the track at a specific time position
/// \param inClef The clef to insert
/// \param inTrack The track to insert the clef in
/// \param inHand The hand of the track
/// \param inTime The time to insert the clef at
void
Measure::insertClef(ClefType inClef,
                    Track& inTrack,
                    int inHand,
                    double inTime)
{
    // Add the item
    const auto measureIndex = getIndex();
    const auto small = measureIndex > 0;
    
    const MusicLocation loc(*this, inTrack, inHand);
    auto& theClef = mScore.getItemRegistry().createItem<Clef>(loc,
                                                              inClef,
                                                              small);
    insertItem(theClef, inTime);
}

/// \brief Offset all items in the measure by the time
/// \param inItem The item that caused change
/// \param inAmount Time to adjust the current values by
void
Measure::offsetItemTimes(Temporal& inItem, double inAmount)
{
    const auto& loc = inItem.getLocation();
    auto& track = loc.getTrack();
    const auto voice = loc.getVoice();
    const auto hand = loc.getHand();
    
    bool foundFirst = false;
    
    const auto filter = [&](Item& filterItem) {
        
        const auto& filterLoc = filterItem.getLocation();
        
        if (filterLoc.getHand() != hand) {
            return false;
        }
        
        if (filterLoc.getVoice() != voice) {
            return false;
        }
        
        if (&filterLoc.getTrack() != &track) {
            return false;
        }
        
        if (!filterItem.is<Temporal>()) {
            return false;
        }
        
        const auto& temporal = filterItem.as<Temporal>();
        
        if (temporal.getIsChordMember()) {
            return false;
        }
        
        return true;
    };
    
    auto temporals = items<Temporal>(track, filter);
    komp::sortItems(temporals);
    
    for (Temporal& temporal : temporals) {
        
        if (foundFirst) {
            temporal.setTime(temporal.getTime() + inAmount);
        }
        
        if (!foundFirst) {
            // See if we have found ourself in the list of items
            foundFirst = &temporal == &inItem;
        }
    }
}

// MARK: - Membership

/// \brief Checks for item membership in measure items
/// \param inItem The item to check
/// \returns true if item is contained by measure
bool
Measure::contains(const Item& inItem)
{
    auto findIter = std::find_if(mItems.cbegin(),
                                 mItems.cend(),
                                 [&](Item& item) {
        return &inItem == &item;
    });
    return findIter != mItems.end();
}

// MARK: - Removal

/// \brief Removes the item from the measure.
/// \param inItem The item to remove from the measure
/// \param inDoUpdateTimes a true value will cause the following temporals to
///        have their time values updated. defaults to true for backward
///        compatibility with the various call sites
ItemRefVec::const_iterator
Measure::removeItem(Item& inItem, bool inDoUpdateTimes)
{
    //write_lock lock(mMutex);
    
    auto doFixChordMembers = false;
    
    if (inItem.getItemTraits().getItemType() == ItemType::Note) {
        const auto& note = inItem.as<Note>();
        if (note.getIsChordMember()) {
            const auto chord = note.getChord()->as<Chord>();
            const auto chordMembers = chord.getNotes();
            doFixChordMembers = chordMembers.size() > 2;
        }
    }
    
    // Remove the item from the global item list
    auto findIter = std::find_if(mItems.cbegin(),
                                 mItems.cend(),
                                 [&](Item& item) {
        return &inItem == &item;
    });
    
    KOMP_ASSERT(findIter != mItems.cend());
    findIter = mItems.erase(findIter);
    
    // Remove the item from the TrackMap
    removeItemInternal(inItem);
    inItem.removed();
    
    int duration = 0;
    
    if (inItem.getItemTraits().getItemType() == ItemType::Note)  {
        
        auto& note = inItem.as<Note>();
        if (doFixChordMembers) {
            
        } else {
            duration = note.getDuration().getClocks();
            if (inDoUpdateTimes) {
                offsetItemTimes(note, -duration);
            }
        }
    } else if (inItem.getItemTraits().getItemType() == ItemType::Rest) {
        auto& rest = inItem.as<Rest>();
        duration = rest.getDuration().getClocks();
        
        if (inDoUpdateTimes) {
            offsetItemTimes(rest, -duration);
        }
        
        // Compressed rest. Delete all at once. Change measure setting
        if (rest.getDuration().getTemporalType() == TemporalType::HALF) {
            //                setCompressedSize(0);
            //
            //                for (const auto& item : mItems) {
            //                    Rest* compressedRest = nullptr;
            //                    if (item.as<Rest>()) {
            //                        compressedRest = item.as<Rest>();
            //                    }
            //
            //                    if (compressedRest && compressedRest->getDuration().getTemporalType() == TemporalType::HALF) {
            //                        // NOTE: offsetItemTimes() makes no sense here since
            //                        // the rest takes up the whole measure.
            //
            //                        // Redraw it all
            //                    }
            //                }
        }
    } else if (inItem.is<Clef>()) {
        //            const auto& clef = inItem.as<Clef>();
        //            changeKeySignature(inItem.getTrack(),
        //                               hand,
        //                               this,
        //                               clef->getX(),
        //                               false,
        //                               0,
        //                               -1,
        //                               false);
    }
    
    mScore.getItemRegistry().setMeasure(inItem, nullptr);
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().itemRemoved(*this, inItem);
    }

    return findIter;
}

/// \brief Removes the item from track/voice/hand map
/// \param inItem The item to remove from the map
void
Measure::removeItemInternal(Item& inItem)
{
    auto foundIt = false;
    
    const auto& loc = inItem.getLocation();
    const auto trackIter = mTrackMap.find(&loc.getTrack());
    if (trackIter != mTrackMap.cend()) {
        
        // Get the HandMap for the track
        const auto& handMap = mTrackMap[&loc.getTrack()];
        auto handIter = handMap->find(inItem.getLocation().getHand());
        if (handIter != handMap->cend()) {
            
            // Iterate all the voices for the hand
            const auto& voiceMap = handIter->second;
            auto voiceIter = voiceMap->cbegin();
            for ( ; voiceIter != voiceMap->cend(); ++voiceIter) {
                const auto& voiceItems = voiceIter->second;
                const auto itemIter = std::find_if(voiceItems->cbegin(),
                                                   voiceItems->cend(),
                                                   [&](const Item& item) {
                    return &item == &inItem;
                });
                if (itemIter != voiceItems->cend()) {
                    voiceItems->erase(itemIter);
                    foundIt = true;
                }
            }
        }
    }
    
    if (!foundIt) {
        KOMP_THROW("Unable to find item to remove in track map");
    }
}

/// \brief Removes staff items from the measure
/// \param inStaff The staff to remove related items from.
void
Measure::removeStaff(const Staff& inStaff)
{
    if (mLeftGapMap.contains(&inStaff)) {
        mLeftGapMap[&inStaff]->removeFromParent();
    }
    
    if (mNoteSpaceMap.contains(&inStaff)) {
        mNoteSpaceMap[&inStaff]->removeFromParent();
    }
    
    if (mRightGapMap.contains(&inStaff)) {
        mRightGapMap[&inStaff]->removeFromParent();
    }
    
    mLeftBarlines.erase(&inStaff);
    mRightBarlines.erase(&inStaff);
    mMeterMap.erase(&inStaff);
    mKeyMap.erase(&inStaff);
}

/// \brief Removes track from the item track map
/// \param inTrack The track to remove
void
Measure::removeTrack(const Track& inTrack)
{
    const auto iter = mTrackMap.find(&inTrack);
    if (iter != mTrackMap.end()) {
        mTrackMap.erase(iter);
    } else {
        // There may have not been any items added to the track.
        // In this case, there is no track map to remove
    }
}

// MARK: - Positioning

/// \brief Get the horizontal pixel position for the given clock
/// \param inClock The time
/// \result The result in pixels
double
Measure::positionForTime(int inClock) const
{
    if (getClocks() <= 0.0) {
        return 0;
    }
    
    if (getWidth() <= 0.0) {
        return 0;
    }
    
    // TODO: Cache the TimeMap if performance is a problem
    TimeMap tmap;
    tmap.setClocks(static_cast<int>(std::ceil(getClocks() - 0.5)));
    tmap.setWidth(getNoteSpace());
    
    // Iterate through all temporals that exist in the measure to create a map
    // of time vs x position. We want to prioritize the inTrack, so we will
    // add those to the map first, then we will proceed with the rest of the tracks.
    
    for (const auto& track : mScore.getTracks()) {
        
        const auto& temporals = items<Temporal>(*track.get());
        for (const Temporal& temporal : temporals) {
            tmap.addPoint(temporal.getRoundedTime(),
                          temporal.getX());
        }
    }
    
    return tmap.getXForTime(inClock);
}

/// \brief Set the horizontal pixel positions of the items.
/// \discussion Use the items optimal width and position based on either time or absolute position.
void
Measure::layout(NoteSpacerIntf& inStrategy)
{
    inStrategy.applySpacing(*this);
}

void
Measure::layout()
{
    if (getMeterVisible()) {
        
        const auto& system = getStaffSystem();
        const auto& staves = system->getStaves();
        
        for (const auto& staff : staves) {
            if (!mMeterMap.contains(staff.get())) {
                addMeter(*staff);
                setMeterVisible(true);
            }
        }
    }
    
    const auto strategy = NoteSpacerIntf::getDefaultStrategy();
    layout(*strategy);
    
    handleVerticalCollisions();
}

void
Measure::handleVerticalCollisions()
{
    handleTempoMarkCollisions();
    handleChordSymbolCollisions();
}

void
Measure::handleChordSymbolCollisions()
{
    // TODO: Deal with vertical collisions
    auto tempoMarks = getTempoMarks();
    if (tempoMarks.empty()) {
        return;
    }
    
    const auto& system = getStaffSystem();
    const auto& staves = system->getStaves();
    const auto& staff = staves.front();
    auto& track = staff->getTrack();

    const auto& chordSymbols = items<ChordSymbol>(track);
    if (chordSymbols.empty()) {
        return;
    }
    
    static constexpr const auto TempoSlotOffset = 8;
        
    const ChordSymbol& chordSymbol = chordSymbols.front();
    const auto chordSlot = chordSymbol.getSlot();
    if (chordSlot >= TempoMark::DefaultSlot) {
        TempoMark& tempoMark = tempoMarks.front();
        tempoMark.setSlot(chordSymbol.getSlot() + TempoSlotOffset);
    }
}

void
Measure::handleTempoMarkCollisions()
{
    const auto& system = getStaffSystem();
    const auto& staves = system->getStaves();
    const auto& staff = staves.front();
    auto& track = staff->getTrack();

    auto tempoMarks = items<TempoMark>(track);
    if (tempoMarks.empty()) {
        return;
    }
            
    TempoMark& tempoMark = tempoMarks.front();
    tempoMark.setSlot(TempoMark::DefaultSlot);
    const auto tempoFrame = tempoMark.getFrame();

    auto measureFrame = getTightFrame(*staff.get());
    
    for (const Item& item : mItems) {
        if (item.getItemID() != tempoMark.getItemID()) {
            const auto itemRect = item.getFrame();
            if (!itemRect.getIsEmpty()) {
                measureFrame.unionRect(itemRect);
            }
        }
    }
    
    if (!tempoFrame.intersects(measureFrame)) {
        return;
    }
            
    static constexpr const auto SlotLimit = 100;
    for (auto slot = tempoMark.getSlot(); slot < SlotLimit; slot++) {
        
        tempoMark.setSlot(slot);
        const auto tempoFrame = tempoMark.getFrame();
        
        if (!tempoFrame.intersects(measureFrame)) {
            ItemImpl::setNeedsDisplay(tempoMark);
            break;
        }
    }
}


/// \brief Find the maximum spacing across all staves
/// \result The maximum staff spacing in slots
double
Measure::maximumStaffSpacing() const
{
    double spacing = 0;
    
    // Find maximum spacing across all staves
    const auto staves = mScore.getHandCount();
    for (auto index = 0; index < staves; ++index){
        spacing = std::max(spacing, Staff::getSpacing());
    }
    
    return spacing;
}

/// \brief Set the amount occupied by a courtesy clef before a clef change.
void
Measure::setCourtesyGap(double inGap)
{
    mCourtesyGap = inGap;
    updateRightGap();
}

/// \brief Calculate the width of the gap (the area where no notes can go) on the left side of the measure
void
Measure::updateLeftGap()
{
    const auto measureIndex = getIndex();
    const auto& system = getStaffSystem();
    const auto inFirstInLine = measureIndex == system->getFirstMeasure();
    const auto& engraver = Engraver::instance();
    const auto& spacing = engraver.spacing();
    
    
    // All measures have an offset from the barline
    auto deadspace = spacing.DataSpace;
    
    // Get the staff space value from the largest staff in the score
    const auto maxStaffSpacing = maximumStaffSpacing();
    
    double keySpaces = 0;
    
    if (inFirstInLine) {
        
        // Handle case where the first measure is displaying a clef
        const auto& staves = system->getStaves();
        const auto& firstStaff = staves.front();
        firstStaff->notifyMeasureChange();

        // Find maximum spaces for key signature
        auto& track = firstStaff->getTrack();
        const auto keys = items<KeySignature>(track);
        for (const KeySignature& key : keys) {
            const auto value = key.getKey().getInt();
            keySpaces = std::max<int>(keySpaces, KeyInfoTable[value].NumSpaces);
        }
        
    } else {
        // Look for changes in key signature
        const auto keys = items<KeySignature>();
        for (const KeySignature& key : keys) {
            const auto value = key.getKey().getInt();
            const auto prevValue = key.getPreviousKey().getInt();
            const auto spaces = KeySignature::calculateSpace(prevValue,
                                                             value);
            keySpaces = std::max(keySpaces, spaces);
        }
    }
    
    if (keySpaces) {
        deadspace += keySpaces + spacing.KeySignatureSpace;
    }
    
    // Space for the meter
    int oldBeats = -1;
    int oldTimebase = -1;
    auto oldSymbol = TimeSignatureType::NUMERIC;
    
    // Get the meter value in pixels and add it later
    double meterPixels = 0.0;
    
    const auto& staves = system->getStaves();
    const auto& firstStaff = staves.front();
    const auto firstInScore = getIndex() == 0;
    if (firstInScore || inFirstInLine) {
        
        const auto& staff = firstStaff.get();
        if (mMeterMap.contains(staff)) {
            const auto& meter = mMeterMap.at(staff);
            meterPixels = meter->getWidth();
            deadspace += spacing.MeterSpace;
        }
    } else {
        const auto& previous = mScore.getMeasure(getIndex() - 1);
        oldBeats = previous->getBeats();
        oldTimebase = previous->getBeatUnit();
        oldSymbol = previous->getTimeSignatureType();
        
        const auto showMeter = getMeterVisible();
        const auto measureBeats = getBeats();
        const auto measureTimebase = getBeatUnit();
        const auto measureSymbol = getTimeSignatureType();
        
        if (showMeter && (measureBeats != oldBeats
                          || measureTimebase != oldTimebase
                          || measureSymbol != oldSymbol)) {
            
            
            const auto staff = firstStaff.get();
            if (mMeterMap.contains(staff)) {
                const auto& meter = mMeterMap.at(staff);
                meterPixels = meter->getWidth();
                deadspace += spacing.MeterSpace;
            }
        }
    }

    double clefPixels = 0;
    for (const auto& track : mScore.getTracks()) {
        const auto clefs = items<Clef>(*track.get());
        if (!clefs.empty()) {
            const Clef& clef = clefs.front();
            clefPixels = std::max(clefPixels, clef.getFrame().getWidth());
        }
    }
    
    deadspace *= maxStaffSpacing;
    const auto result = deadspace
                      + clefPixels
                      + meterPixels
                      + getLeftBarlineWidth();
    
    mLeftGap = result;
}

/// \brief Calculates the amount of the gap (the area where no notes can go)
/// on the right side of the measure
void
Measure::updateRightGap()
{
    const auto halfSpace = Staff::getSpacing() / 2.0;
    const auto defaultGap = std::max(mCourtesyGap + halfSpace, DEFAULT_RIGHT_GAP * halfSpace);
    const auto barlineWidth = getRightBarlineWidth();
    mRightGap = defaultGap + barlineWidth;
}

int
Measure::time(const Track& inTrack, double inX) const
{
    if (getClocks() <= 0.0) {
        return 0;
    }
    
    if (getWidth() <= 0.0) {
        return 0;
    }
    
    // TODO: cache the TimeMap if performance is a problem
    TimeMap tmap;
    tmap.setClocks(static_cast<int>(std::ceil(getClocks() - 0.5)));
    tmap.setWidth(getNoteSpace());
    
    // iterate through all temporals that exist in the measure to create a map
    // of time vs x. we want to prioritize the inTrack, so we will add those to
    // the map first, then we will proceed with the rest of the tracks.
    
    auto temporals = items<Temporal>(inTrack);
    for (const Temporal& temporal : temporals) {
        tmap.addPoint(temporal.getRoundedTime(),
                      temporal.getX());
    }
    
    for (const auto& track : mScore.getTracks()) {
        if (track.get() == &inTrack) {
            continue;
        }
        
        temporals = items<Temporal>(*track.get());
        for (const Temporal& temporal : temporals) {
            tmap.addPoint(temporal.getRoundedTime(),
                          temporal.getX());
        }
    }
    
    return tmap.getTimeForX(inX);
}

int
Measure::roundTimeToNearestBeat(double inTime) const
{
    const int beatsPerBar = mTimeSignatureParams.beatsPerBar;
    const auto ticksPerBeat = getTicksPerBeat();
    
    double upperBounds = (beatsPerBar - 1) * ticksPerBeat;
    
    double nearestAbove = 0.0;
    double nearestBelow = 0.0;
    
    for (int i = 0; i < beatsPerBar + 1; ++i) {
        nearestAbove += ticksPerBeat;
        
        if (nearestAbove >= static_cast<double>(inTime)) {
            break;
        }
        
        nearestBelow = nearestAbove;
    }
    
    const double distanceAbove = nearestAbove - static_cast<double>(inTime);
    const double distanceBelow = static_cast<double>(inTime) - nearestBelow;
    
    if (std::abs(distanceAbove) < std::abs(distanceBelow)) {
        
        if (nearestAbove > upperBounds) {
            return upperBounds;
        } else {
            return nearestAbove;
        }
        
    } else {
        
        return nearestBelow;
    }
    
    KOMP_THROW("Broken!");
}

int
Measure::getTicksPerBeat() const
{
    const double beatUnit = mTimeSignatureParams.beatUnit;
    
    const auto temporalType = TemporalValue::getTypeByTimeSignatureDenominator(static_cast<int>(beatUnit));
    const auto ticksPerBeat = TemporalValue::getValue(temporalType);
    
    return ticksPerBeat;
}

/// \brief Update the measure width
/// \param inNoteSpace The size of the measure notespace
void
Measure::updateNoteSpace(double inNoteSpace)
{
    KOMP_ASSERT(inNoteSpace >= 0);
    
    if (inNoteSpace == getNoteSpace()) {
        return;
    }
    
    if (inNoteSpace <= 0) {
        LOG_F(WARNING, "Fixing invalid note space: %f", inNoteSpace);
        inNoteSpace = DEFAULT_NOTE_SPACE;
    }
    
    setNoteSpace(inNoteSpace);
}

/// \brief  Return the globally determined minimum measure width.
/// \discussion This is a global constant plus the left and right gap space
/// \result The minimum width allowed for layout
double
Measure::getMinimumWidth() const
{
    // TODO: Do a real implementation
    //return desiredWidth();
    return MINIMUM_MEASURE_NOTESPACE + mRightGap + mLeftGap;
}

// MARK: - Timing

/// \brief Get the absolute start time for this measure
/// \result The start time
double
Measure::getStartTime() const
{
    double time = 0;
    
    for (const auto& measure : mScore.getMeasures()) {
        
        if (measure.get() == this) {
            return time;
        }
        time += measure->getClocks();
    }
    
    LOG_F(ERROR, "Unable to find measure start time");
    return -1;
}

/// \brief Get the absolute end time for this measure
/// \result The end time
double
Measure::getEndTime() const
{
    const double time = getStartTime() + getClocks();
    return time;
}

#define MulDiv(a,b,c)        ((((a) * (b) + ((c) / 2)) / (c)))


/// \brief Get the time for the horizontal pixel position
/// \param inX The horizonal position
/// \param inNoteSpace If true, in note space
/// \result The time
int
Measure::absoluteTime(double inX, bool inNoteSpace) const
{
    if (!inNoteSpace) {
        inX -= getLeftGap();
    }
    
    return std::max(0.0, MulDiv(inX, getClocks(), getNoteSpace()));
}

/// \brief Return the total linear time for all temporal items in the measure
/// \param inTrack The track
/// \param inHand The hand of the track
/// \param inVoice The voice index
/// \result The total linear time of temporal measure items
double
Measure::actualClocks(const Track& inTrack,
                      int inHand,
                      int inVoice) const
{
    const auto temporals = items<Temporal>(inTrack,
                                           inVoice,
                                           [&](Item& filterItem) {
        const auto& loc = filterItem.getLocation();
        return loc.getHand() == inHand;
    });
    
    double totalClocks = 0;
    for (const Temporal& temporal : temporals) {
        totalClocks = temporal.getTime();
        if (temporal.getItemTraits().getItemType() == ItemType::Rest) {
            totalClocks += temporal.getDuration().getClocks();
        }
    }
    return totalClocks;
}

// MARK: - Tempo

TempoMarkRefVec
Measure::getTempoMarks() const
{
    TempoMarkRefVec tempoMarks;
    
    const auto& tracks = mScore.getTracks();

    auto tempoMarkFilterThisMeasure = [&](Item& filterItem) {
        if (filterItem.getItemTraits().getItemType() != ItemType::TempoMark) {
            return false;
        }
        if (filterItem.getRoundedTime() != 0) {
            return false;
        }
        return true;
    };
    
    auto tempoMarkFilterPreviousMeasures = [&](Item& filterItem) {
        if (filterItem.getItemTraits().getItemType() != ItemType::TempoMark) {
            return false;
        }
        return true;
    };
    
    bool isCurrentMeasure = true;
    for (int m = getIndex(); m >= 0; --m) {
        
        for (const auto& track : tracks) {
            
            TempoMarkRefVec foundTempoMarks;
            
            if (isCurrentMeasure) {
                foundTempoMarks = items<TempoMark>(*track.get(),
                                                   tempoMarkFilterThisMeasure);
            } else {
                foundTempoMarks = items<TempoMark>(*track.get(),
                                                   tempoMarkFilterPreviousMeasures);
            }
            
            for (const auto& foundTempoMark : foundTempoMarks) {
                tempoMarks.push_back(foundTempoMark);
            }
        }
        
        if (tempoMarks.size() > 0) {
            break;
        }
        isCurrentMeasure = false;
    }
    
    // Sort the correct tempo to the front of the vector
    auto sortCompare = [&](TempoMark& lhs, TempoMark& rhs) {
        
        const auto& leftLoc = lhs.getLocation();
        const auto& rightLoc = rhs.getLocation();

        const auto lhsMeasureIndex = leftLoc.getMeasure().getIndex();
        const auto rhsMeasureIndex = rightLoc.getMeasure().getIndex();
        
        if (lhsMeasureIndex > rhsMeasureIndex) {
            return true;
        }
        
        if (lhsMeasureIndex < rhsMeasureIndex) {
            return false;
        }
        
        const auto lhsTime = lhs.getTime();
        const auto rhsTime = rhs.getTime();

        if (lhsTime > rhsTime) {
            return true;
        }
        
        if (lhsTime < rhsTime) {
            return false;
        }
        
        const auto lhsX = lhs.getX();
        const auto rhsX = rhs.getX();

        if (lhsX > rhsX) {
            return true;
        }
        
        if (lhsX < rhsX) {
            return false;
        }
        
        const auto lhsIndex = mScore.getTrackIndex(leftLoc.getTrack());
        const auto rhsIndex = mScore.getTrackIndex(rightLoc.getTrack());
        
        if (lhsIndex < rhsIndex) {
            return true;
        }
        
        if (lhsIndex > rhsIndex) {
            return false;
        }
                
        if (leftLoc.getHand() < rightLoc.getHand()) {
            return true;
        }
        
        if (leftLoc.getHand() > rightLoc.getHand()) {
            return false;
        }
        
        return &lhs < &rhs;
    };
    
    std::ranges::sort(tempoMarks, sortCompare);
    
    return tempoMarks;
}

TempoValue
Measure::getTempoValue() const
{
    const auto tempoMarks = getTempoMarks();
    if (tempoMarks.size() == 0) {
        TempoValue defaultReturn{static_cast<int>(DEFAULT_TEMPO), TemporalType::QUARTER};
        return defaultReturn;
    }
        
    const TempoMark& tempoMark = tempoMarks.front();
    return tempoMark.getTempoValue();
}

// MARK: - Key Signatures

/// \brief Inserts the key signature at the given position and then calls changeKeySignature() to adjust the accidentals.
/// \param inTrack The track
/// \param inHand The hand
/// \param inKeySignatureValue The key signature we will insert.
/// \param inTransposition The TranspositionType to be applied
void
Measure::insertKeySignature(Track& inTrack,
                            int inHand,
                            KeySignatureValue inKeySignatureValue,
                            TranspositionType inTransposition)
{
    const MusicLocation keyLocation(*this,
                                    inTrack,
                                    inHand);
    
    {
        //write_lock lock(mMutex);
        
        // Delete any existing ones at this position
        for (auto iter = mItems.cbegin(); iter < mItems.cend();) {
            
            bool erased = false;
            
            Item& item = *iter;
            const auto& loc = item.getLocation();
            
            if (item.is<KeySignature>()
                && &loc.getTrack() == &inTrack
                && loc.getHand() == inHand) {
                auto& key = item.as<KeySignature>();
                iter = removeItem(key);
                erased = true;
            }
            
            if (!erased) {
                ++iter;
            }
        }
    }
    
    auto& reg = mScore.getItemRegistry();
    auto& keySignature = reg.createItem<KeySignature>(keyLocation,
                                                      inKeySignatureValue);
    insertItem(keySignature, 0);
}

// MARK: - Meter

/// \brief Sets the meter for given measure.
/// \param inBeatsPerBar The meter's numerator
/// \param inBeatUnit The meter's denominator
void
Measure::setMeter(int inBeatsPerBar,
                  int inBeatUnit)
{
    mTimeSignatureParams.beatsPerBar = inBeatsPerBar;
    mTimeSignatureParams.beatUnit = inBeatUnit;
    const auto base = inBeatsPerBar * TIME_BASE * 4;
    mTimeSignatureParams.clocks = base / inBeatUnit;
    updateMeter();
}

void
Measure::setMeterVisible(bool inDisplay)
{
    mMeterVisible = inDisplay;
    updateLeftGap();
    updateRightGap();
    updateNoteSpace(mNoteSpace);
}

bool
Measure::getMeterVisible() const
{
    return mMeterVisible;
}

int
Measure::getBeats() const
{
    return mTimeSignatureParams.beatsPerBar;
}

int
Measure::getBeatUnit() const
{
    return mTimeSignatureParams.beatUnit;
}

const BeatGrouping&
Measure::getBeatGrouping() const
{
    return mTimeSignatureParams.beatGrouping;
}

void
Measure::setBeatGrouping(const BeatGrouping& inValue)
{
    mTimeSignatureParams.beatGrouping = inValue;
    updateMeter();
}

TimeSignatureType
Measure::getTimeSignatureType() const
{
    return mTimeSignatureParams.type;
}

void
Measure::setTimeSignatureType(TimeSignatureType inValue)
{
    mTimeSignatureParams.type = inValue;
    updateMeter();
}

const TimeSignatureParams&
Measure::getTimeSignatureParams() const
{
    return mTimeSignatureParams;
}

double
Measure::getClocks() const
{
    return mTimeSignatureParams.clocks;
}

void
Measure::updateMeter()
{
    const auto& staffSystem = getStaffSystem();
    const auto& staves = staffSystem->getStaves();
    for (const auto& staff : staves) {
        const auto staffPtr = staff.get();
        if (mMeterMap.contains(staffPtr)) {
            const auto& meter = mMeterMap.at(staffPtr);
            meter->update(staff.get(),
                          mTimeSignatureParams.type,
                          mTimeSignatureParams.beatsPerBar,
                          mTimeSignatureParams.beatUnit);
        }
    }
}

// MARK: - Barlines

BarlineType
Measure::getLeftBarline() const
{
    return mBarlineLeft;
}

void
Measure::setLeftBarline(BarlineType inValue)
{
    if (inValue != mBarlineLeft) {
        mBarlineLeft = inValue;
    }
}

double
Measure::getLeftBarlineWidth() const
{
    double width = 0.0;
    
    const auto& staffSystem = getStaffSystem();
    const auto& staves = staffSystem->getStaves();
    const auto& staff = staves.front();
    if (mLeftBarlines.contains(staff)) {
        const auto& barline = mLeftBarlines.at(staff);
        width = barline->getWidth();
    }
    
    return width;
}

BarlineType
Measure::getRightBarline() const
{
    return mBarlineRight;
}

void
Measure::setRightBarline(BarlineType inValue)
{
    if (inValue != mBarlineRight) {
        mBarlineRight = inValue;
    }
}

double
Measure::getRightBarlineWidth() const
{
    double width = 0.0;
    
    const auto& staffSystem = getStaffSystem();
    const auto& staves = staffSystem->getStaves();
    const auto& staff = staves.front();
    if (mRightBarlines.contains(staff)) {
        const auto& barline = mRightBarlines.at(staff);
        width = barline->getWidth();
    }
    
    return width;
}

// MARK: - Spacing

void
Measure::updateSpacing()
{
    read_lock lock(mMutex);
    
    updateLeftGap();
    updateRightGap();
    
    for (Item& item : mItems) {
        // TODO: Improve item invalidation here.
        item.notifyMeasureXChanged();
        item.update();
    }
}

double
Measure::getNoteSpace() const
{
    return mNoteSpace;
}

void
Measure::setNoteSpace(double inValue)
{
    read_lock lock(mMutex);
    
    if (inValue == mNoteSpace) {
        return;
    }
    
    mNoteSpace = inValue;
    
    for (Item& item : mItems) {
        // TODO: Improve item invalidation here.
        item.notifyMeasureXChanged();
        item.update();
    }
}

// MARK: - Voices

/// \brief Return the number of voices for the track hand
/// \param inTrack The track
/// \param inHand The hand
/// \result The number of voices in the track hand
int
Measure::voices(const Track& inTrack, int inHand) const
{
    int voices = 1;
    
    const auto trackIter = mTrackMap.find(&inTrack);
    if (trackIter == mTrackMap.cend()) {
        return voices;
    }
    
    const auto& handMap = mTrackMap.at(&inTrack);
    auto handIter = handMap->find(inHand);
    if (handIter != handMap->cend()) {
        const auto& voiceMap = handIter->second;
        voices = voiceMap->size();
    }
    
    return voices;
}

#pragma - Item Lists

bool
Measure::empty() const
{
    read_lock lock(mMutex);
    return mItems.empty();
}

const std::vector<ItemRef>&
Measure::allItems() const
{
    return mItems;
}

void
Measure::sort()
{
    // Sort all items"draws"
    std::ranges::sort(mItems,
                      [](Item& left, Item& right) {
        return left.getTime() < right.getTime();
    });
    
    // Sort the tracks
    for (const auto& trackMap : mTrackMap) {
        const auto& handMap = trackMap.second;
        for (auto handIter = handMap->begin(); handIter != handMap->end(); ++handIter) {
            const auto& voiceMap = *handIter->second;
            for (auto voiceIter = voiceMap.begin(); voiceIter != voiceMap.end(); ++voiceIter) {
                const auto& itemList = voiceIter->second;
                std::sort(itemList->begin(),
                          itemList->end(),
                          [](Item& left, Item& right) {
                    return left.getTime() < right.getTime();
                });
            }
        }
    }
}

// MARK: - Observation

void
Measure::registerObserver(MeasureObserver& inObserver)
{
    auto predicate = [&](MeasureObserverRef& observer) {
        return &observer.get() == &inObserver;
    };
    
    auto iter = std::find_if(mObservers.begin(),
                             mObservers.end(),
                             predicate);
    
    if (iter == mObservers.end()) {
        mObservers.emplace_back(inObserver);
    }
}

void
Measure::unregisterObserver(MeasureObserver& inObserver)
{
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        if (&observer.get() == &inObserver) {
            it = mObservers.erase(it);
            --it;
            continue;
        }
    }
}

// MARK: - Debugging

#ifdef DEBUG
#define DEBUG_OUT std::cout
#define DEBUG_END std::endl
#endif

//#ifdef DEBUG
std::string
Measure::debugPrintTimes() const
{
    std::stringstream ss;
    const std::string separator{"--------------------"};
    std::stringstream ess;
    ess << std::endl;
    const std::string e{ess.str()};
    const std::string tab{'\t'};
    
    for (auto& track : mScore.getTracks()) {
        
        const auto trackIndex = mScore.getTrackIndex(*track.get());
        ss << "track " << trackIndex << separator << e;
        
        const int numHands = track->getHands();
        for (int hand = 0; hand < numHands; ++hand) {
            ss << "hand " << hand << separator << e;
            const int numVoices = MAX_VOICES;
            for (int voice = 0; voice < numVoices; ++voice) {
                ss << "voice " << voice << separator << e;
                
                auto filter = [&](Item& filterItem) {
                    const auto& loc = filterItem.getLocation();
                    return &loc.getTrack() == track.get()
                    && loc.getHand() == hand
                    && loc.getVoice() == voice;
                };
                const auto temporals = items<Temporal>(*track.get(),
                                                       filter);
                for (Temporal& temporal : temporals) {
                    if (temporal.getItemTraits().getItemType() == ItemType::Rest) {
                        ss << "rest, time="
                        << temporal.getRoundedTime()
                        << ", duration="
                        << temporal.getDuration().getRoundedClocks()
                        << e;
                        
                    } else if (temporal.getItemTraits().getItemType() == ItemType::Note) {
                        const Note& note = temporal.as<Note>();
                        if (note.getIsChordMember()) {
                            continue;
                        }
                        ss << "note, time=" << temporal.getRoundedTime() << ", duration=" << temporal.getDuration().getRoundedClocks();
                        ss << ", pitch=" << note.getPitch().getString() << e;
                        
                    } else if (temporal.getItemTraits().getItemType() == ItemType::Chord) {
                        const Chord& chord = temporal.as<Chord>();
                        ss << "chord, time=" << temporal.getRoundedTime() << ", duration=" << temporal.getDuration().getRoundedClocks() << e;
                        for (const Note& note : chord.getNotes()) {
                            ss << tab << "chordednote, time=" << temporal.getRoundedTime() << ", duration=" << temporal.getDuration().getRoundedClocks();
                            ss << ", pitch=" << note.getPitch().getString() << e;
                        }
                    }
                }
            }
        }
    }
    std::cout << ss.str() << std::endl;
    return ss.str();
}

/// \brief Output measure information to the console
void
Measure::printToStream() const
{
    LOG_F(ERROR, "-----------------------------------------------------------------------------------");
    std::stringstream ss;
    const auto& items = allItems();
    ss << "measure index: " << getIndex() << ", ";
    ss << "item count: " << items.size();
    LOG_F(ERROR, "%s", ss.str().c_str());
    ss.str("");
    int i = 0;
    for (const Item& item : mItems) {
        const auto& loc = item.getLocation();
        ss << "index: " << i << ", ";
        ss << "track: " << mScore.getTrackIndex(loc.getTrack()) << ", ";
        ss << "hand: " << loc.getHand() << ", ";
        ss << "voice: " << loc.getVoice() << ", ";
        ss << "time: " << item.getTime() << ", ";
        ss << "x-position: " << item.getX() << ", ";
        ss << item.getItemTraits().getName();
        LOG_F(ERROR, "%s", ss.str().c_str());
        ss.str("");
        ++i;
    }
    LOG_F(ERROR, "-----------------------------------------------------------------------------------");
}

int
Measure::getIndex() const
{
    return mScore.getMeasureIndex(*this);
}

std::vector<MeasureUPtr>::const_iterator
Measure::iter() const
{
    const auto iter = std::find_if(mScore.getMeasuresBegin(),
                                   mScore.getMeasuresEnd(),
                                   [&](const MeasureUPtr& measure) {
        return measure.get() == this;
    });
    return iter;
}

const StaffSystemUPtr&
Measure::getStaffSystem() const
{
    const auto measureIndex = getIndex();
    size_t lines = 0;
    const auto& staffSystems = mScore.getStaffSystems();
    for (const auto& line : staffSystems) {
        const auto lineMeasure = line->getFirstMeasure();
        const auto lastMeasure =  lineMeasure + line->getMeasureCount();
        if (measureIndex >= lineMeasure && measureIndex < lastMeasure) {
            return line;
        }
        ++lines;
    }
    return staffSystems.at(std::max(size_t(0), lines - 1));
}

TemporalRefVec
Measure::getTemporals() const
{
    TemporalRefVec temporals;
    
    auto filter = [&](Item& filterItem) {
        return true;
    };
    
    for (const auto& track : mScore.getTracks()) {
        auto temp = items<Temporal>(*track.get(), filter);
        temporals.insert(temporals.cend(), temp.cbegin(), temp.cend());
    }
    
    return temporals;
}

TemporalRefVec
Measure::getTemporals(const Track& inTrack,
                      int inHand,
                      int inVoice) const
{
    auto filter = [&](Item& filterItem) {
        
        const auto& loc = filterItem.getLocation();
        
        if (loc.getHand() != inHand) {
            return false;
        }
        
        if (loc.getVoice() != inVoice) {
            return false;
        }
        return true;
    };
    
    return items<Temporal>(inTrack, filter);
}

} // namespace

//static int
//GetKeySteps(int inOld, int inNew)
//{
//    // Figure out the scale steps between two keys by comparing the scale
//    // step of the tonic of each key in its own key.
//    const auto delta = komp::KeyInfoTable[inNew].ScalePos[komp::KeyInfoTable[inNew].DeltaPitch] -
//    komp::KeyInfoTable[inOld].ScalePos[komp::KeyInfoTable[inOld].DeltaPitch];
//
//    if (delta < 0) {
//        return -delta;
//    } else {
//        return delta;
//    }
//}
//
//static int
//TransposeKeySignature(int inOldKeySignature, int inAmount, int inKeySteps)
//{
//    auto newPitch = komp::KeyInfoTable[inOldKeySignature].DeltaPitch + inAmount;
//    while (newPitch < 0) {
//        newPitch += 12;
//    }
//    newPitch %= 12;
//
//    // Look for the new key signature
//    int newKey = -1;
//    for (auto I = 0; I < 15; I++) {
//        if (komp::KeyInfoTable[I].DeltaPitch == newPitch) {
//            // Duplicated keys are Db, Gb, Cb, C#, F#, B. Respect the step count
//            // of the original transposition when deciding which to use.
//            if ((newKey == -1) || inKeySteps == GetKeySteps(inOldKeySignature, I)) {
//                newKey = I;
//            }
//        }
//    }
//
//    return newKey;
//}

//static int sKeyYPositions[] = { 0, 3, 6, 2, 5, 1, 4, 0, 4, 1, 5, 2, 6, 3, 0 };
//
//static int
//GetSlotOffsetForKey(int NewKey, int inOldKeySignature, int PitchOffset)
//{
//    auto deltaY = sKeyYPositions[NewKey] - sKeyYPositions[inOldKeySignature];
//    if (deltaY) {
//        if (PitchOffset < 0) {
//            if (deltaY > 0) {
//                deltaY -= 7;
//            }
//        } else {
//            if (deltaY < 0) {
//                deltaY += 7;
//            }
//        }
//    } else {
//        // Same key scale degrees, like C and C flat; handles those cases
//        if (PitchOffset < -2) {
//            deltaY -= 7;
//        } else if (PitchOffset > 2) {
//            deltaY += 7;
//        }
//    }
//
//    return deltaY;
//}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

