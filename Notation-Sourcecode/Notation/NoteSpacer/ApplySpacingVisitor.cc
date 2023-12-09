// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ApplySpacingVisitor.h"

#include <cmath>

// Local
#include "Constants.h"
#include "ChordSymbol.h"
#include "Clef.h"
#include "EventIntf.h"
#include "Sim.h"
#include "Staff.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Throw.h"


namespace komp
{
    ApplySpacingVisitor::ApplySpacingVisitor()
    : mIsVisitCalled{false}
    {

    }


    void
    ApplySpacingVisitor::visit(EventIntf& ioEvent, Sim& ioSim)
    {
        mIsVisitCalled = true;
        const auto x = ioSim.getX();

        auto& item = ioEvent.getItem();
        const auto& itemTraits = item.getItemTraits();
        const auto itemType = itemTraits.getItemType();

        switch (itemType)
        {
        case ItemType::ChordSymbol:
        case ItemType::Clef:
            break;
            
        default:
            item.setX(x);
        }
    }


    void
    ApplySpacingVisitor::notifyBeginVisiting()
    {

    }


    void
    ApplySpacingVisitor::notifyDoneVisiting()
    {

    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
