// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "KeySignatureType.h"

namespace komp
{
    /// Changes the key signature (unless the new key signature is equal to the old)
    /// and updates the pitch of any affected notes.  Stops processing if a future
    /// key signature is encountered
    ///
    class KeySignatureCommand : public Command
    {
    public:
        virtual ~KeySignatureCommand();
        KeySignatureCommand(CommandFactory& inFactory,
                            ItemRegistry& inRegistry,
                            Score& inScore,
                            Measure& inFirstMeasure,
                            KeySignatureType inNewKeySignature);

        virtual void undo() override;
        virtual void redo() override;

    private:
        
        void changeKey(Measure& inFirstMeasure,
                       Measure* inLastMeasure,
                       KeySignatureType inNewKey) const;

        virtual CommandResult executeImpl() override;
        
        Score& mScore;
        Measure& mFirstMeasure;
        Measure* mLastMeasure;
        KeySignatureType mNewKeySignature;
        KeySignatureType mOldKeySignature;

        KeySignatureType getCurrentKeySignature() const;
        Measure* getLastMeasure() const;

    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
