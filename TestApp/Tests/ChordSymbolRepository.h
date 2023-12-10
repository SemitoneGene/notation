// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "ChordSymbolValue.h"

namespace komp
{

    using ChrdSymValues = std::vector<ChordSymbolValue>;

    /// \brief  A bunch of Chord Symbol Values for testing purposes.
    ///
    /// \discussion A bunch of sane and randomly insane ChordSymbolValue
    /// instances to be used in testing.
    ///
	class ChordSymbolRepository
	{
	public:
        
    public:
    	static const ChrdSymValues& getHandpicked();
        static const ChrdSymValues& getLogicallyGenerated();
        static const ChrdSymValues& getRandomlyGenerated();
        	
    private:
        ChrdSymValues mHandGenerated;
        ChrdSymValues mLogicallyGenerated;
        ChrdSymValues mRandomlyGenerated;
        std::vector<ChordSymbolCharValue> mCharValues;
        std::vector<ChordSymbolCharValue> mRoots;
        std::vector<ChordSymbolCharValue> mQualities;
        std::vector<ChordSymbolCharValue> mAccidentals;
        std::vector<ChordSymbolCharValue> mNumbers;
        std::vector<ChordSymbolCharValue> mParensAndSlash;
        std::vector<bool> mRandomBools;
        size_t mRandomBoolsPosition;
        std::vector<int> mRandBetween0and34;
        size_t mRandBetween0and34Position;
        std::vector<int> mRandBetween0and6;
        size_t mRandBetween0and6Position;
        std::vector<int> mRandBetween0and5;
        size_t mRandBetween0and5Position;
        std::vector<int> mRandBetween0and11;
        size_t mRandBetween0and11Position;
        std::vector<int> mRandBetween0and7;
        size_t mRandBetween0and7Position;

    private:
        ChordSymbolRepository();
        static const ChordSymbolRepository& getInstance();
        void populateCharValues();
        void populateRoots();
        void populateQualities();
        void populateAccidentals();
        void populateNumbers();
        void populateParensAndSlash();
        void populateRandomNumbers();
        void populateHandGenerated();
        void populateLogicallyGenerated();
        void populateRandomlyGenerated();
        bool getRandomBool();
        int getRandBetween0and34();
        int getRandBetween0and6();
        int getRandBetween0and5();
        int getRandBetween1and6();
        int getRandBetween0and11();
        int getRandBetween0and7();
        ChordSymbolCharValue getRandomCharValue();
        ChordSymbolCharValue getRandomRoot();
        ChordSymbolCharValue getRandomQuality();
        ChordSymbolCharValue getRandomAccidental();
        ChordSymbolCharValue getRandomNumber();
        ChordSymbolCharValue getRandomParensAndSlash();

        template<typename T>
        T getRand(const std::vector<T>& inVector, size_t& ioPosition)
        {
            const auto result = inVector.at(ioPosition);
            ++ioPosition;
            if (ioPosition >= inVector.size()) {
                ioPosition = 0;
            }
            return result;
        }
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
