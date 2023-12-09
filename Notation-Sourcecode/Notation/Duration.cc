// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Duration.h"

// Local
#include "Constants.h"
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{
    Duration::Duration()
    : mTemporalType{TemporalType::QUARTER}
    , mDots{0}
    , mSustainType{SustainType::Full}
    , mSustainPercentage{1.0}
    , mFixedSustainClocks{0.0}
    {

    }
    
    Duration::Duration(TemporalType inTemporalType)
    : mTemporalType{inTemporalType}
    , mDots{0}
    , mSustainType{SustainType::Full}
    , mSustainPercentage{1.0}
    , mFixedSustainClocks{0.0}
    {

    }
    
    double
    Duration::getClocks() const
    {
        return calculateClocks();
    }
    
    int
    Duration::getRoundedClocks() const
    {
        return static_cast<int>(std::ceil(getClocks() - 0.5));
    }
    
    TemporalType
    Duration::getTemporalType() const
    {
        return mTemporalType;
    }
    
    const Duration&
    Duration::setTemporalType(TemporalType inValue)
    {
        mTemporalType = inValue;
        return *this;
    }
    
    int
    Duration::getDots() const
    {
        return mDots;
    }

    const Duration&
    Duration::setDots(int inValue)
    {
        if (inValue <= 0) {
            mDots = 0;
        } else if (inValue >= MAX_DOTS) {
            mDots = MAX_DOTS;
        } else {
            mDots = inValue;
        }
        return *this;
    }
    
    
    const Duration&
    Duration::parseClocks(double inClocks)
    {
        const int rounded = static_cast<int>(std::ceil(inClocks - 0.5));
        
        // TODO: move this to TemporalValue
        
        switch (rounded) {
            case static_cast<int>(TemporalValue::SIXTEENTH): {
                setTemporalType(TemporalType::SIXTEENTH);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::EIGHTH): {
                setTemporalType(TemporalType::EIGHTH);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::QUARTER): {
                setTemporalType(TemporalType::QUARTER);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::HALF): {
                setTemporalType(TemporalType::HALF);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::WHOLE): {
                setTemporalType(TemporalType::WHOLE);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::THIRTY_SECOND): {
                setTemporalType(TemporalType::THIRTY_SECOND);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::SIXTY_FOURTH): {
                setTemporalType(TemporalType::SIXTY_FOURTH);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::ONE_TWENTY_EIGHTH): {
                setTemporalType(TemporalType::ONE_TWENTY_EIGHTH);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::TWO_FIFTY_SIXTH): {
                setTemporalType(TemporalType::TWO_FIFTY_SIXTH);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::FIVE_TWELFTH): {
                setTemporalType(TemporalType::FIVE_TWELFTH);
                setDots(0);
                break;
            }
            case static_cast<int>(TemporalValue::TEN_TWENTY_FOURTH): {
                setTemporalType(TemporalType::TEN_TWENTY_FOURTH);
                setDots(0);
                break;
            }
            default:
                break;
        }
        return *this;
    }
    
    
    double
    Duration::getSustainClocks() const
    {
        switch (getSustainType()) {
            case SustainType::Full: return getClocks();
            case SustainType::Percentage: return getClocks() * getSustainPercentage();
            case SustainType::Fixed: return getSustainFixedClocks();
            default: return 0.0;
        }
    }
    
    
    int
    Duration::getRoundedSustainClocks() const
    {
        return static_cast<int>(std::ceil(getSustainClocks() - 0.5));
    }
    
    
    SustainType
    Duration::getSustainType() const
    {
        return mSustainType;
    }
    
    
    const Duration&
    Duration::setSustainType(SustainType inValue)
    {
        mSustainType = inValue;
        return *this;
    }
    
    
    double
    Duration::getSustainPercentage() const
    {
        return mSustainPercentage;
    }
    
    
    const Duration&
    Duration::setSustainPercentage(double inValue)
    {
        mSustainPercentage = std::max(0.0, inValue);
        return *this;
    }
    
    
    double
    Duration::getSustainFixedClocks() const
    {
        return mFixedSustainClocks;
    }
    
    
    const Duration&
    Duration::setSustainFixedClocks(double inValue)
    {
        mFixedSustainClocks = std::max(0.0, inValue);
        return *this;
    }
    
    inline static bool doublesEqual(double l, double r) {
        return std::abs(l - r) < 0.0001;
    }


    const std::vector<TupletRatio>&
    Duration::getTupletRatios() const
    {
        return mTupletRatios;
    }


    void
    Duration::addTupletRatio(TupletRatio inTupletRatio)
    {
        KOMP_ASSERT(mTupletRatios.size() < MAX_TUPLET_NESTING);
        mTupletRatios.emplace_back(std::move(inTupletRatio));
    }


    bool
    Duration::removeTupletRatio(const TupletRatio& inTupletRatio)
    {
        for (auto it = mTupletRatios.cbegin(); it != mTupletRatios.cend(); ++it) {
            const auto& tupletRatio = *it;
            if (tupletRatio.equals(inTupletRatio)) {
                mTupletRatios.erase(it);
                return true;
            }
        }
        return false;
    }


    void
    Duration::clearTupletRatios()
    {
        mTupletRatios.clear();
    }
    
    
    double
    Duration::getUntupletedClocks() const
    {
        double value = TemporalValue::getValue(getTemporalType());
        double augmentationAmount = value / 2.0;
        
        for (int i = 0; i < getDots(); ++i) {
            value += augmentationAmount;
            augmentationAmount /= 2.0;
        }
        
        return value;
    }

    
    bool
    Duration::equals(const Duration& other) const
    {
        if (this->getTemporalType() != other.getTemporalType()) {
            return false;
        }

        if (this->getDots() != other.getDots()) {
            return false;
        }

        if (this->getSustainType() != other.getSustainType()) {
            return false;
        }

        if (!doublesEqual(this->getSustainPercentage(), other.getSustainPercentage())) {
            return false;
        }

        if (!doublesEqual(this->getSustainFixedClocks(), other.getSustainFixedClocks())) {
            return false;
        }

        if (!doublesEqual(this->getClocks(), other.getClocks())) {
            return false;
        }

        if (mTupletRatios.size() != other.mTupletRatios.size()) {
            return false;
        }

        auto thisIter = std::cbegin(mTupletRatios);
        auto otherIter = std::cbegin(other.mTupletRatios);
        const auto thisEnd = std::cend(mTupletRatios);
        const auto otherEnd = std::cend(other.mTupletRatios);

        for ( ; thisIter != thisEnd && otherIter != otherEnd; ++thisIter, ++otherIter) {
            if (!thisIter->equals(*otherIter)) {
                return false;
            }
        }

        return true;
    }
    
    
    double
    Duration::calculateClocks() const
    {
        double value = getUntupletedClocks();
        
        for (const auto& tupletRatio : getTupletRatios()) {
            value *= tupletRatio.getModifier();
        }
        
        return value;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
