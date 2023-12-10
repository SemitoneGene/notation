// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp
#include "Clef.h"
#include "Constants.h"
#include "FileMediatorFactory.h"
#include "Forward.h"
#include "Item.h"
#include "Measure.h"
#include "Note.h"
#include "Pitch.h"
#include "Rest.h"
#include "Score.h"
#include "Staff.h"
#include "TemporalType.h"
#include "TemporalValue.h"
#include "Track.h"

namespace komp
{
    template <typename T>
    inline std::vector<std::reference_wrapper<T>> getItems(const komp::Score& inScore,
                                                    int measureIndex,
                                                    int trackIndex,
                                                    int handIndex,
                                                    int voiceIndex)
    {
        using namespace komp;
        const auto& measure = inScore.getMeasure(measureIndex);
        const auto& track = inScore.getTracks().at(trackIndex);
        KOMP_ASSERT(track);
        KOMP_ASSERT(handIndex >= 0);
        KOMP_ASSERT(handIndex < track->getHands());

        auto filter = [&](Item& inFilterItem) {
            if (!inFilterItem.is<T>()) {
                return false;
            }
            
            const auto& loc = inFilterItem.getLocation();
            if (&loc.getTrack() != track.get()) {
                return false;
            }

            if (loc.getHand() != handIndex) {
                return false;
            }

            if (loc.getVoice() != voiceIndex) {
                return false;
            }

            return true;
        };

        auto items = measure->items<T>(*track.get(), filter);
        std::ranges::sort(items,
                          [&](Item& lhs, Item& rhs) {
            
            if (lhs.getTime() < rhs.getTime() ) {
                return true;
            } else if (lhs.getTime() > rhs.getTime() ) {
                return false;
            }
            
            const auto lhsIsTemporal = lhs.is<Temporal>();
            const auto rhsIsTemporal = rhs.is<Temporal>();
            
            if (lhsIsTemporal && !rhsIsTemporal) {
                return true;
            } else if (!lhsIsTemporal && rhsIsTemporal) {
                return false;
            } else if (lhsIsTemporal && rhsIsTemporal) {
                auto& l = lhs.as<Temporal>();
                auto& r = rhs.as<Temporal>();
                
                if (l.getIsChord() && !r.getIsChord()) {
                    return true;
                } else if (!l.getIsChord() && r.getIsChord()) {
                    return false;
                } else {
                    
                    if (l.getSlot() < r.getSlot()) {
                        return true;
                    } else if (l.getSlot() > r.getSlot()) {
                        return false;
                    }
                    
                    const auto lpitch = l.getPitch();
                    const auto rpitch = r.getPitch();
                    
                    if (lpitch.getMidiNote() < rpitch.getMidiNote()) {
                        return true;
                    } else if (lpitch.getMidiNote() > rpitch.getMidiNote()) {
                        return false;
                    }
                    
                    if (l.getX() < r.getX()) {
                        return true;
                    } else if (l.getX() > r.getX()) {
                        return false;
                    }
                }
            }
            
            return &lhs < &rhs;
        });

        return items;
    }

    template <typename T>
inline std::vector<std::reference_wrapper<T>> getItems(const komp::Score& inScore,
                                                  int measureIndex,
                                                  int trackIndex,
                                                  int handIndex)
    {
        using namespace komp;
        const auto voices = MAX_VOICES;
        std::vector<std::reference_wrapper<T>> items;

        for (int voiceIndex = 0; voiceIndex < voices; ++voiceIndex) {
            auto temp = getItems<T>(inScore,
                                    measureIndex,
                                    trackIndex,
                                    handIndex,
                                    voiceIndex);
            std::copy(temp.begin(), temp.end(), std::back_inserter(items));
        }

        return items;
    }

    template <typename T>
    inline std::vector<std::reference_wrapper<T>> getItems(const komp::Score& inScore,
                                                    int measureIndex,
                                                    int trackIndex)
    {
        using namespace komp;
        KOMP_ASSERT(trackIndex >= 0)
        KOMP_ASSERT(trackIndex < static_cast<int>(inScore.getTracks().size()));
        const auto& track = inScore.getTracks().at(trackIndex);
        KOMP_ASSERT(track);
        const auto hands = track->getHands();
        std::vector<std::reference_wrapper<T>> items;

        for (int handIndex = 0; handIndex < hands; ++handIndex) {
            auto temp = getItems<T>(inScore, measureIndex, trackIndex, handIndex);
            std::copy(temp.begin(), temp.end(), std::back_inserter(items));
        }

        return items;
    }

    template <typename T>
    inline std::vector<std::reference_wrapper<T>> getItems(komp::Score& inScore,
                                                    int measureIndex)
    {
        using namespace komp;
        std::vector<std::reference_wrapper<T>> items;
        
        const auto trackCount = inScore.getTracks().size();
        for (int trackIndex = 0; trackIndex < trackCount; ++trackIndex) {
            auto temp = getItems<T>(inScore, measureIndex, trackIndex);
            std::copy(temp.begin(), temp.end(), std::back_inserter(items));
        }

        return items;
    }

    inline std::string saveScore(const komp::ScoreUPtr& inScore)
    {
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*inScore.get()};
        fm->saveStream(fparams, ss);
        return ss.str();
    }

    inline ScoreUPtr readScore(const std::string& inXml)
    {
        std::istringstream iss{inXml};
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        return fm->loadStream(iss);
    }

    inline ScoreUPtr roundTripScore(const komp::ScoreUPtr& inScore)
    {
        auto xml = saveScore(inScore);
        //std::cout << xml << std::endl << std::endl;
        return readScore(xml);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
