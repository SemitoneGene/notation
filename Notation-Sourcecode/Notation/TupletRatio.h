// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DurationCount.h"

namespace komp
{
    /// A tuplet ratio represents a tuplet by encapsulating two
    /// DurationCount objects.  For example, a normal eighth note
    /// triplet will have a DurationCount 3-Eighths and a Duration
    /// Count 2-Eighths.  We borrow terminology from MusicXML and
    /// name the 3-Eighths value as "ActualNotes" and the 2-Eighths
    /// value as "NormalNotes".  This can be thought of as "There
    /// are *Actually* 3-Eighth notes in a space where there would
    /// *Normally* be 2-Eighth notes.
    ///
    class TupletRatio
    {
    public:
        /// TupletRatio is an immutable object, so you must use this nont-
        /// default constructor to give it the values you want. This default
        /// constructor is provided for convenience when your values are not
        /// yet known. you must then replace the default-constructed Tuplet-
        /// Ration with one that has been constructed with the other ctor.
        TupletRatio();

        /// TupletRatio is an immutable object, so you must use this constructor
        /// to give it the values you want
        TupletRatio(DurationCount inActualNotes, DurationCount inNormalNotes);
        
        DurationCount getActual() const;
        DurationCount getNormal() const;
        
        /// the modifier is the value by which a duration's clocks should be
        /// multiplied by when it is affected by a tuplet.  For example, with
        /// an eighth note triplet, the modifier will be 2/3 or approx 0.66667.
        /// Thus any note that is affected by the tuplet ratio should have its
        /// normal clocks value multiplied by 0.6666667
        ///
        double getModifier() const;
        int getRoundedModifier() const;

        /// returns true if the tuplet ratio is one typically encountered in
        /// musical literature without necessitating a ratio drawn with a colon.
        /// for example 3-eighths in the space of 2-eighths is typical in music
        /// literature and is written as '3', whereas 3-eighths in the space of
        /// five eighths would be bizarre and would need to be written as '5:3'
        ///
        bool getIsNormalRatio() const;
        
        /// returns true if this TupletRation and the other TupletRatio have
        /// exactly the same values
        bool equals(const TupletRatio& other) const;
        
    private:
        DurationCount mActual; ///< e.g. in a triplet this would be 3-Eighths
        DurationCount mNormal; ///< e.g. in a triplet this would be 2-Eighths
    };

    inline bool operator==(const TupletRatio& inLeft, const TupletRatio& inRight)
    {
        if (inLeft.getActual() != inRight.getActual()) {
            return false;
        }

        if (inLeft.getNormal() != inRight.getNormal()) {
            return false;
        }

        return true;
    }

    inline bool operator!=(const TupletRatio& inLeft, const TupletRatio& inRight)
    {
        return !(inLeft == inRight);
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
