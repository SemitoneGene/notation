// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "MxMeasureWriter.h"
#include "MxStaffWriter.h"
#include "MxConverter.h"

// Komp
#include "KeySignature.h"
#include "Measure.h"
#include "Score.h"
#include "Track.h"

namespace komp
{

    MxMeasureWriter::MxMeasureWriter(Measure& inMeasure,
                                     MxLocation inMxLocation,
                                     MxScoreWriter& inMxScoreWriter)
    : mMeasure(inMeasure)
    , mMxLocation(inMxLocation)
    , mMxScoreWriter(inMxScoreWriter)
    {
        KOMP_ASSERT(mMeasure.getStaffSystem() != nullptr);
        KOMP_ASSERT(mMeasure.getIndex() == inMxLocation.getKompMeasureIndex());
        KOMP_ASSERT(mMxLocation.partIndex >= 0);
        KOMP_ASSERT(mMxLocation.partIndex < static_cast<int>(mMxScoreWriter.getScore().getTracks().size()));
        const auto& track = mMxScoreWriter.getScore().getTracks().at(mMxLocation.partIndex);
        KOMP_ASSERT(mMxLocation.handIndex >= 0);
        KOMP_ASSERT(mMxLocation.handIndex < track->getHands());
    }
    

    mx::api::MeasureData
    MxMeasureWriter::getMeasureData()
    {
        mOutMeasureData = mx::api::MeasureData{};
        const double widthPixels = mMeasure.getLeftGap() + mMeasure.getNoteSpace();
        const double widthSpaces = widthPixels / mMxScoreWriter.getSpaceSize();
        const double widthTenths = widthSpaces * 10;
        mOutMeasureData.width = widthTenths;

        setTimeSignature();
        auto& track = mMxScoreWriter.getScore().getTracks().at(mMxLocation.partIndex);
        
        auto keySignatures = mMeasure.items<KeySignature>(*track.get(),
                                                          mMxLocation.handIndex);
        for (const KeySignature& keyPtr : keySignatures) {
            
            if (keyPtr.getRoundedTime() == 0) {
                
                const auto key = keyPtr.getKey();
                mx::api::KeyData keyData;
                keyData.fifths = key.getAccidentals();
                keyData.tickTimePosition = 0;
                mOutMeasureData.keys.emplace_back(keyData);
            }
        }

        
        mMxLocation.handIndex = 0;
        for( ; mMxLocation.handIndex < track->getHands(); ++mMxLocation.handIndex) {
            const auto items = getItems(*track.get(), mMxLocation.handIndex);
            MxStaffWriter staffWriter{items, mMxLocation, mMxScoreWriter};
            auto staffData = staffWriter.getStaffData();
            mOutMeasureData.staves.emplace_back(std::move(staffData));
        }
        
        const auto leftBarline = mMeasure.getLeftBarline();
        mx::api::BarlineData leftData;
        leftData.barlineType = MxConverter::convertLeftBarline(leftBarline);
        leftData.tickTimePosition = 0;
        leftData.location = mx::api::HorizontalAlignment::left;
        if (leftBarline == BarlineType::LEFT_REPEAT) {
            leftData.repeat = true;
        }
//        if (mMeasure.hasEndingStart()) {
//            leftData.endingType = mx::api::EndingType::start;
//            leftData.endingNumber = mMeasure.getEnding();
//        }
        mOutMeasureData.barlines.emplace_back(std::move(leftData));

        const auto rightBarline = mMeasure.getRightBarline();
        mx::api::BarlineData rightData;
        rightData.barlineType = MxConverter::convertRightBarline(rightBarline);
        rightData.tickTimePosition = mMeasure.getEndTime();
        rightData.location = mx::api::HorizontalAlignment::right;
//        if (mMeasure.hasEndingStop()) {
//            rightData.endingType = mx::api::EndingType::stop;
//            rightData.endingNumber = mMeasure.getEnding();
//        }
        if (rightBarline == BarlineType::RIGHT_REPEAT) {
            rightData.repeat = true;
        }
        mOutMeasureData.barlines.emplace_back(std::move(rightData));

        mx::api::MeasureData temp = std::move(mOutMeasureData);
        mOutMeasureData = mx::api::MeasureData{};
        return temp; 
    }
    
    void
    MxMeasureWriter::setTimeSignature()
    {
        auto currentMeasureTimeSignatureData = parseTimeSignatureDataFromKompMeasure(mMeasure);
        currentMeasureTimeSignatureData.isImplicit = true;
        if (mMxLocation.getKompMeasureIndex() == 0) {
            currentMeasureTimeSignatureData.isImplicit = false;
            auto displayValue = mx::api::Bool::unspecified;
            const auto isMeterVisible = mMeasure.getMeterVisible();
            if (!isMeterVisible) {
                // TODO: allow for explicit hiding of the time signature once
                // Gene changes the measureIsDisplayed accessor
                //displayValue = mx::api::Bool::no;
            }
            currentMeasureTimeSignatureData.display = displayValue;
        } else {
            const auto& previousMeasure = mMxScoreWriter.getScore().getMeasure(mMxLocation.getKompMeasureIndex() - 1);
            auto previousTimeSignatureData = parseTimeSignatureDataFromKompMeasure(*previousMeasure.get());
            const bool isBeatsChanged = currentMeasureTimeSignatureData.beats != previousTimeSignatureData.beats;
            const bool isBeatTypeChanged = currentMeasureTimeSignatureData.beatType != previousTimeSignatureData.beatType;
            const bool isSymbolChanged = currentMeasureTimeSignatureData.symbol != previousTimeSignatureData.symbol;
            const bool isChanged = isBeatsChanged || isBeatTypeChanged || isSymbolChanged;
            if (isChanged || mMeasure.getMeterVisible() ) {
                currentMeasureTimeSignatureData.isImplicit = false;
            }
        }
        
        if (!mMeasure.getMeterVisible() && !currentMeasureTimeSignatureData.isImplicit) {
            // TODO: allow for explicit hiding of the time signature once
            // Gene changes the getMeterVisible accessor
            //currentMeasureTimeSignatureData.display = mx::api::Bool::no;
        }
        mOutMeasureData.timeSignature = currentMeasureTimeSignatureData;
    }
    
    
    mx::api::TimeSignatureData
    MxMeasureWriter::parseTimeSignatureDataFromKompMeasure(Measure& measure) const
    {
        mx::api::TimeSignatureData timeSigData{};
        timeSigData.beats = measure.getBeats();
        timeSigData.beatType = measure.getBeatUnit();
        timeSigData.symbol = MxConverter::convertTimeSignatureType( measure.getTimeSignatureType() );
        return timeSigData;
    }


    ItemRefVec
    MxMeasureWriter::getItems(Track& inTrack, int handIndex) const
    {
        auto filter = [&](Item& inFilterItem) {

            const auto& loc = inFilterItem.getLocation();
            
            if (loc.getHand() != handIndex) {
                return false;
            }

            if (&loc.getTrack() != &inTrack) {
                return false;
            }

            if (inFilterItem.is<Temporal>()) {
                const auto& inFilterTemporal = inFilterItem.as<Temporal>();
                if (inFilterTemporal.getIsChord()) {
                    return false;
                }
            }

            return true;
        };

        auto allItems = mMeasure.allItems();
        ItemRefVec items;
        std::copy_if(allItems.begin(), allItems.end(), std::back_inserter(items), filter);
        sortItems(items);
        return items;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
