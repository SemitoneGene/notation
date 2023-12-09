// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"
#include "StemDirection.h"

namespace komp
{
    class InsertBeamCommand : public Command
    {
    public:
        InsertBeamCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          const std::vector<ItemID>& inItems);
        
        InsertBeamCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          const std::vector<ItemID>& inItems,
                          StemDirection inStemDirection);

        virtual ~InsertBeamCommand() = default;

        virtual void undo() override;
        virtual void redo() override;

    private:
        void populateSets(std::set<Temporal*>& beamables,
                          std::set<Note*>& notes,
                          std::set<Rest*>& rests,
                          std::set<Chord*>& chords,
                          std::set<BeamIntf*>& beams) const;

        void validate(const std::set<Temporal*>& beamables) const;

        void createMementos(const std::set<Temporal*>& beamables,
                            const std::set<Note*>& notes,
                            const std::set<Rest*>& rests,
                            const std::set<Chord*>& chords,
                            const std::set<BeamIntf*>& beams);

        void deleteBeams(const std::set<BeamIntf*>& beams);

        void processBeam(BeamIntf* inBeam,
                         std::set<Temporal*>& ioBeamables,
                         std::set<Note*>& ioNotes,
                         std::set<Rest*>& ioRests,
                         std::set<Chord*>& ioChords,
                         std::set<BeamIntf*>& ioBeams) const;

        void processTemporal(Temporal& inTemporal,
                             std::set<Temporal*>& ioBeamables,
                             std::set<Note*>& ioNotes,
                             std::set<Rest*>& ioRests,
                             std::set<Chord*>& ioChords,
                             std::set<BeamIntf*>& ioBeams) const;

        void correctTemporalTypes(const std::set<Temporal*>& beamables);

        void insertBeam(const std::set<Temporal*>& beamables);
        virtual CommandResult executeImpl() override;
        
        std::set<ItemID> mItemIDs;
        bool mIsExecuteCalled{false};
        bool mIsUndoCalled{false};
        ItemID mBeamID;
        CommandIntfUPtr mDeleteBeamCommand{nullptr};
        bool mIsStemDirectionGiven{false};
        StemDirection mStemDirection{StemDirection::NONE};
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
