// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordSymbolRepository.h"

// Local
#include "Throw.h"

namespace komp
{
	ChordSymbolRepository::ChordSymbolRepository()
    : mHandGenerated{}
    , mLogicallyGenerated{}
    , mRandomlyGenerated{}
    , mCharValues{}
    , mRoots{}
    , mQualities{}
    , mAccidentals{}
    , mNumbers{}
    , mParensAndSlash{}
    , mRandomBools{}
    , mRandomBoolsPosition{0}
    , mRandBetween0and34{}
    , mRandBetween0and34Position{0}
    , mRandBetween0and6{}
    , mRandBetween0and6Position{0}
    , mRandBetween0and5{}
    , mRandBetween0and5Position{0}
    , mRandBetween0and11{}
    , mRandBetween0and11Position{0}
    , mRandBetween0and7{}
    , mRandBetween0and7Position{0}
	{
        populateCharValues();
        populateRoots();
        populateQualities();
        populateAccidentals();
        populateNumbers();
        populateParensAndSlash();
        populateRandomNumbers();
        populateHandGenerated();
        populateLogicallyGenerated();
        populateRandomlyGenerated();
	}


    const ChrdSymValues&
    ChordSymbolRepository::getHandpicked()
    {
        const auto& theInstance = getInstance();
        const auto& result = theInstance.mHandGenerated;
        return result;
    }


    const ChrdSymValues&
    ChordSymbolRepository::getLogicallyGenerated()
    {
        const auto& theInstance = getInstance();
        const auto& result = theInstance.mLogicallyGenerated;
        return result;
    }


    const ChrdSymValues&
    ChordSymbolRepository::getRandomlyGenerated()
    {
        const auto& theInstance = getInstance();
        const auto& result = theInstance.mRandomlyGenerated;
        return result;
    }


    const ChordSymbolRepository&
    ChordSymbolRepository::getInstance()
    {
        static const ChordSymbolRepository instance;
        return instance;
    }


