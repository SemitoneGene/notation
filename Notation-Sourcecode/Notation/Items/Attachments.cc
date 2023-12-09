// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Attachments.h"

// Local
#include "Curve.h"
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Mark.h"
#include "Measure.h"
#include "Note.h"
#include "RelativePoint.h"
#include "Temporal.h"
#include "Tie.h"

namespace komp
{

Attachments::Attachments(Item& inItem)
: mTemporal{inItem}
, mIncomingCurves{}
, mOutgoingCurves{}
, mMarks{}
{
    
}


void
Attachments::setNeedsDisplay()
{
    for (Curve& curve : mIncomingCurves) {
        ItemImpl::setNeedsDisplay(curve);
    }
    
    for (Curve& curve : mOutgoingCurves) {
        ItemImpl::setNeedsDisplay(curve);
    }
    
    for (Mark& mark : mMarks) {
        ItemImpl::setNeedsDisplay(mark);
    }
}

void
Attachments::clearMarks()
{
    for (Mark& mark : mMarks) {
        auto& measure = mark.getLocation().getMeasure();
        measure.removeItem(mark);
    }
    mMarks.clear();
}


const std::vector<MarkRef>
Attachments::getMarks() const
{
    return mMarks;
}


std::vector<MarkRef>
Attachments::getMarks()
{
    return mMarks;
}


Mark&
Attachments::addMark(ItemRegistry& inRegistry,
                     Temporal& inTemporal,
                     MarkType inMarkType,
                     PlacementType inPlacementType)
{
    KOMP_ASSERT(inMarkType != MarkType::NONE);
    
    const RelativePoint spatialPoint(inTemporal);
    
    const auto& loc = inTemporal.getLocation();
    auto& markNew = inRegistry.createItem<Mark>(loc,
                                                inMarkType,
                                                inPlacementType,
                                                spatialPoint);
    markNew.setIsQuiescent(false);
    auto& measure = inTemporal.getLocation().getMeasure();
    measure.insertItem(markNew, markNew.getTime());
    mMarks.emplace_back(markNew);
    
    const MarkData& data = markNew.getMarkData();
    
    PlacementType placement = inPlacementType;
    if (data.getPlacementType() == PlacementType::AUTOMATIC) {
        // ??
    } else {
        placement = data.getPlacementType();
    }
    
    const auto markCategory = data.getMarkCategory();
    switch (markCategory)
    {
    case MarkCategory::ARTICULATION:
    case MarkCategory::DYNAMIC:
    case MarkCategory::EXPRESSION:
    case MarkCategory::FERMATA:
    case MarkCategory::ORNAMENT:
    case MarkCategory::TECHNIQUE:
        {
            const auto& markPlacement = data.getMarkPlacement();
            markPlacement->placeMark(markNew, placement);
            return markNew;
        }
        
    default:
        //LOG_F(INFO, "Unhandled attachment type");
        break;
    }
    
    const auto markIsRelativeToID = markNew.getRelativePoint().getItem().getItemID();
    KOMP_ASSERT(markIsRelativeToID == inTemporal.getItemID());
    return markNew;
}


void
Attachments::addMark(ItemRegistry& inRegistry,
                     Temporal& inTemporal,
                     Mark& inMark)
{
    const auto& point = inMark.getRelativePoint();
    const auto markIsRelativeToID = point.getItem().getItemID();
    KOMP_ASSERT(markIsRelativeToID == inTemporal.getItemID());
    mMarks.emplace_back(inMark);
}


void
Attachments::removeMark(Mark& inMark)
{
    auto filter = [&](MarkRef mark) {
        return &mark.get() == &inMark;
    };
    
    auto iter = std::find_if(mMarks.begin(),
                             mMarks.end(),
                             filter);
    
    if (iter != mMarks.end()) {
        mMarks.erase(iter);
    }
}


bool
Attachments::hasCurve() const
{
    return !mIncomingCurves.empty() || !mOutgoingCurves.empty();
}


bool
Attachments::hasIncomingCurve() const
{
    return !mIncomingCurves.empty();
}


bool
Attachments::hasOutgoingCurve() const
{
    return !mOutgoingCurves.empty();
}


void
Attachments::addIncomingCurve(Curve& inCurve)
{
    const auto iter = std::find_if(mIncomingCurves.cbegin(),
                                   mIncomingCurves.cend(),
                                   [&](const CurveRef& curve) {
        return &curve.get() == &inCurve;
    });
    
    if (iter != mIncomingCurves.cend()) {
        LOG_F(WARNING, "Incoming curve already set");
    } else {
        mIncomingCurves.push_back(inCurve);
    }
}


void
Attachments::removeIncomingCurve(Curve& inCurve)
{
    const auto iter = std::find_if(mIncomingCurves.cbegin(),
                                   mIncomingCurves.cend(),
                                   [&](const CurveRef& curve) {
        return &curve.get() == &inCurve;
    });
    if (iter == mIncomingCurves.cend()) {
        LOG_F(WARNING, "Unable to find outgoing curve");
    } else {
        mIncomingCurves.erase(iter);
    }
}


void
Attachments::addOutgoingCurve(Curve& inCurve)
{
    const auto iter = std::find_if(mOutgoingCurves.cbegin(),
                                   mOutgoingCurves.cend(),
                                   [&](const CurveRef& curve) {
        return &curve.get() == &inCurve;
    });
    
    if (iter != mOutgoingCurves.cend()) {
        LOG_F(WARNING, "Outgoing curve already set");
    } else {
        mOutgoingCurves.push_back(inCurve);
    }
}


void
Attachments::removeOutgoingCurve(Curve& inCurve)
{
    const auto iter = std::find_if(mOutgoingCurves.cbegin(),
                                   mOutgoingCurves.cend(),
                                   [&](const CurveRef& curve) {
        return &curve.get() == &inCurve;
    });
    if (iter == mOutgoingCurves.cend()) {
        LOG_F(WARNING, "Unable to find outgoing curve");
    } else {
        mOutgoingCurves.erase(iter);
    }
}


const std::vector<CurveRef>&
Attachments::getIncomingCurves()
{
    return mIncomingCurves;
}


const std::vector<CurveRef>&
Attachments::getOutgoingCurves()
{
    return mOutgoingCurves;
}


const std::vector<CurveRef>&
Attachments::getIncomingCurves() const
{
    return mIncomingCurves;
}


const std::vector<CurveRef>&
Attachments::getOutgoingCurves() const
{
    return mOutgoingCurves;
}


TemporalRefVec
Attachments::getTieChain() const
{
    TemporalRefVec temporals;
    
    for (const Curve& curve : mOutgoingCurves) {
        
        if (curve.getItemTraits().getItemType() == ItemType::Tie) {
            
            // Get the item the curve is attached to
            auto& temporal = curve.getEndTemporal();
            if (temporal.getItemTraits().getItemType() == ItemType::Note) {
                
                // Add the temporal to the tie chain
                temporals.emplace_back(temporal);
            }
        }
    }
    
    return temporals;
}

Rect
Attachments::getTechniquesFrame(const Point& inPoint) const
{
    Rect result;
    
    for (Mark& mark : mMarks) {
        const auto frame = mark.getTechniquesFrame(inPoint);
        result.initializeOrUnion(frame);
    }
    return result;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
