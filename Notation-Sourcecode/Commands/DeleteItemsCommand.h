// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"

namespace komp
{
    /// \brief  handles any collection of items and deletes
    /// them in the proper way
    ///
    class DeleteItemsCommand : public Command
    {
    public:
        DeleteItemsCommand(CommandFactory& inFactory,
                           ItemRegistry& inRegistry,
                           Score& inScore,
                           const ItemRefVec& inItems);
        
        virtual ~DeleteItemsCommand() = default;
        
        virtual void undo() override;
        virtual void redo() override;
        
        virtual void addSubCommand(CommandIntfUPtr inCommand) override;
        
    private:
        // ItemIsDeleted.first -> ItemID
        // ItemIsDeleted.second -> is the item already deleted?
        using ItemIDBool = std::pair<ItemID, bool>;
        using ItemIDBools = std::vector<ItemIDBool>;
        using ItemBool = std::pair<ItemRef, bool>;
        using ItemBools = std::vector<ItemBool>;
        
        std::vector<ItemIDBool> mItems;
        ItemBools getItems() const;
        
        void deleteBeam(const BeamIntf& inBeam, ItemBools& outItems);
        void deleteBeamedNotes(const Temporal& inNote, ItemBools& outItems, bool& ioDeleted);
        void deleteCurve(const Curve& inCurve, ItemBools& outItems);
        void deleteChord(const Chord& inChord, ItemBools& outItems, bool& ioDeleted);
        void deleteChordedNote(const Note& inChordedNote, ItemBools& outItems, bool& ioDeleted);
        void deleteItem(const Item& inItem, ItemBools& outItems, bool& ioDeleted);
        void deleteMark(const Mark& inMark, ItemBools& outItems, bool& ioDeleted);
        void deleteTemporal(const Temporal& inTemporal, ItemBools& outItems, bool& ioDeleted);
        void deleteTuplet(const TupletIntf& inTuplet, ItemBools& outItems);
        void setItemDeleted(const Item& inItem, ItemBools& outItems) const;
        virtual CommandResult executeImpl() override;
        
        Score& mScore;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
