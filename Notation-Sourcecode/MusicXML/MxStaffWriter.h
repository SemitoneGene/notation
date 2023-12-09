// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "MxLocation.h"
#include "MxScoreWriter.h"
#include "MxConverter.h"

// Mx
#include "mx/api/StaffData.h"

namespace komp
{
    enum class ClefType;
    
    class Item;
    class Dynamic;
    
    class MxStaffWriter
    {
    public:
        MxStaffWriter(const ItemRefVec& inSortedItems,
                      MxLocation inMxLocation,
                      MxScoreWriter& inMxScoreWriter);
        
        mx::api::StaffData getStaffData();
        
    private:
        const ItemRefVec& mItems;
        MxLocation mMxLocation;
        MxScoreWriter& mMxScoreWriter;
        mx::api::StaffData mOutStaffData;
        MxConverter mConverter;
        std::set<Chord*> mProcessedChords;
        
    private:
        void dispatchItem(Item& item, int& tupletNumberLevel);
        void addStartingClefIfAbsent();
        mx::api::ClefData makeClefData(ClefType clefType, int handIndex, int time);
        void handleWedgeItem(Wedge& inWedge);
        void closeoutWedges();
        void handleChordSymbol(const ChordSymbol& inChordSymbol);
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
