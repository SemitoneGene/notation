// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Chord.h"
#include "Constants.h"
#include "DrawableStaffLines.h"
#include "DrawableString.h"
#include "Forward.h"
#include "Measure.h"
#include "Note.h"
#include "Geometry.h"
#include "StaffType.h"
#include "Track.h"

namespace komp
{
    enum class LocateChords : bool
    {
        Yes,
        No
    };

    class Staff
    {
    public:
        Staff(const Score& inScore,
              Track& inTrack,
              StaffSystem* inStaffSystem,
              StaffType inType,
              int inLineCount,
              int inHand);
        
        Staff() = delete;
        Staff(const Staff& inStaff) = delete;
        Staff& operator=(const Staff& inStaff) = delete;
        Staff(Staff&& inStaff) = delete;
        Staff& operator=(Staff&& inStaff) = delete;

        ~Staff() = default;

        /// When the object which holds this staff (i.e. the StaffSystem object)
        /// Changes it's range of measures (i.e. when the StaffSystem's firstMeasure
        /// value changes) it needs to notify it's staves
        void notifyMeasureChange();
        
        void draw(const DrawableContext& inContext);
        void drawStaffName(const DrawableContext& inContext,
                           const DrawParams& inDrawParams);

        static double getFontSize();

        double baseY() const  { return mBaseY; }

        /// The position, in pixels, of the top staff line, relative to
        /// StaffSystem::y()
        double getY() const;
        void setY(double inValue);

        /// the position, in score pixels, of the top staff line
        double getScoreY() const;

        /// the position, in pixels, or the bottom staff line, relative
        /// to StaffSystem::y()
        double bottom() const { return mY + mHeight; }

        /// the position, in score pixels, of the bottom staff line
        double getScoreBottom() const;

        /// the position, in score pixels, of slot 6, which is the middle line
        double getScoreMiddle() const;
        
        char getLyricPosition(int inVoice) const;
        void setLyricPosition(int inVoice, int inValue);

        int	getLyricYPos(int inVoice) const;

        double getHeight() const { return mHeight; }
        
        /// get the size of a staff space in score pixels
        static double getSpacing();
        
        /// The size of a slot in score pixels (2 slots pers staff space)
        static double getSlotSize();

        /// returns the number of lines in the staff. for example a normal treble-
        /// clef staff would return 5. a one line percussion staff would return 1.
        int getStaffLineCount() const;

        /// get the slot number for the top-most line of the staff
        int getTopLineSlot() const;

        /// get the slot number for the bottom-most line of the staff
        int getBottomLineSlot() const;

        /// for a note on the provided slot, returns true if the slot is above
        /// the staff and, if it were a note, would require leger lines
        bool getIsLegerAbove(int inSlot) const;

        /// for a note on the provided slot, returns true if the slot is below
        /// the staff and, if it were a note, would require leger lines
        bool getIsLegerBelow(int inSlot) const;

        /// the slot at which things that should be horizontally aligned (i.e.
        /// dynamics) should be baselined below the staff
        int getBaselineSlotBelow() const;

        /// the slot at which things that should be horizontally aligned (i.e.
        /// dynamics) should be baselined above the staff
        int getBaselineSlotAbove() const;

        StaffType type() const { return mStaffType; }

        int lineCount() const { return mLineCount; }

        int getHand() const { return mHand; }
        void setHand(int inValue) { mHand = inValue; }
        
        Track& getTrack() const { return mTrack; }

        int getTrackIndex() const;

        StaffSystem* getStaffSystem() const { return mStaffSystem; }
        void setStaffSystem(StaffSystem* inValue) { mStaffSystem = inValue; }
        
        int getIndex() const;
        
        double pixelToSlot(double inPixel) const;

        double slotToPixel(int inSlot) const;
        double slotToPixel(double inSlot) const;
        
        bool itemsAreDraggable() const;
        
        double getBracketWidth() const;
        
        static komp::Rect getSlotFrame();

        void updateStaffName();
        
        /// \brief Locate an item at or near the point in the staff
        /// \param inMeasure The measure to look in
        /// \param inHand The hand to check against
        /// \param inPoint The point to locate with
        /// \param inGrowExtents The extents to expand the search area by
        /// \param inLocateChords Locate chords
        /// \result The item found or nullptr if none
        template<typename T>
        T* locateItem(Measure& inMeasure,
                      int inHand,
                      const Point& inPoint,
                      const Extents& inGrowExtents = EXTENTS_ZERO,
                      LocateChords inLocateChords = LocateChords::No) const
        {
            if (inMeasure.empty()) {
                // No items in the measure. Nothing left to do here.
                return nullptr;
            }
            
            // Get the track items of the requested type
            auto& track = getTrack();
            
            auto filter = [&](Item& filterItem) {
                
                const auto& loc = filterItem.getLocation();
                
                if (loc.getHand() != inHand) {
                    return false;
                }
                
                if (&loc.getTrack() != &track) {
                    return false;
                }

                // TODO: Should be sensitive to the current voice context
                if (loc.getVoice() != 0) {
                    return false;
                }

                switch (inLocateChords)
                {
                    case LocateChords::Yes:
                    {
                        if (filterItem.getItemTraits().getItemType() == ItemType::Note) {
                            auto& note = filterItem.as<Note>();
                            if (note.getIsChordMember()) {
                                return false;
                            }
                        }
                        break;
                    }
                        
                    case LocateChords::No:
                    {
                        if (filterItem.getItemTraits().getItemType() == ItemType::Chord) {
                            return false;
                        }
                    }
                }
                
                return true;
            };
            

            // Get the filtered items and sort them
            auto items = inMeasure.items<T>(track, filter);
            komp::sortItems(items);
            
            // Do an intial search without applying the grow extents
            for (T& item : items) {
                if (item.contains(inPoint, true)) {
                    return &item;
                }
            }
            
            if (inGrowExtents != EXTENTS_ZERO) {
                // Do a broader search using the search extents
                for (T& item : items) {
                    
                    auto itemRect = item.getFrame();
                    
                    // Expand the search area
                    itemRect.left -= inGrowExtents.left;
                    itemRect.top -= inGrowExtents.top;
                    itemRect.right += inGrowExtents.right;
                    itemRect.bottom += inGrowExtents.bottom;
                    
                    // See if the area contains the point
                    if (itemRect.contains(inPoint)) {
                        return &item;
                    }
                }
            }
            
            // Unable to find an item
            return nullptr;
        }
         
    private:
        bool shouldShowStaffName() const;
        
        const Score& mScore;
        StaffType mStaffType;		            // The staff type
        int mLineCount;                         // The number of lines in the staff
        int mHand;                              // The hand it belongs to
        double mSpacing;		                // Number of pixels in one staff line space
        double mSlotSize;		                // Number of pixels in a slot (2 slots per staff space)
        double mY{0.0};                         // Distance in pixels from mStaffSystem->y() to the uppermost line in this staff
        double mHeight{0.0};                    // Height of staff: mY + (mSpacing * number of staff spaces)
        double mBaseY{0.0};			            // Temporal positions are based here
        int mLyricPositions[MAX_VOICES];        // Scale position of lyrics
        Track& mTrack;                          // Parent track
        StaffSystem* mStaffSystem;              // Parent system
        
        bool mUpdateLines;
        
        mutable DrawableString mName;
        mutable DrawableStaffLines mLines;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
