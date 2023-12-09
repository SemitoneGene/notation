// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteCurveCommand.h"

// Local
#include "Curve.h"
#include "Score.h"
#include "Temporal.h"

namespace komp
{

DeleteCurveCommand::DeleteCurveCommand(CommandFactory& inFactory,
                                       ItemRegistry& inRegistry,
                                       ItemID inItemID)
    : Command{"Delete Curve", inFactory, inRegistry}
    , mCurveID{inItemID}
    {

    }
    
    DeleteCurveCommand::~DeleteCurveCommand()
    {
        
    }
    
    CommandResult
    DeleteCurveCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mCurveID);
        KOMP_ASSERT(item.is<Curve>());
        auto& curve = item.as<Curve>();
        
        auto& first = curve.getBeginTemporal();
        auto& last = curve.getEndTemporal();
        
        auto firstMemento = first.getMemento();
        auto lastMemento = last.getMemento();
        
        first.getAttachments().removeOutgoingCurve(curve);
        last.getAttachments().removeIncomingCurve(curve);
        
        auto firstStateCommand = mFactory.itemStateCommand(std::move(firstMemento),
                                                           first.getMemento());
        addSubCommand(std::move(firstStateCommand));
        
        auto lastStateCommand = mFactory.itemStateCommand(std::move(lastMemento),
                                                          last.getMemento());
        addSubCommand(std::move(lastStateCommand));
        
        auto deleteItemCommand = mFactory.deleteItemCommand(mCurveID);
        deleteItemCommand->execute();
        addSubCommand(std::move(deleteItemCommand));
        return true;
    }


    void
    DeleteCurveCommand::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }


    void
    DeleteCurveCommand::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
