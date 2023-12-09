// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "KeySignatureCommand.h"

// Local
#include "ItemImpl.h"
#include "KeySignature.h"
#include "KeySignatureType.h"
#include "Measure.h"
#include "Note.h"
#include "Score.h"
#include "Temporal.h"
#include "Track.h"


namespace komp
{
    KeySignatureCommand::~KeySignatureCommand() = default;

    KeySignatureCommand::KeySignatureCommand(CommandFactory& inFactory,
                                             ItemRegistry& inRegistry,
                                             Score& inScore,
                                             Measure& inFirstMeasure,
                                             KeySignatureType inNewKeySignature)
    : Command("Change Key Signature", inFactory, inRegistry)
    , mScore(inScore)
    , mFirstMeasure(inFirstMeasure)
    , mLastMeasure(nullptr)
    , mNewKeySignature(inNewKeySignature)
    , mOldKeySignature(KeySignatureType::C)
    {

    }


    CommandResult
    KeySignatureCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        mOldKeySignature = getCurrentKeySignature();

        if (mOldKeySignature == mNewKeySignature) {
            return false;
        }

        mLastMeasure = getLastMeasure();
        changeKey(mFirstMeasure, mLastMeasure, mNewKeySignature);
        
        return true;
    }


    void
    KeySignatureCommand::undo()
    {
        KOMP_LOG_COMMAND
        if (mNewKeySignature == mOldKeySignature) {
            return;
        }
        KOMP_ASSERT(mLastMeasure);
        changeKey(mFirstMeasure, mLastMeasure, mOldKeySignature);
    }


    void
    KeySignatureCommand::redo()
    {
        KOMP_LOG_COMMAND
        execute();
    }


    KeySignatureType
    KeySignatureCommand::getCurrentKeySignature() const
    {
        const auto& tracks = mScore.getTracks();
        KOMP_ASSERT(tracks.size() > 0);
        const MusicLocation loc(mFirstMeasure,
                                *tracks.front().get());
        return loc.getKeySignature();
    }


    Measure*
    KeySignatureCommand::getLastMeasure() const
    {
        auto miter = mFirstMeasure.iter();
        auto mprevious = miter;
        const auto e = mScore.getMeasuresEnd();
        for ( ; miter != e; ++miter) {
            const auto& keys = (*miter)->items<KeySignature>();
            if (!keys.empty()) {
                break;
            }
            mprevious = miter;
        }

        if (mprevious != e) {
            return (*mprevious).get();
        } else {
            return mScore.getMeasuresBack().get();
        }
    }


    void
    KeySignatureCommand::changeKey(Measure& inFirstMeasure,
                                   Measure* inLastMeasure,
                                   KeySignatureType inNewKey) const
    {
        
        auto miter = inFirstMeasure.iter();
        const auto e = mScore.getMeasuresEnd();
        const KeySignatureValue keyValue{inNewKey};

        for ( ; miter != e; ++miter) {
            
            const auto& measure = *miter;
            const auto& keys = measure->items<KeySignature>();
            
            for (KeySignature& key : keys) {
                measure->removeItem(key);
            }
            
            if (measure.get() == inLastMeasure) {
                break;
            }
        }

        // After deleting keys, if we arrive at the desired key we don't
        // need to insert any key signature objects. check for this condition
        const auto currentKey = getCurrentKeySignature();

        // Reset the iterator
        miter = inFirstMeasure.iter();

        for ( ; miter != e; ++miter) {

            for (const auto& track : mScore.getTracks()) {
                for (int hand = 0; hand < track->getHands(); ++hand) {

                    auto& measure = *miter;

                    if (measure.get() == &inFirstMeasure
                     && currentKey != inNewKey) {
                        
                        const MusicLocation keyLocation(*measure.get(),
                                                        *track,
                                                        hand);
                        auto& keyPtr = mRegistry.createItem<KeySignature>(keyLocation,
                                                                          keyValue);
                        measure->insertItem(keyPtr, 0.0);
                        
                        ItemImpl::setNeedsDisplay(keyPtr);
                    }

                    const auto& notes = measure->items<Note>(*track.get(), hand);
                    for (Note& note : notes) {
                        note.deducePitch();
                    }
                }
            }

            if ((*miter).get() == inLastMeasure) {
                break;
            }
        }

        // Call layout as a key signature change will effect horizontal spacing
        mScore.layout();
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
