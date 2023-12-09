// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TemporalImpl.h"

// Local
#include "Measure.h"
#include "Temporal.h"
#include "Staff.h"
#include "Score.h"
#include "TemporalValue.h"
#include "TemporalConstants.h"
#include "Throw.h"
#include "Curve.h"
#include "Tie.h"
#include "TupletIntf.h"

namespace komp
{
void
TemporalImpl::setDuration(Temporal& outTemporal,
                          Duration inDuration,
                          Duration& mOutDuration)
{
    KOMP_ASSERT(inDuration.getTemporalType() != TemporalType::UNHANDLED);
    const auto oldDuration = outTemporal.getDuration();
    const auto newDuration = inDuration;
    
    if (newDuration.equals(oldDuration)) {
        return;
    }
    
    if (!outTemporal.getTuplets().empty()) {
        //LOG_F(INFO, "once a tuplet has been added the temporal's duration values can no longer be changed.");
        return;
    }
    
    mOutDuration = newDuration;
    outTemporal.notifyObservers({ItemChangeType::Duration});
    
    const auto& inType = newDuration.getTemporalType();
    const bool hasStem = TemporalValue::getHasStem(inType);
    
    if (hasStem) {
        auto policy = outTemporal.getStemPolicy();
        
        if (policy.getIsAutomatic()) {
            
            if (policy.direction == StemDirection::NONE) {
                if (outTemporal.getSlot() > 6) {
                    policy.direction = StemDirection::DOWN;
                } else {
                    policy.direction = StemDirection::UP;
                }
            }
        }
        outTemporal.setStemPolicy(policy);
        
    } else if (!hasStem) {
        outTemporal.setStemPolicy(StemPolicy{StemDirection::NONE, Policy::AUTOMATIC});
    }
}


void
TemporalImpl::initializeStemPolicy(Temporal& outTemporal, StemPolicy& outStemPolicy)
{
    if (TemporalValue::getHasStem(outTemporal.getDuration().getTemporalType())) {
        outStemPolicy.direction = StemDirection::UP;
    } else {
        outStemPolicy.direction = StemDirection::NONE;
    }
    TemporalImpl::updateStemDirection(outTemporal);
}


void
TemporalImpl::setStemPolicy(Temporal& outTemporal,
                            const StemPolicy& inStemPolicy,
                            StemPolicy& outStemPolicy)
{
    if (inStemPolicy == outStemPolicy) {
        return;
    }
    
    const bool wasOldPolicyAutomatic = outStemPolicy.policy == Policy::AUTOMATIC;
    const bool isNewPolicyAutomatic = inStemPolicy.policy == Policy::AUTOMATIC;
    
    if (TemporalValue::getHasStem(outTemporal.getDuration().getTemporalType())) {
        outStemPolicy = inStemPolicy;
    } else {
        outStemPolicy.direction = StemDirection::NONE;
        outStemPolicy.policy = Policy::AUTOMATIC;
    }
    
    if (!wasOldPolicyAutomatic && isNewPolicyAutomatic) {
        TemporalImpl::updateStemDirection(outTemporal);
    }
}


static Pitch
transposePitch(const Temporal& inTemporal,
               const Instrument& inInstrument)
{
    const auto pitch = inTemporal.getPitch();
    const auto transposition = inInstrument.transposition;
    if (transposition == 0) {
        return pitch;
    }

    const auto location = inTemporal.getLocation();
    const auto midiPitch = pitch.getMidiNote();
    
    return Pitch(location.getClef(),
                 inTemporal.getSlot(),
                 midiPitch + transposition);
}

PlaybackNote
TemporalImpl::getPlaybackNote(const Temporal& inTemporal)
{
    PlaybackNote result;
    auto thisDuration = inTemporal.getDuration();
    thisDuration.setSustainType(SustainType::Percentage);
    thisDuration.setSustainPercentage(DEFAULT_DURATION_PERCENTAGE);
    result.addDuration(thisDuration);
    result.setStartTime(inTemporal.getTime());
    
    const auto tiedTemporals = inTemporal.getAttachments().getTieChain();
    for (const Temporal& tiedTemporal : tiedTemporals) {
        auto theDuration = tiedTemporal.getDuration();
        theDuration.setSustainType(SustainType::Percentage);
        theDuration.setSustainPercentage(DEFAULT_DURATION_PERCENTAGE);
        result.addDuration(theDuration);
    }
    
    const auto& musicLocation = inTemporal.getLocation();
    const auto& track = musicLocation.getTrack();
    const auto& instrument = track.getInstrument();
    
    const auto pitch = transposePitch(inTemporal, instrument);
    
    result.setPitch(pitch);
    
    for (const Curve& curve : inTemporal.getAttachments().getIncomingCurves()) {
        
        const auto& itemTraits = curve.getItemTraits();
        const auto itemType = itemTraits.getItemType();
        
        if (itemType == ItemType::Tie) {
            result.setIsSilent(true);
            break;
        }
    }
    
    if (inTemporal.getIsRest()) {
        result.setIsSilent(true);
    }
    
    if (inTemporal.getIsChord()) {
        result.setIsSilent(true);
    }
    
    if (inTemporal.getIsNote()) {
        static constexpr const double DefaultLoudness = 0.75;
        result.setLoudness(DefaultLoudness);
        result.setOffLoudness(DefaultLoudness);
    }
    
    return result;
}


bool
TemporalImpl::getIsTupleted(const TupletIntfRefVec& inTuplets)
{
    return inTuplets.size() > 0;
}


TupletIntfRefVec
TemporalImpl::getTuplets(const TupletIntfRefVec& inTuplets)
{
    TupletIntfRefVec tuplets;
    for (const auto& tuplet : inTuplets) {
        tuplets.emplace_back(std::move(tuplet));
    }
    return tuplets;
}


bool
TemporalImpl::addTuplet(Temporal& outTemporal,
                        TupletIntfRefVec& outTuplets,
                        TupletIntf& inTuplet)
{
    auto predicate = [&](const TupletIntf& vectorTuplet) {
        return &vectorTuplet == &inTuplet;
    };
    
    const auto iter = std::find_if(outTuplets.cbegin(),
                                   outTuplets.cend(),
                                   predicate);
    if (iter != outTuplets.cend()) {
        return false;
    }
    
    auto duration = outTemporal.getDuration();
    duration.addTupletRatio(inTuplet.getTupletRatio());
    outTemporal.setDuration(duration);
    outTuplets.emplace_back(inTuplet);
    return true;
}


bool
TemporalImpl::removeTuplet(Temporal& outTemporal,
                           TupletIntfRefVec& outTuplets,
                           TupletIntf& inTuplet)
{
    auto predicate = [&](const TupletIntf& vectorTuplet) {
        return &vectorTuplet == &inTuplet;
    };
    
    const auto iter = std::find_if(outTuplets.cbegin(), outTuplets.cend(), predicate);
    if (iter == outTuplets.cend()) {
        return false;
    }
    
    outTuplets.erase(iter);
    auto duration = outTemporal.getDuration();
    duration.removeTupletRatio(inTuplet.getTupletRatio());
    outTemporal.setDuration(duration);
    return true;
}


void
TemporalImpl::updateStemDirection(Temporal& outTemporal)
{
    if (!outTemporal.getStemInfo().isStemOwner) {
        return;
    }
    
    if (outTemporal.getStemPolicy().getIsAutomatic()) {
        if (TemporalValue::getHasStem(outTemporal.getDuration().getTemporalType())) {
            
            if (outTemporal.getIsGrace()) {
                auto policy = outTemporal.getStemPolicy();
                policy.policy = Policy::SPECIFIED;
                outTemporal.setStemPolicy(policy);
            } else {
                if (outTemporal.getSlot() > MIDDLE_SLOT) {
                    outTemporal.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
                } else {
                    outTemporal.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
                }
            }
        } else {
            outTemporal.setStemPolicy(StemPolicy{StemDirection::NONE, Policy::AUTOMATIC});
        }
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
