// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Clipboard.h"

// Local
#include "CommandFactory.h"
#include "Item.h"
#include "Score.h"
#include "Selection.h"
#include "UndoManager.h"


namespace komp
{
Clipboard::Clipboard(CommandFactory& inFactory)
: mFactory(inFactory)
, mOrigin{nullptr}
{
    clear();
}

void
Clipboard::cut(const ClipboardRange& inRange)
{
}

void
Clipboard::copy(const ClipboardRange& inRange)
{
    mOrigin = std::make_unique<PasteRangeContext>(*inRange.startMeasure,
                                                  *inRange.startStaff,
                                                  *inRange.endMeasure,
                                                  *inRange.endStaff);
}

void
Clipboard::paste(Score& inScore, const PasteItemsContext& inContext)
{
    auto command = mFactory.clipboardPasteItemsCommand(std::move(inContext));
    command->execute();
    inScore.getUndoManager().addCommand(std::move(command));
}

void
Clipboard::paste(Score& inScore, const PasteRangeContext& inDestination)
{
    PasteRangeParams params {
        {
            mOrigin->startMeasure,
            mOrigin->startStaff,
            mOrigin->endMeasure,
            mOrigin->endStaff
        },
        {
            inDestination.startMeasure,
            inDestination.startStaff,
            inDestination.endMeasure,
            inDestination.endStaff
        }
    };
    
    auto command = mFactory.clipboardPasteRangeCommand(std::move(params));
    command->execute();
    inScore.getUndoManager().addCommand(std::move(command));
}

void
Clipboard::clear()
{
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
