// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <unordered_set>
#include <vector>

// Local frameworks
#include <parallel_hashmap/phmap.h>

// Local
#include "BarlineType.h"
#include "Constants.h"
#include "DrawableBarline.h"
#include "DrawableKeySignature.h"
#include "DrawableRectangle.h"
#include "DrawableTimeSignature.h"
#include "Forward.h"
#include "Item.h"
#include "ItemSort.h"
#include "Iterators.h"
#include "KeySignatureType.h"
#include "LockTypes.h"
#include "Geometry.h"
#include "TempoValue.h"
#include "Throw.h"
#include "TimeSignatureParams.h"


// Forward declarations
namespace komp
{
    // Item identification lambda function
    using ItemFilter = std::function<bool(Item&)>;
}

namespace komp
{
    enum class BarlineLocation
    {
        NONE,
        LEFT,
        RIGHT,
        BOTH
    };

    enum class MeasureNumberType
    {
        NONE = 0,
        EVERY,
        SYSTEM
    };

    enum class MeasureNumberLocation
    {
        BELOW = 0,
        ABOVE
    };

    class Measure
    {
        
    public:
        Measure(Score& inScore);
        
        Measure() = delete;
        Measure(const Measure& inMeasure) = delete; // copying measures is a very bad idea
        Measure& operator=(const Measure& inMeasure) = delete; // copying measures is a very bad idea
        Measure(Measure&& inMeasure) = delete;
        Measure& operator=(Measure&& inMeasure) = delete;
        
        ~Measure();
        
        Rect getFrame(const Staff& inStaff) const;
        Rect getTightFrame(const Staff& inStaff) const;
        Rect getInterstaffUpperFrame(const Staff& inStaff) const;
        Rect getInterstaffLowerFrame(const Staff& inStaff) const;
        
        void inserted();
        void removed();
        
        /// When an Item is Quiescent it will refrain from sending or responding
        /// to ItemObserver notifications. A measure is Quiescent when all of
        /// its items are Quiescent. All Items must be polled to determine.
        bool getIsQuiescent() const;
        
        /// When an Item is Quiescent it will refrain from sending or responding
        /// to ItemObserver notifications.  A measure is Quiescent when all of
        /// its items are Quiescent.
        void setIsQuiescent(bool inValue);
        
        void draw();
        void draw(const DrawableContext& inContext, Staff& inStaff);
        void drawBarlines(const DrawableContext& inContext, Staff& inStaff);
        
        /// Use default strategy and do update positions
        void layout();
        
        void handleVerticalCollisions();
        
        // Gap handling
        void updateSpacing();
        
        double getGap() const { return mLeftGap + mRightGap; }
        double getLeftGap() const { return mLeftGap; }
        double getRightGap() const { return mRightGap; }
        
        void setCourtesyGap(double inGap);
        
        /// \discussion calculates the exact time that corresponds to the exact x position
        /// (where x is in  pixel distance from dataX). prioritizes the
        /// inTrack's temporals to determine this, but also considers all temporals
        /// in the measure
        int time(const Track& inTrack, double inX) const;
        
        /// For the given inTime returns the closest time which is quantized to the beat
        int roundTimeToNearestBeat(double inTime) const;
        
        int getTicksPerBeat() const;
        
        double positionForTime(int inClock) const;
        
        void updateNoteSpace(double inNoteSpace);
        double getWidth() const;
        double getMinimumWidth() const;
        
        /// \brief Insert the given item at the given time
        void insertItem(Item&, double inTime);
        
        /// Insert the given item at inItem->getTime()
        void insertItem(Item& inItem);
        
        void insertClef(ClefType inClef,
                        Track& inTrack,
                        int inHand,
                        double inTime);
        
        void insertKeySignature(Track& inTrack,
                                int inHand,
                                KeySignatureValue inKeySignatureValue,
                                TranspositionType inTransposition = TranspositionType::NONE);
        
        void offsetItemTimes(Temporal& inItem, double inAmount);
        
        bool contains(const Item& inItem);
        
        ItemRefVec::const_iterator removeItem(Item& inItem,
                                              bool inDoUpdateTimes = true);
        
        void setMeter(int inBeatsPerBar,
                      int inBeatUnit);
        
        void addMeter(Staff& inStaff);
        
        int absoluteTime(double inX, bool inNoteSpace) const;
        
        double getClocks() const;
        double actualClocks(const Track& inTrack,
                            int inHand,
                            int inVoice) const;
        
        double getX() const { return mX; }
        void setX(double inX);
        
        /// the x-position at which the measure's contents
        /// begin, in score pixels
        double getDataX() const { return mX + mLeftGap; }
        
        double getRight() const { return mX + getWidth(); }
        
        /// Scans the current measure and previous measures (if necessary)
        /// to deduce the current measure's starting tempo.  If no TempoMark
        /// can be found, a default tempo is returned (Q=120)
        TempoValue getTempoValue() const;
        
        double getStartTime() const;
        double getEndTime() const;
        
        int getIndex() const;
        std::vector<MeasureUPtr>::const_iterator iter() const;
        
        const StaffSystemUPtr& getStaffSystem() const;
        
