// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "ScoreCreationInfo.h"

namespace komp
{
    void
    ScoreInfo::deduceBrackets(const std::unordered_map<komp::SoundID,
                              komp::Instrument>& inInstruments)
    {
        if (trackInfoList.empty() || inInstruments.empty()) {
            return;
        }
        
        auto emplaceAndClear = [&](std::unique_ptr<SystemBracketInfo>& ioBracketInfo, int inEndTrackIndex) {
            if (ioBracketInfo) {
                ioBracketInfo->trackEnd = inEndTrackIndex;
                if (ioBracketInfo->trackStart <= ioBracketInfo->trackEnd) {
                    systemBracketInfoList.push_back(*ioBracketInfo);
                }
                ioBracketInfo = nullptr;
            }
        };

        systemBracketInfoList.clear();
        const auto instEnd = inInstruments.cend();

        const auto foundIter = inInstruments.find(trackInfoList.front().getSoundID());
        Instrument info;
        if (foundIter == instEnd) {
            info.name = "error not found";
            info.family = "notfound";
        } else {
            info = foundIter->second;
        }
            
        std::string previousFamily = info.family;
        std::unique_ptr<SystemBracketInfo> currentBracket = nullptr;
        int trackIndex = 0;

        for (const auto& trackInfo : trackInfoList) {
            
            const auto instrumentIter = inInstruments.find(trackInfo.getSoundID());
            KOMP_ASSERT(instrumentIter != instEnd);
            const auto instrument = instrumentIter->second;

            if (instrument.instrumentBracket == GroupBracketType::PianoBrace) {
                emplaceAndClear(currentBracket, trackIndex - 1);
                SystemBracketInfo pianoBrace;
                pianoBrace.trackStart = trackIndex;
                pianoBrace.trackEnd = trackIndex;
                pianoBrace.type = SystemBracketType::BRACE;
                systemBracketInfoList.push_back(pianoBrace);
            } else if ((instrument.family != previousFamily) ||
                       (instrument.startingClefs.size() > 1)) {

                emplaceAndClear(currentBracket, trackIndex - 1);
                currentBracket = nullptr;
            }

            if (!currentBracket) {
                if (instrument.startingClefs.size() == 1) {
                    currentBracket = std::make_unique<SystemBracketInfo>();
                    currentBracket->trackStart = trackIndex;
                    currentBracket->trackEnd = -1;
                    currentBracket->type = SystemBracketType::BRACKET;
                }
            }

            previousFamily = instrument.family;
            ++trackIndex;
        }
        emplaceAndClear(currentBracket, trackIndex - 1);
    }
}