    void
    ChordSymbolRepository::populateCharValues()
    {
        mCharValues.push_back(ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT);
        mCharValues.push_back(ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP);
        mCharValues.push_back(ChordSymbolCharValue::ACCIDENTAL_FLAT);
        mCharValues.push_back(ChordSymbolCharValue::ACCIDENTAL_NATURAL);
        mCharValues.push_back(ChordSymbolCharValue::ACCIDENTAL_SHARP);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_11);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_13);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_2);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_4);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_5);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_6);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_7);
        mCharValues.push_back(ChordSymbolCharValue::NUMBER_9);
        mCharValues.push_back(ChordSymbolCharValue::PARENS_LEFT);
        mCharValues.push_back(ChordSymbolCharValue::PARENS_RIGHT);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_AUG_SYMBOL);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_AUG_TEXT);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_DIM_SYMBOL);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_DIM_TEXT);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_MIN_TEXT);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_SUS_TEXT);
        mCharValues.push_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_A);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_B);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_C);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_D);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_E);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_F);
        mCharValues.push_back(ChordSymbolCharValue::ROOT_G);
        mCharValues.push_back(ChordSymbolCharValue::SLASH);
    }


    void
    ChordSymbolRepository::populateRoots()
    {
        mRoots.push_back(ChordSymbolCharValue::ROOT_A);
        mRoots.push_back(ChordSymbolCharValue::ROOT_B);
        mRoots.push_back(ChordSymbolCharValue::ROOT_C);
        mRoots.push_back(ChordSymbolCharValue::ROOT_D);
        mRoots.push_back(ChordSymbolCharValue::ROOT_E);
        mRoots.push_back(ChordSymbolCharValue::ROOT_F);
        mRoots.push_back(ChordSymbolCharValue::ROOT_G);
    }


    void
    ChordSymbolRepository::populateQualities()
    {
        mQualities.push_back(ChordSymbolCharValue::QUALITY_AUG_SYMBOL);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_AUG_TEXT);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_DIM_SYMBOL);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_DIM_TEXT);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_MIN_TEXT);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_SUS_TEXT);
        mQualities.push_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M);
    }


    void
    ChordSymbolRepository::populateAccidentals()
    {
        mAccidentals.push_back(ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT);
        mAccidentals.push_back(ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP);
        mAccidentals.push_back(ChordSymbolCharValue::ACCIDENTAL_FLAT);
        mAccidentals.push_back(ChordSymbolCharValue::ACCIDENTAL_NATURAL);
        mAccidentals.push_back(ChordSymbolCharValue::ACCIDENTAL_SHARP);
    }


    void
    ChordSymbolRepository::populateNumbers()
    {
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_11);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_13);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_2);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_4);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_5);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_6);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_7);
        mNumbers.push_back(ChordSymbolCharValue::NUMBER_9);
    }


    void
    ChordSymbolRepository::populateParensAndSlash()
    {
        mParensAndSlash.push_back(ChordSymbolCharValue::PARENS_LEFT);
        mParensAndSlash.push_back(ChordSymbolCharValue::PARENS_RIGHT);
        mParensAndSlash.push_back(ChordSymbolCharValue::SLASH);
    }


    void
    ChordSymbolRepository::populateRandomNumbers()
    {
        mRandomBools = std::vector<bool>{ false, false, false, true, false, false, false, true, true, false, true, true, true, true, true, false, true, false, true, false, true, false, true, false, true, false, true, true, true, false, false, false, true, false, false, true, true, true, false, false, false, false, true, true, true, true, false, false, true, false, false, false, true, true, false, true, false, false, true, true };
        mRandBetween0and34 = std::vector<int>{ 6, 25, 0, 27, 12, 14, 4, 28, 17, 10, 6, 15, 1, 7, 0, 13, 21, 13, 23, 9, 3, 33, 28, 30, 25, 10, 12, 25, 28, 13, 4, 22, 11, 21, 2, 30, 24, 13, 25, 33, 23, 28, 6, 13, 14, 19, 9, 24, 16, 5, 30, 28, 29, 17, 15, 14, 29, 3, 14, 4, 29, 27, 7, 24, 34, 33, 26, 7, 20, 12, 29, 6, 8, 13, 1, 26, 0, 2, 23, 0, 17, 31, 16, 14, 31, 11, 22, 28, 33, 2, 22, 31, 26, 1, 29, 12, 18, 4, 2, 21, 14, 16, 11, 32, 8, 9, 30, 15, 1, 11, 21, 4, 18, 24, 24, 32, 3, 3, 19, 16, 12, 32, 27, 34, 4, 5, 29, 23, 29, 1, 29, 23, 34, 8, 0, 3, 2, 16, 34, 12, 4, 16, 13, 8, 9, 26, 25, 15, 21, 21, 9, 26, 20, 24, 3, 17, 3, 19, 23, 24, 32, 10, 23, 34, 28, 31, 12, 18, 15, 27, 31, 29, 8, 9, 33, 15, 12, 30, 3, 5, 9, 12, 8, 16, 3, 6, 15, 26, 23, 19, 0, 34, 30, 0, 1, 4, 19, 1, 23, 34, 29, 26, 0, 5, 13, 33, 12, 20, 7, 2, 13, 31, 3, 28, 20, 12, 34, 26, 17, 17, 23, 10, 13, 20, 19, 22, 9, 1, 0, 34, 9, 8, 32, 6, 10, 0, 14, 23, 13, 19, 27, 7, 23, 13, 21, 6, 32, 26, 13, 2, 34, 7, 4, 29, 5, 27, 7, 33, 34, 32, 11, 21, 2, 33, 2, 0, 20, 17, 7, 5, 7, 19, 23, 26, 30, 22, 0, 23, 19, 30, 21, 18, 1, 17, 32, 10, 5, 33, 6, 3, 14, 11, 24, 20, 15, 11, 10, 7, 15, 11, 29, 30, 29, 6, 13, 23, 1, 16, 19, 11, 26, 0, 16, 17, 13, 10, 18, 34, 28, 26, 6, 11, 31, 5, 25, 20, 25, 23, 24, 26, 25, 26, 34, 14, 33, 22, 3, 32, 3, 24, 9, 31, 15, 19, 10, 25, 0, 28, 26, 19, 7, 32, 27, 6, 19, 26, 3, 11, 15, 20, 32, 29, 9, 11, 28, 5, 19, 3, 25, 8, 34, 18, 27, 32, 2, 26, 0, 29, 32, 13, 17, 9, 28, 25, 16, 33, 7, 5, 10, 9, 10, 3, 14, 32, 12, 10, 30, 11, 4, 23, 4, 8, 26, 31, 3, 31, 9, 15, 13, 20, 1, 6, 4, 24, 10, 31, 7, 28, 32, 0, 3, 28, 33, 6, 32, 21, 1, 25, 17, 33, 4, 0, 13, 20, 22, 25, 15, 26, 19, 4, 21, 9, 26, 31, 11, 9, 9, 2, 11, 30, 6, 4, 22, 22, 8, 26, 18, 16, 8, 9, 21, 29, 5, 7, 23, 14, 25, 31, 25, 26, 2, 0, 23, 16, 5, 20, 15, 17, 6, 18, 26, 30, 10, 29, 9, 3, 19, 16, 25, 18, 6, 23, 13, 27, 2, 14, 26, 9, 1, 16, 16, 5, 18, 1, 8, 26, 32, 6, 31, 4, 20, 22, 14 };
        mRandBetween0and6 = std::vector<int>{ 4, 4, 5, 6, 6, 6, 0, 3, 1, 3, 3, 0, 5, 5, 2, 1, 4, 2, 4, 4, 6, 5, 4, 3, 4, 5, 6, 4, 2, 2, 4, 4, 6, 3, 0, 5, 6, 4, 0, 4, 1, 5, 4, 2, 6, 3, 2, 6, 5, 0, 3, 1, 6, 6, 1, 3, 2, 2, 0, 3, 2, 1, 6, 0, 0, 5, 4, 3, 5, 3, 0, 2, 0, 3, 0, 4, 6, 3, 1, 0, 6, 5, 1, 1, 2, 6, 3, 4, 4, 1, 3, 0, 5, 4, 2, 6, 5, 5, 5, 6, 2, 5, 5, 4, 5, 4, 5, 4, 4, 0, 4, 4, 2, 1, 5, 2, 5, 3, 5, 4, 4, 4, 5, 3, 6, 1, 6, 0, 1, 3, 2, 0, 4 };
        mRandBetween0and5 = std::vector<int>{ 4, 1, 3, 3, 4, 5, 5, 5, 5, 2, 3, 4, 2, 0, 4, 2, 4, 5, 1, 5, 4, 1, 5, 1, 1, 3, 1, 5, 3, 1, 2, 3, 1, 3, 5, 1, 4, 2, 3, 5, 1, 2, 0, 5, 0, 4, 1, 0, 1, 5, 3, 2, 2, 0, 2, 1, 4, 1, 2, 3, 5, 4, 5, 5, 3, 1, 1, 0, 3, 2, 4, 3, 1, 0, 5, 0, 0, 1, 4, 4, 3, 5, 1, 3, 3, 2, 0, 4, 1, 2, 4, 0, 2, 5, 4, 4, 4 };
        mRandBetween0and11 = std::vector<int>{ 11, 5, 10, 2, 2, 5, 6, 8, 5, 3, 2, 1, 3, 3, 5, 5, 9, 11, 11, 6, 2, 0, 8, 0, 1, 9, 5, 4, 1, 11, 9, 10, 10, 6, 7, 3, 4, 6, 6, 10, 8, 7, 9, 0, 3, 5, 3, 9, 6, 7, 0, 4, 11, 7, 5, 6, 0, 2, 8, 4, 11, 0, 10, 5, 0, 8, 10, 4, 8, 1, 0, 4, 7, 0, 1, 4, 4, 2, 2, 9, 11, 3, 5, 5, 7, 1, 3, 1, 3, 11, 2, 4, 5, 2, 9, 1, 5, 6, 1, 9, 10, 4, 8, 11, 1, 6, 0, 9, 6, 8, 3, 8, 11, 7, 2, 9, 6, 5, 7, 10, 3, 0, 7, 11, 1, 4, 5, 7, 3, 7, 1, 8, 6, 10, 2, 2, 8, 1, 6, 0, 9, 7, 3, 9, 1, 10, 8, 8, 3, 2, 10, 1, 6, 5, 4, 8, 10, 4, 0, 3, 2, 8, 9, 11, 0, 6, 10, 7, 7, 5, 7, 10, 7, 6, 8, 10, 4, 6, 6, 4, 7, 10, 0, 10, 7, 3, 9, 3, 8, 6, 10, 9, 9, 11, 10, 11, 10, 7, 10, 2, 6, 1, 11 };
        mRandBetween0and7 = std::vector<int>{ 1, 5, 7, 6, 5, 0, 7, 2, 5, 2, 3, 2, 0, 3, 5, 0, 2, 6, 4, 5, 0, 6, 3, 0, 4, 3, 2, 6, 0, 5, 2, 2, 1, 5, 1, 5, 2, 1, 7, 6, 7, 5, 2, 7, 7, 5, 5, 6, 0, 5, 1, 7, 0, 1, 3, 2, 3, 1, 4, 1, 4, 4, 2, 3, 6, 0, 3, 5, 2, 4, 6, 2, 0, 5, 0, 3, 5, 7, 0, 0, 7, 7, 5, 0, 4, 1, 5, 6, 6, 1, 5, 4, 1, 6, 6, 1, 7, 7, 1, 5, 5, 1, 6, 6, 4, 1, 1, 6, 7, 0, 6, 5, 0, 7, 7, 3, 4, 2, 6, 0, 7, 5, 4, 3, 3, 0, 6, 1, 3, 2, 6, 1, 2, 1, 1, 5, 6, 0, 7, 1, 0, 2, 1, 4, 1, 6, 2, 3, 7, 0, 3, 6, 7, 0, 2, 5, 6, 3, 1, 2, 6, 4, 7, 0, 3, 1, 6, 5, 7, 5, 7, 0, 7, 4, 7, 5, 3 };
    }


	void
	ChordSymbolRepository::populateHandGenerated()
	{
        ChordSymbolString str;
        str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
        str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
        mHandGenerated.emplace_back(str);

        str.clear();
        str.emplace_back(ChordSymbolCharValue::ROOT_D, false);
        str.emplace_back(ChordSymbolCharValue::QUALITY_SUS_TEXT, true);
        str.emplace_back(ChordSymbolCharValue::NUMBER_4, true);
        mHandGenerated.emplace_back(str);
	}


    void
    ChordSymbolRepository::populateLogicallyGenerated()
    {
        for (int i = 0; i < 1000; ++i) {

//            if (i == 309) {
//                std::cout << "309" << std::endl;
//            }

            ChordSymbolString random;

            random.emplace_back(getRandomRoot(), false);

            if (getRandomBool()) {
                const auto accidental = getRandomAccidental();
                random.emplace_back(accidental, getRandomBool());
            }

            const bool a = getRandomBool();
            const bool b = getRandomBool();
            const bool isQualityShown = a || b;

            if (isQualityShown) {
                const auto quality = getRandomQuality();
                random.emplace_back(quality, getRandomBool());

                if (quality == ChordSymbolCharValue::QUALITY_SUS_TEXT) {
                    if (getRandomBool()) {
                        random.emplace_back(ChordSymbolCharValue::NUMBER_2, getRandomBool());
                    } else {
                        random.emplace_back(ChordSymbolCharValue::NUMBER_4, getRandomBool());
                    }
                }
            }

            if (getRandomBool()) {

            } else {

                if (getRandomBool()) {
                    random.emplace_back(ChordSymbolCharValue::NUMBER_7, getRandomBool());
                }

                if (getRandomBool()) {
                    if (getRandomBool()) {
                        random.emplace_back(getRandomAccidental(), getRandomBool());
                    }
                    random.emplace_back(ChordSymbolCharValue::NUMBER_9, getRandomBool());
                }

                if (getRandomBool()) {
                    if (getRandomBool()) {
                        random.emplace_back(getRandomAccidental(), getRandomBool());
                    }
                    random.emplace_back(ChordSymbolCharValue::NUMBER_11, getRandomBool());
                }

                if (getRandomBool()) {
                    if (getRandomBool()) {
                        random.emplace_back(getRandomAccidental(), getRandomBool());
                    }
                    random.emplace_back(ChordSymbolCharValue::NUMBER_13, getRandomBool());
                }
            }

            const bool one = getRandomBool();
            const bool two = getRandomBool();

            if (one && two) {
                random.emplace_back(ChordSymbolCharValue::SLASH, false);
                random.emplace_back(getRandomRoot(), false);
                if (getRandomBool()) {
                    random.emplace_back(getRandomAccidental(), false);
                }
            }

            mLogicallyGenerated.emplace_back(random);
        }
    }


    void
    ChordSymbolRepository::populateRandomlyGenerated()
    {

        for (int i = 0; i < 1000; ++i) {

            ChordSymbolString random;
            const int numChars = getRandBetween1and6();

            for (int j = 0; j < numChars; ++j) {
                const auto charValue = getRandomCharValue();
                random.emplace_back(charValue, getRandomBool());
            }

            mRandomlyGenerated.emplace_back(random);
        }
    }

    bool
    ChordSymbolRepository::getRandomBool()
    {
        return getRand(mRandomBools, mRandomBoolsPosition);
    }


    int
    ChordSymbolRepository::getRandBetween0and34()
    {
        return getRand(mRandBetween0and34, mRandBetween0and34Position);
    }


    int
    ChordSymbolRepository::getRandBetween0and6()
    {
        return getRand(mRandBetween0and6, mRandBetween0and6Position);
    }


    int
    ChordSymbolRepository::getRandBetween0and5()
    {
        return getRand(mRandBetween0and5, mRandBetween0and5Position);
    }


    int
    ChordSymbolRepository::getRandBetween1and6()
    {
        return getRand(mRandBetween0and5, mRandBetween0and5Position) + 1;
    }


    int
    ChordSymbolRepository::getRandBetween0and11()
    {
        return getRand(mRandBetween0and11, mRandBetween0and11Position);
    }


    int
    ChordSymbolRepository::getRandBetween0and7()
    {
        return getRand(mRandBetween0and7, mRandBetween0and7Position);
    }


    ChordSymbolCharValue
    ChordSymbolRepository::getRandomCharValue()
    {
        return mCharValues.at(getRandBetween0and34());
    }


    ChordSymbolCharValue
    ChordSymbolRepository::getRandomRoot()
    {
        return mRoots.at(getRandBetween0and6());
    }


    ChordSymbolCharValue
    ChordSymbolRepository::getRandomQuality()
    {
        return mQualities.at(getRandBetween0and11());
    }


    ChordSymbolCharValue
    ChordSymbolRepository::getRandomAccidental()
    {
        const int randValue = getRandBetween0and5();

        if (randValue == 5) {
            return ChordSymbolCharValue::ACCIDENTAL_SHARP;
        }

        return mAccidentals.at(randValue);
    }


    ChordSymbolCharValue
    ChordSymbolRepository::getRandomNumber()
    {
        return mNumbers.at(getRandBetween0and7());
    }


    ChordSymbolCharValue
    ChordSymbolRepository::getRandomParensAndSlash()
    {
        const bool a = getRandomBool();
        const bool b = getRandomBool();
        if (a && b ) {
            return ChordSymbolCharValue::SLASH;
        } else if ( a ) {
            return ChordSymbolCharValue::PARENS_LEFT;
        }

        return ChordSymbolCharValue::PARENS_RIGHT;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