        int getTuplet(double inX, const Track& inTrack, int inVoice);
                
        BarlineType getLeftBarline() const;
        void setLeftBarline(BarlineType inValue);
        double getLeftBarlineWidth() const;

        BarlineType getRightBarline() const;
        void setRightBarline(BarlineType inValue);
        double getRightBarlineWidth() const;

        void setBarlineSelection(BarlineLocation inLocation);
        BarlineLocation getBarlineSelection() const { return mBarlineSelection; }
        
        double getNoteSpace() const;
        void setNoteSpace(double inValue);
        
        Score& getScore() const { return mScore; }
        
        bool empty() const;
        
        const ItemRefVec& allItems() const;
        
        /// Print the temporals in each track, hand voice
        std::string debugPrintTimes() const;
        
        /// Returns all temporals from the measure sorted by track, hand, voice, time
        TemporalRefVec getTemporals() const;
        
        /// Returns all temporals from the measure track, hand, voice
        TemporalRefVec getTemporals(const Track& inTrack,
                                    int inHand,
                                    int inVoice) const;
        
        /// \brief Return the items of the template type
        /// \result A vector of items found
        template<typename T>
        std::vector<std::reference_wrapper<T>> items() const
        {
            read_lock lock(mMutex);
            std::vector<std::reference_wrapper<T>> outItems;
            for (Item& item : mItems) {
                if (item.is<T>()) {
                    auto& typedItem = item.as<T>();
                    outItems.emplace_back(typedItem);
                }
            }
            return outItems;
        }
        
        /// \brief Return the items for the track
        /// \param inTrack The track to get the items for
        /// \result A vector of items found
        template<typename T>
        std::vector<std::reference_wrapper<T>> items(const Track& inTrack) const
        {
            const auto filter = [&](Item& inItem) {
                return inItem.is<T>();
            };
            return items<T>(inTrack, -1, filter);
        }
        
        /// \brief Return the items for the track sorted by time
        /// \param inTrack The track to get the items for
        /// \result A vector of items found
        template<typename T>
        std::vector<std::reference_wrapper<T>> items(const Track& inTrack,
                                                     const ItemFilter& inFilter) const
        {
            return items<T>(inTrack, -1, inFilter);
        }
        
        /// \brief Return the items for the track sorted by time
        /// \param inTrack The track to get the items for
        /// \param inVoice The voice the item must be in. Use -1 for all voices.
        /// \result A vector of items found
        template<typename T>
        std::vector<std::reference_wrapper<T>> items(const Track& inTrack,
                                                     int inVoice) const
        {
            const auto filter = [&](Item& inItem) {
                return inItem.is<T>();
            };
            return items<T>(inTrack, inVoice, filter);
        }
        
        /// \brief Return the items for the track sorted by time
        /// \param inTrack The track to get the items for
        /// \param inVoice The voice the item must be in. Use -1 for all voices.
        /// \param inFilter The item filter or nullptr
        /// \result A vector of items found
        template<typename T>
        std::vector<std::reference_wrapper<T>> items(const Track& inTrack,
                                                     int inVoice,
                                                     const ItemFilter& inFilter) const
        {
            std::vector<std::reference_wrapper<T>> outItems;
            
            // Check for the track items
            auto trackIter = mTrackMap.find(&inTrack);
            if (trackIter == mTrackMap.cend()) {
                return outItems;
            }
            
            // Check all the hands for the track
            const auto& handMap = mTrackMap.at(&inTrack);
            auto handIter = handMap->cbegin();
            for ( ; handIter != handMap->cend(); ++handIter) {
                if (inVoice == -1) {
                    // Iterate all the voices for the hand
                    const auto& voiceMap = handIter->second;
                    auto voiceIter = voiceMap->cbegin();
                    for ( ; voiceIter != voiceMap->cend(); ++voiceIter) {
                        const auto& voiceItems = voiceIter->second;
                        for (Item& item : *voiceItems) {
                            if (item.is<T>()) {
                                const auto pushBack = inFilter ? inFilter(item) : true;
                                if (pushBack) {
                                    outItems.push_back(item.as<T>());
                                }
                            }
                        }
                    }
                } else {
                    // Check only the requested voice
                    const auto& voiceMap = handIter->second;
                    const VoiceMap::const_iterator voiceIter = voiceMap->find(inVoice);
                    if (voiceIter != voiceMap->cend()) {
                        const auto& voiceItems = voiceIter->second;
                        for (Item& voiceItem : *voiceItems) {
                            if (voiceItem.is<T>()) {
                                const auto pushBack = inFilter ? inFilter(voiceItem) : true;
                                if (pushBack) {
                                    outItems.push_back(voiceItem.as<T>());
                                }
                            }
                        }
                    }
                }
            }
            
            komp::sortItems(outItems);
            
            return outItems;
        }
        
        /// \brief Return the first item found that matches the conditions in the filter
        /// \param inTrack The track to get the items for
        /// \param inVoice The voice the item must be in.
        /// \result The item or nullptr if none found
        template<typename T>
        T* firstMatch(const Track& inTrack, int inVoice) const
        {
            return firstMatch<T>(inTrack,
                                 inVoice,
                                 [&](Item& filterItem) {
                return true;
            });
        }
        
