// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Score.h"
#include "Measure.h"
#include "Track.h"
#include "Temporal.h"
#include "FileMediatorFactory.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace komp
{
    constexpr const char* const fileName = "Bombe.xml";

    inline komp::ScoreUPtr loadBombe()
    {
        using namespace komp;
        return komp::TestFileRepository::loadScore(fileName);
    }

    inline std::string saveAnyScore(const komp::ScoreUPtr& inScore)
    {
        using namespace komp;
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        FileSaveParams fparams{*inScore.get()};
        fm->saveStream(fparams, ss);
        std::string xml{ss.str()};
        return xml;
    }

    inline std::string saveBombe()
    {
        using namespace komp;
        return saveAnyScore(loadBombe());
    }

    inline komp::ScoreUPtr roundTripBombe()
    {
        using namespace komp;
        const std::string xml = saveBombe();
        std::istringstream iss{xml};
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        return fm->loadStream(iss);
    }

    struct BombeFuzz
    {
        const std::vector<int> measureFuzz = {0, 0, 1, 1, 2, 3, 2, 1, 1, 0, 0, 3, 2, 3, 3, 1, 1, 3, 3, 2, 0};
        const std::vector<int> trackHandFuzz = {1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1};
        const std::vector<int> itemFuzz = {13, 2, 31, 23, 34, 34, 18, 31, 16, 18, 3, 15, 10, 22, 33, 2, 23, 20, 26, 15, 3, 2, 27, 20, 19, 17, 22, 7, 30, 11, 3, 4, 13, 17, 2, 7, 25, 34, 18, 2, 11, 22, 2, 12, 11, 4, 31, 8, 17, 34, 18, 15, 4, 11, 33, 15, 34, 32, 26, 28, 12, 31, 23, 11, 3, 18, 19, 9, 28, 30, 7, 32, 25, 13, 13, 32, 31, 2, 23, 18, 28, 34, 0, 1, 18, 34, 26, 5, 29, 12, 0, 14, 33, 3, 20, 17, 26, 23, 20, 33, 32};
        ScoreUPtr score;
        ItemRefVec mementos;
        int counter;
        CommandIntfUPtr deleteItemsCommand;

        static inline ItemRefVec retreiveAllItemsFromScore(Score* inScore)
        {
            ItemRefVec result;
            auto m = inScore->getMeasuresBegin();
            const auto e = inScore->getMeasuresEnd();

            for ( ; m != e; ++m) {
                auto&& measure = *m;
                auto&& measureItems = measure->allItems();

                for (auto&& item : measureItems) {
                    result.emplace_back(item);
                }
            }
            return result;
        }


        inline void initializeScore()
        {
            this->score = loadBombe();
            this->counter = 0;
            this->mementos.clear();
            this->deleteItemsCommand = nullptr;

            auto allItemsFromScore = retreiveAllItemsFromScore(this->score.get());
            for (Item& item : allItemsFromScore) {
                mementos.emplace_back(item);
            }

            komp::sortItems(mementos);
        }


        inline void runDeleteItemsCommand(Score& inScore,
                                          int numItems)
        {
            std::set<Item*> uniqueItemSet;

            for (int i = 0; i < numItems; ++i) {
                const int measureIndex = measureFuzz.at(counter++ % static_cast<int>(measureFuzz.size()));
                const int trackIndex = trackHandFuzz.at(counter++ % static_cast<int>(trackHandFuzz.size()));
                const int handIndex = trackHandFuzz.at(counter++ % static_cast<int>(trackHandFuzz.size()));

                auto&& measure = score->getMeasure(measureIndex);
                auto&& track = score->getTracks().at(trackIndex);

                auto filter = [&](Item& inFilterItem) {
                    
                    const auto& loc = inFilterItem.getLocation();
                    
                    if (loc.getHand() != handIndex) {
                        return false;
                    }

                    if (&loc.getTrack() != track.get()) {
                        return false;
                    }

                    return true;
                };

                auto items = measure->items<Item>(*track.get(), filter);
                komp::sortItems(items);
                auto itemIndex = counter++ % static_cast<int>(itemFuzz.size());
                itemIndex = itemIndex % static_cast<int>(items.size());
                Item& item = items.at(itemIndex);
                uniqueItemSet.insert(&item);
            }
            ItemRefVec itemsToDelete;
            for (const auto& item : uniqueItemSet) {
                itemsToDelete.push_back(*item);
            }

            deleteItemsCommand = score->getCommandFactory().deleteItemsCommand(itemsToDelete);
            deleteItemsCommand->execute();
        }

    };

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
