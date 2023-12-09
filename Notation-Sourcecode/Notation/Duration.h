// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "TemporalType.h"
#include "TupletRatio.h"

namespace komp
{
    enum class SustainType
    {
        Full,        ///< The getSustain will equal getClocks
        Percentage,  ///< getSustain will apply a percentage to getClocks
        Fixed        ///< getSustain will ignore getClocks and return a fixed amount
    };
    
    /// calculates the clocks for a given TemporalType,
    /// number of dots, and tuplet ratio(s)
    ///
    class Duration
    {
    public:
        Duration();
        Duration(TemporalType inTemporalType);
        
        /// get the duration in clock time
        double getClocks() const;
        
        /// get the duration in clock time rounded to integer
        int getRoundedClocks() const;
        
        /// gets the temporal type
        TemporalType getTemporalType() const;
        
        /// sets the temporal type
        const Duration& setTemporalType(TemporalType inValue);
        
        /// gets the number of augmentation dots
        int getDots() const;
        
        /// sets the number of augmentation dots. will be range
        /// bound between zero and MAX_DOTS as found in Constants.h
        const Duration& setDots(int inValue);
        
        /// will attempt to set the TemporalType, dots and tuplet
        /// characteristics from the incoming clocks value. caution
        /// this is messy guesswork
        const Duration& parseClocks(double inClocks);
        
        /// returns the sustain duration depending on the SustainType
        double getSustainClocks() const;

        /// returns the sustain duration depending on the SustainType
        /// rounded to the nearest integer
        int getRoundedSustainClocks() const;
        
        /// gets the method by which sustain clocks are determined.
        SustainType getSustainType() const;
        
        /// sets the method by which sustain clocks are determined.
        const Duration& setSustainType(SustainType inValue);
        
        /// gets the sustain perentage - caution, the sustain percentage
        /// will only be applied to getSustainClocks() when the SustainType
        /// is set to Percentage
        double getSustainPercentage() const;
        
        /// sets the sustain perentage - caution, the sustain percentage
        /// will only be applied to getSustainClocks() when the SustainType
        /// is set to Percentage
        const Duration& setSustainPercentage(double inValue);
        
        /// gets the fixed sustain value - caution, the fixed sustain will
        /// only be returned by getSustainClocks() when the SustainType
        /// is set to Fixed
        double getSustainFixedClocks() const;

        /// sets the fixed sustain value - caution, the fixed sustain will
        /// only be returned by getSustainClocks() when the SustainType
        /// is set to Fixed
        const Duration& setSustainFixedClocks(double inValue);
        
        /// gets the tuplet ratios
        const std::vector<TupletRatio>& getTupletRatios() const;

        /// adds a tuplet ratio which will modify the duration
        void addTupletRatio(TupletRatio inTupletRatio);

        /// removes the first TupletRatio which matches the inTupletRatio
        /// returns true if inTupletRatio was found and removed
        bool removeTupletRatio(const TupletRatio& inTupletRatio);

        /// removes any tuplet ratio modifications
        void clearTupletRatios();
        
        /// get the duration clocks without applying any tuplet ratios
        double getUntupletedClocks() const;
        
        /// returns true if the state matches exactly with other
        bool equals(const Duration& other) const;
        
    private:
        TemporalType mTemporalType;
        int mDots;
        SustainType mSustainType;
        double mSustainPercentage;              ///< used with SustainType Percentage - 1.0 is 100%, 0.5 is 50%, etc
        double mFixedSustainClocks;             ///< used with SustainType Fixed
        std::vector<TupletRatio> mTupletRatios; ///< if size is greater than one, then tuplets are nested

    private:
        double calculateClocks() const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
