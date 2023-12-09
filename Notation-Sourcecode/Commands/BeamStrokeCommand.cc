// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeamStrokeCommand.h"

// Local
#include "AdditionalBeam.h"
#include "HalveDuration.h"
#include "Throw.h"
#include "InsertBeamCommand.h"
#include "ItemRegistry.h"
#include "MakeDurationsBeamable.h"
#include "Note.h"
#include "BeamFactory.h"


namespace komp
{
    BeamStrokeCommand::~BeamStrokeCommand() = default;
    
    BeamStrokeCommand::BeamStrokeCommand(CommandFactory& inFactory,
                                         ItemRegistry& inRegistry,
                                         const BeamStrokeParams& inData)
    : komp::Command{"Beam Stroke", inFactory, inRegistry}
    , mData{std::move(inData)}
    {
        
    }
    
    CommandResult
    BeamStrokeCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        const auto temporals = mData.getTemporals();

        if (temporals.empty()) {
            LOG_F(WARNING, "called with empty temporals vector");
            return false;
        }
        if (mData.isAdditionalBeam) {
            handleAdditionalBeam();
            fixTimes();
            return true;
        } else if (mData.isConnectionBeam || mData.isInitialBeam) {
            handleConnectingBeam();
            fixTimes();
            return true;
        } else if (mData.isFractionalBeam) {
            handleFractionalBeam();
            fixTimes();
            return true;
        } else if (mData.isHalvingBeam) {
            handleHalvingBeam();
            fixTimes();
            return true;
        } else {
            LOG_F(INFO, "Beam stroke intent was not ascertained.");
        }
        return false;
    }
    
    
    void
    BeamStrokeCommand::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }
    
    
    void
    BeamStrokeCommand::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
    }


    void
    BeamStrokeCommand::handleAdditionalBeam()
    {
        auto additionalBeamCmd = std::make_unique<AdditionalBeam>(mFactory,
                                                                  mRegistry,
                                                                  mData);
        additionalBeamCmd->execute();
        addSubCommand(std::move(additionalBeamCmd));
    }
    

    void
    BeamStrokeCommand::handleConnectingBeam()
    {
        auto temporals = mData.getTemporals();
        std::vector<ItemID> ids;
        for (const Temporal& temporal : temporals) {
            ids.push_back(temporal.getItemID());
        }
        
        CommandIntfUPtr insertBeamCmd;
        if (mData.isExistingStemDirectionDeterministic) {
            insertBeamCmd = std::make_unique<InsertBeamCommand>(mFactory,
                                                                mRegistry,
                                                                ids,
                                                                mData.stemDirection);
        } else {
            insertBeamCmd = std::make_unique<InsertBeamCommand>(mFactory,
                                                                mRegistry,
                                                                ids);
        }
        insertBeamCmd->execute();
        addSubCommand(std::move(insertBeamCmd));
    }


    void
    BeamStrokeCommand::handleHalvingBeam()
    {
        auto temporals = mData.getTemporals();
        TemporalRefVec rebeamables;
        BeamIntfRefVec beams;
        getRebeamables(rebeamables, beams);

        for (BeamIntf& beam : beams) {
            auto deleteBeam = mFactory.deleteBeamCommand(beam);
            deleteBeam->execute();
            addSubCommand(std::move(deleteBeam));
        }

        auto halveTemporals = std::make_unique<HalveDuration>(mFactory,
                                                              mRegistry,
                                                              temporals);
        halveTemporals->execute();
        addSubCommand(std::move(halveTemporals));

        std::vector<ItemID> ids;
        for (const Temporal& temporal : rebeamables) {
            ids.push_back(temporal.getItemID());
        }

        CommandIntfUPtr insertBeamCmd;
        if (mData.isExistingStemDirectionDeterministic) {
            insertBeamCmd = std::make_unique<InsertBeamCommand>(mFactory,
                                                                mRegistry,
                                                                ids,
                                                                mData.stemDirection);
        } else {
            insertBeamCmd = std::make_unique<InsertBeamCommand>(mFactory,
                                                                mRegistry,
                                                                ids);
        }
        insertBeamCmd->execute();
        addSubCommand(std::move(insertBeamCmd));
    }
    

    void
    BeamStrokeCommand::handleFractionalBeam()
    {
        handleHalvingBeam();
    }
    
    
    void
    BeamStrokeCommand::getRebeamables(TemporalRefVec& outRebeamables,
                                      BeamIntfRefVec& outBeams)
    {
        std::set<Temporal*> uniqueTemporals;
        std::set<BeamIntf*> uniqueBeams;

        for (Temporal& t : mData.getTemporals()) {
            
            Temporal* temporal = &t;
            if (temporal->getIsChordMember()) {
                temporal = temporal->getChord();
                KOMP_ASSERT(temporal);
            }
            uniqueTemporals.emplace(temporal);
            const auto beam = temporal->getBeam();
            if (beam) {
                uniqueBeams.emplace(beam);
                for (const auto& beamedNote : beam->getBeamedNotes()) {
                    auto& note = beamedNote->getNote();
                    uniqueTemporals.emplace(&note);
                }
            }
        }

        for (auto& t : uniqueTemporals) {
            outRebeamables.emplace_back(*t);
        }

        for (auto& b : uniqueBeams) {
            outBeams.emplace_back(*b);
        }
    }


    void
    BeamStrokeCommand::fixTimes()
    {
        auto& measure = mData.getMeasure();
        auto fixTimesCommand = mFactory.fixTimesCommand(measure);
        fixTimesCommand->execute();
        addSubCommand(std::move(fixTimesCommand));
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