        /// \brief Return the first item found that matches the conditions in the filter
        /// \param inTrack The track to get the items for
        /// \param inVoice The voice the item must be in.
        /// \param inFilter The item filter. Must not be null
        /// \result The found item or nullptr if none was found
        template<typename T>
        T* firstMatch(const Track& inTrack,
                      int inVoice,
                      const ItemFilter& inFilter) const
        {
            assert(inFilter);
            
            // Check for the track items
            const auto trackIter = mTrackMap.find(&inTrack);
            if (trackIter == mTrackMap.cend()) {
                // I've disabled this warning because I think it is unimporant.
                // I checked the code and any measure that has never had an item
                // inserted into it will hit this branch, which seems fine. - Matt
                //LOG_F(WARNING, "Track not found in track map");
                return nullptr;
            }
            
            if (inVoice == -1 || inVoice > MAX_VOICES) {
                LOG_F(ERROR, "Voice is out of bounds: %d", inVoice);
                return nullptr;
            }
            
            // Check all the hands for the track
            const auto& handMap = mTrackMap.at(&inTrack);
            auto handIter = handMap->cbegin();
            for ( ; handIter != handMap->cend(); ++handIter) {
                const auto& voiceMap = handIter->second;
                const VoiceMap::const_iterator voiceIter = voiceMap->find(inVoice);
                if (voiceIter != voiceMap->cend()) {
                    const auto& voiceItems = voiceIter->second;
                    for (Item& voiceItem : *voiceItems) {
                        if (voiceItem.is<T>() && inFilter(voiceItem)) {
                            auto& result = voiceItem.as<T>();
                            return &result;
                        }
                    }
                }
            }
            return nullptr;
        }
        
        int voices(const Track& inTrack, int inHand) const;
        
        void setMeterVisible(bool inDisplay);
        bool getMeterVisible() const;
        
        int getBeats() const;
        int getBeatUnit() const;
        
        const BeatGrouping& getBeatGrouping() const;
        void setBeatGrouping(const BeatGrouping& inValue);
        
        TimeSignatureType getTimeSignatureType() const;
        void setTimeSignatureType(TimeSignatureType inValue);
        
        const TimeSignatureParams& getTimeSignatureParams() const;
                
        void sort();
        
        void removeStaff(const Staff& inStaff);
        void removeTrack(const Track& inTrack);
        
        void registerObserver(MeasureObserver& inObserver);
        void unregisterObserver(MeasureObserver& inObserver);

        void printToStream() const;
                
    private:
        
        void handleChordSymbolCollisions();
        void handleTempoMarkCollisions();
        
        void updateLeftGap();
        void updateRightGap();
        
        void configureBarlines(Staff& inStaff);
        
        double maximumStaffSpacing() const;
        
        void layout(NoteSpacerIntf& inStrategy);
        
        void updateMeter();
        
        void removeItemInternal(Item& inItem);
        
        TempoMarkRefVec getTempoMarks() const;
        
        double mX{0.0};             // Start position
        bool mMeterVisible;
        
        BarlineType mBarlineLeft;
        BarlineType mBarlineRight;
        BarlineLocation mBarlineSelection;
        using BarlineMap = phmap::node_hash_map<Staff*, DrawableBarlineUPtr>;
        BarlineMap mLeftBarlines;
        BarlineMap mRightBarlines;
        
        double mLeftGap{0.0};       // Unoccupied area on left
        double mNoteSpace;          // mLeftPad + mNoteSpace + mRightPad = Width
        double mRightGap;           // Unoccupied area on right
        double mCourtesyGap{0.0};   // Courtesy clef area

        using FrameMap = phmap::node_hash_map<const Staff*, std::unique_ptr<DrawableRectangle>>;
        FrameMap mLeftGapMap;
        FrameMap mNoteSpaceMap;
        FrameMap mRightGapMap;
        
        Score& mScore;
        TimeSignatureParams mTimeSignatureParams;
        
        MeasureObserverRefVec mObservers;

        mutable mutex_type mMutex;
        
        // Items are stored in the measure and then categorized by their attributes
        // Track, Voice, Hand
        using ItemList = ItemRefVec;
        using ItemListPtr = std::unique_ptr<ItemList>;
        ItemList mItems;
        
        using VoiceMap = phmap::node_hash_map<int, ItemListPtr>;
        using VoiceMapPtr = std::unique_ptr<VoiceMap>;
        
        using HandMap = phmap::node_hash_map<int, VoiceMapPtr>;
        using HandMapPtr = std::unique_ptr<HandMap>;
        
        using TrackMap = phmap::node_hash_map<Track*, HandMapPtr>;
        TrackMap mTrackMap;
        
        using MeterMap = phmap::node_hash_map<Staff*, std::unique_ptr<DrawableTimeSignature>>;
        MeterMap mMeterMap;
        
        using KeyMap = phmap::node_hash_map<Staff*, std::unique_ptr<DrawableKeySignature>>;
        KeyMap mKeyMap;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

