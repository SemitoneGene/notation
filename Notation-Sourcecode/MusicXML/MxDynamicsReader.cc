// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "MxDynamicsReader.h"

// Local
#include "ArpeggiateType.h"
#include "Mark.h"
#include "Measure.h"
#include "MxConverter.h"
#include "Note.h"
#include "Rest.h"
#include "RoundPlaces.h"
#include "Score.h"
#include "Track.h"

namespace komp
{
MxDynamicsReader::MxDynamicsReader(MxScoreReader& mxScore,
                                   mx::api::MarkData inMarkData,
                                   MxLocation inMxLocation)
: mMxScoreReader(mxScore)
, mMarkData(inMarkData)
, mMxLocation(inMxLocation)
, mDirectionPlacement(mx::api::Placement::unspecified)
{
    
}

MxDynamicsReader::MxDynamicsReader(MxScoreReader& mxScore,
                                   mx::api::MarkData inMarkData,
                                   MxLocation inMxLocation,
                                   mx::api::Placement inDirectionPlacement)
: mMxScoreReader(mxScore)
, mMarkData(inMarkData)
, mMxLocation(inMxLocation)
, mDirectionPlacement(inDirectionPlacement)
{

}

void
MxDynamicsReader::insertDynamic(Temporal& inTemporal)
{
    if (!mx::api::isMarkDynamic(mMarkData.markType)) {
        return;
    }
    const auto& track = mMxScoreReader.getTrack(mMxLocation.partIndex);
    KOMP_ASSERT(mMxLocation.handIndex >= 0);
    KOMP_ASSERT(mMxLocation.handIndex < track->getHands());
    auto dynamicType = MxConverter::convertMark(mMarkData.markType);
    
    if (dynamicType == MarkType::DYNAMIC_OTHER) {
        // TODO: SMUFLFIX - make sure this is working ok after smufl map removal
        if (mMarkData.name == "dynamicNiente") {
            dynamicType = MarkType::DYNAMIC_NIENTE;
        }
    }
    
    auto placement = mMarkData.positionData.placement;
    if (placement == mx::api::Placement::unspecified) {
        placement = mDirectionPlacement;
    }
    
    const auto above = mx::api::Placement::above;
    const auto kplace = placement == above ? PlacementType::ABOVE_STAFF
    : PlacementType::BELOW_STAFF;
    auto& reg = mMxScoreReader.getScore().getItemRegistry();
    auto& mark = inTemporal.getAttachments().addMark(reg,
                                                     inTemporal,
                                                     dynamicType,
                                                     kplace);
    
    if (mMarkData.positionData.isDefaultXSpecified && mMxScoreReader.getIsScalingFound()) {
        const auto defaultXTenths = mMarkData.positionData.defaultX;
        const auto defaultXSpaces = defaultXTenths / 10.0;
        const auto spaceSize = mMxScoreReader.getSpaceSize();
        KOMP_ASSERT(spaceSize > 0.0);
        const auto defaultXPixels = spaceSize * defaultXSpaces;
        mark.getRelativePoint().setX(defaultXPixels);
    }
    
    if (mMarkData.positionData.isDefaultYSpecified && mMxScoreReader.getIsScalingFound()) {
        const auto spaceSize = mMxScoreReader.getSpaceSize();
        const auto staffY = mark.getLocation().getStaff().getScoreY();
        const auto defaultY = komp::roundPlaces(mMarkData.positionData.defaultY, 5);
        const auto ySpaces = defaultY / 10;
        const auto yPixels = ySpaces * spaceSize * -1;
        const auto markY = staffY + yPixels;
        mark.getRelativePoint().setScoreY(markY);
    }
}

void
MxDynamicsReader::insertDynamicOnNearestTemporalIfPossible()
{
    if (!mx::api::isMarkDynamic(mMarkData.markType)) {
        return;
    }
    
    const auto& kompMeasure = mMxScoreReader.getScore().getMeasure(mMxLocation.getKompMeasureIndex());
    const auto& track = mMxScoreReader.getScore().getTracks().at(mMxLocation.partIndex);
    
    auto temporalsFilter = [&](Item& inItem) {
        if (inItem.is<Temporal>()) {
            return true;
        }
        return false;
    };
    
    auto temporals = kompMeasure->items<Temporal>(*track.get(),
                                                  temporalsFilter);
    
    if (temporals.size() == 0) {
        // TODO: message? some other trickery?
        return;
    }
    
    auto awesomeSortLambda = [&](Temporal& a, Temporal& b) {
        
        // sort temporals with the correct hand to the front
        
        if (std::abs(a.getLocation().getHand() - mMxLocation.handIndex)
            < std::abs(b.getLocation().getHand() - mMxLocation.handIndex)) {
            return true;
        }
        
        if (std::abs(a.getLocation().getHand() - mMxLocation.handIndex)
            > std::abs(b.getLocation().getHand() - mMxLocation.handIndex)) {
            return false;
        }
        
        // if we reach here, the temporals have the correct hand
        // now sort temporals with the closest time to the front
        
        if (std::abs(a.getRoundedTime() - mMxLocation.getKompTickPosition())
            < std::abs(b.getRoundedTime() - mMxLocation.getKompTickPosition())) {
            return true;
        }
        
        if (std::abs(a.getRoundedTime() - mMxLocation.getKompTickPosition())
            > std::abs(b.getRoundedTime() - mMxLocation.getKompTickPosition())) {
            return false;
        }
        
        // if we reach here, the temporals have the exact correct time
        // now sort temporals with the closest voice to the front
        
        if (std::abs(a.getLocation().getVoice() - mMxLocation.voiceIndex)
            < std::abs(b.getLocation().getVoice() - mMxLocation.voiceIndex)) {
            return true;
        }
        
        if (std::abs(a.getLocation().getVoice() - mMxLocation.voiceIndex)
            > std::abs(b.getLocation().getVoice() - mMxLocation.voiceIndex)) {
            return false;
        }
        
        // if we reach here, the temporals have the exact correct voice
        // now sort temporals with the closest slot to the side of the
        // staff where dynamic is placed
        
        if (mDirectionPlacement == mx::api::Placement::above) {
            
            if (a.getSlot() > b.getSlot()) {
                return true;
            }
            
            if (a.getSlot() < b.getSlot()) {
                return false;
            }
            
        } else {
            
            if (a.getSlot() < b.getSlot()) {
                return true;
            }
            
            if (a.getSlot() > b.getSlot()) {
                return false;
            }
        }
        
        return false;
    };
    
    std::ranges::sort(temporals, awesomeSortLambda);
    Temporal& bestTemporal = temporals.front();
    
    Temporal* markTemporal;
    if (bestTemporal.getIsChordMember()) {
        markTemporal = bestTemporal.getChord();
    } else {
        markTemporal = &bestTemporal;
    }
    
    auto kompMarkType = MxConverter::convertMark(mMarkData.markType);
    
    if (kompMarkType == MarkType::DYNAMIC_OTHER) {
        // TODO: SMUFLFIX - make sure this is working ok after smufl map removal
        if (mMarkData.name == "dynamicNiente") {
            kompMarkType = MarkType::DYNAMIC_NIENTE;
        }
    }
    
    auto placement = mMarkData.positionData.placement;
    
    if (placement == mx::api::Placement::unspecified) {
        placement = mDirectionPlacement;
    }
    
    const auto kplace = placement == mx::api::Placement::above ? PlacementType::ABOVE_STAFF : PlacementType::BELOW_STAFF;
    auto& reg = mMxScoreReader.getScore().getItemRegistry();
    markTemporal->getAttachments().addMark(reg,
                                           *markTemporal,
                                           kompMarkType,
                                           kplace);
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
