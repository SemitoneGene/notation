// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertChordSymbolCommand.h"

// Local
#include "ChordSymbol.h"
#include "Score.h"

namespace komp
{
    InsertChordSymbolCommand::~InsertChordSymbolCommand() = default;

    InsertChordSymbolCommand::InsertChordSymbolCommand(CommandFactory& inFactory,
                                                       ItemRegistry& inRegistry,
                                                       Measure& inMeasure,
                                                       const ChordSymbolParams& inParams,
                                                       bool inDoQuantize)
    : Command("Insert Chord Symbol", inFactory, inRegistry)
    , mParams{inParams}
    , mItemID(initializeItemID())
    , mMeasure{inMeasure}
    , mDoQuantize{inDoQuantize}
    {
        if (mDoQuantize) {
            mParams.time = mMeasure.roundTimeToNearestBeat(mParams.time);
        }
    }


    CommandResult
    InsertChordSymbolCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        const auto& score = mMeasure.getScore();

        if (!mParams.isSlotSpecified) {

            // Find the slot of any existing chord symbols in the same
            // track/hand and match the new chord symbol's slot to them
            for (const auto& measure : score.getMeasures()) {
                const auto filterLambda = [&](Item& inItem) {
                    
                    const auto& loc = inItem.getLocation();
                    if (loc.getHand() != mParams.hand) {
                        return false;
                    }

                    if (&loc.getTrack() != &mParams.track) {
                        return false;
                    }

                    return true;
                };

                const auto& chordSymbols = measure->items<ChordSymbol>(mParams.track,
                                                                       filterLambda);

                if (!chordSymbols.empty()) {
                    mParams.slot = chordSymbols.front().get().getSlot();
                    mParams.isSlotSpecified = true;
                }
            }
        }

        const MusicLocation loc(mMeasure, mParams.track);
        auto& chordSymbol = mRegistry.createItem<ChordSymbol>(loc, mParams);
        mMeasure.insertItem(chordSymbol, mParams.time);
        mItemID = chordSymbol.getItemID();
        return true;
    }


    void
    InsertChordSymbolCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mItemID);
        mMeasure.removeItem(item);
        mMeasure.draw();
    }


    void
    InsertChordSymbolCommand::redo()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mItemID);
        mMeasure.insertItem(item);
        mMeasure.draw();
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
