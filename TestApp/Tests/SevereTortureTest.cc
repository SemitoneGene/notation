// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Score.h"
#include "UndoManager.h"

// Test Support
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace
{
    struct Node
    {
        std::unique_ptr<Node> next;
        int value;
    };

    class Generator
    {
    public:
        Generator()
        : myHead{}
        , myCurrent{ nullptr }
        {
            myHead.value = 1;
            myCurrent = &myHead;
            addValue(5);
            addValue(11);
            addValue(13);
            addValue(19);
            addValue(23);
            addValue(29);
            addValue(17);
            addValue(31);
            addValue(37);
            addValue(41);
            addValue(43);
            addValue(199);
            addValue(59);
            addValue(73);
            addValue(53);
            addValue(61);
            addValue(67);
            addValue(71);
            addValue(101);
            addValue(79);
            addValue(89);
            addValue(97);
            addValue(103);
            addValue(107);
            addValue(47);
            addValue(109);
            addValue(131);
            addValue(113);
            addValue(127);
            addValue(149);
            addValue(137);
            addValue(83);
            addValue(139);
            addValue(7);
            addValue(151);
            addValue(163);
            addValue(157);
            addValue(167);
            addValue(3);
            addValue(181);
            addValue(197);
            addValue(173);
            addValue(2);
            addValue(179);
            addValue(191);
            addValue(193);
        }

        int get()
        {
            if (!myCurrent || !myCurrent->next.get() )
            {
                myCurrent = &myHead;
            }
            else
            {
                myCurrent = myCurrent->next.get();
            }

            return myCurrent->value;
        }

    private:
        Node myHead;
        Node* myCurrent;
        void addValue(int inValue)
        {
            myCurrent->next = std::make_unique<Node>();
            myCurrent = myCurrent->next.get();
            myCurrent->value = inValue;
        }
    };

    class ScoreWrapper
    {
    public:
        ScoreWrapper(std::string inFilename)
        : mFilename{std::move(inFilename)}
        , mGenerator{}
        , mScore{nullptr}
        {
            mScore = komp::TestFileRepository::loadScore(mFilename);
        }

        komp::Score* get() const
        {
            return mScore.get();
        }

        bool getIsScoreEmpty() const
        {
            auto mit = mScore->getMeasuresBegin();
            auto mend = mScore->getMeasuresEnd();

            for( ; mit != mend; ++mit )
            {
                const auto& measure = *mit;
                const auto& allItems = measure->allItems();

                if (!allItems.empty() )
                {
                    return false;
                }
            }

            return true;
        }

        komp::Item*
        getRandomItem()
        {
            const auto trackIndex = mGenerator.get() % static_cast<int>( mScore->getTracks().size() ) ;
            const auto measureIndex = mGenerator.get() % mScore->getMeasureCount();
            const auto measureIter = mScore->getMeasuresBegin() + measureIndex;
            const auto& measure = *measureIter;
            const auto& track = mScore->getTracks().at( trackIndex );
            const auto& items = measure->items<komp::Item>(*track.get());

            if (items.empty()) {
                return nullptr;
            }

            const int itemsSize = static_cast<int>( items.size() );
            const int itemIndex = mGenerator.get() % itemsSize;
            auto& item = items.at(itemIndex);
            return &item.get();
        }

    private:
        std::string mFilename;
        Generator mGenerator;
        komp::ScoreUPtr mScore;
    };
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( torture, addValuexyz )
    {

        const std::string filename = "freezing.xml";
        ScoreWrapper s{filename};

        auto& factory = s.get()->getCommandFactory();
        
        auto fm = FileMediatorFactory::create( FileMediatorType::MusicXML );
        
        {
            FileSaveParams params{*s.get()};
            fm->saveFile(params, "./tortureTemp0.xml");

            for (int i = 0; i < 1000; ++i) {
                const auto item = s.getRandomItem();
                if (item) {
                    ItemRefVec v;
                    v.push_back(*item);
                    auto deleteCmd = factory.deleteItemsCommand(v);
                    deleteCmd->execute();
                    s.get()->getUndoManager().addCommand(std::move(deleteCmd));
                }
            }
        }
        
        {
            FileSaveParams params{*s.get()};
            fm->saveFile(params, "./tortureTemp1.xml");

            for( int i = 0; i < 1000; ++i )
            {
                if (!s.get()->getUndoManager().canUndo() )
                {
                    std::cout << "cannot undo" << std::endl;
                }
                s.get()->getUndoManager().undo();
            }
        }
        
        {
            FileSaveParams params{*s.get()};
            fm->saveFile(params, "./tortureTemp2.xml");
        }
    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

