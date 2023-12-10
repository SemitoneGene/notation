// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp Test
#include "BasicScore.h"
#include "TestNoteFactory.h"

// Komp
#include "Mark.h"
#include "ItemRegistry.h"

namespace komp
{
    inline komp::ScoreUPtr getAttachedItemScore()
    {
        using namespace komp;
        auto score = getBasicScore();
        
        TestNoteFactory noteFactory{*score.get()};
        Note& note = noteFactory.insertNote();
        noteFactory.advancePosition();
        
        RelativePoint sp{note};
        
        auto& reg = score->getItemRegistry();
        const auto& loc = note.getLocation();
        auto& mark = reg.createItem<Mark>(loc,
                                          MarkType::UNSTRESS_ABOVE,
                                          PlacementType::AUTOMATIC,
                                          sp);
        note.setIsQuiescent(false);
        note.getLocation().getMeasure().insertItem(mark, mark.getTime());
        note = noteFactory.insertNote();
        noteFactory.advancePosition();
        note.getAttachments().addMark(reg,
                                       note,
                                       MarkType::ACCENT_BELOW,
                                       PlacementType::BELOW_ITEM);
        note.getAttachments().addMark(reg,
                                      note,
                                      MarkType::DYNAMIC_FFF,
                                      PlacementType::BELOW_STAFF);

        note = noteFactory.insertNote();
        noteFactory.advancePosition();
        
        note = noteFactory.insertNote();
        noteFactory.advancePosition();
        
        score->layout();
        
        return score;
    }

    inline komp::ScoreUPtrVec getAttachedItemScores()
    {
        komp::ScoreUPtrVec outScores;
        outScores.emplace_back(getAttachedItemScore());
        return outScores;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
