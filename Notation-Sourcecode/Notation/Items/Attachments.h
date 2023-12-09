// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"

namespace komp
{
    class KOMP_EXPORT Attachments
    {
    public:
        Attachments(Item& inItem);

        const std::vector<MarkRef> getMarks() const;
        std::vector<MarkRef> getMarks();

        /// \brief Creates a mark, adds it to the temporal and inserts it into the measure
        Mark& addMark(ItemRegistry& inRegistry,
                      Temporal& inTemporal,
                      MarkType inMarkType,
                      PlacementType inPlacement);

        /// \brief Creates a mark, adds it to the temporal and inserts it into the measure
        void addMark(ItemRegistry& inRegistry,
                     Temporal& inTemporal,
                     Mark& inMark);
        void removeMark(Mark& inMark);
        void clearMarks();
        
        void setNeedsDisplay();

        bool hasCurve() const;
        bool hasIncomingCurve() const;
        bool hasOutgoingCurve() const;
        void addIncomingCurve(Curve& inCurve);
        void removeIncomingCurve(Curve& inCurve);
        void addOutgoingCurve(Curve& inValue);
        void removeOutgoingCurve(Curve& inCurve);
        const std::vector<CurveRef>& getIncomingCurves();
        const std::vector<CurveRef>& getOutgoingCurves();
        const std::vector<CurveRef>& getIncomingCurves() const;
        const std::vector<CurveRef>& getOutgoingCurves() const;

        /// returns all of the temporals following this temporal that
        /// are connected by a tie chain. will return an empty vector
        /// if no following notes are tied to from this temporal.
        TemporalRefVec getTieChain() const;

        Rect getTechniquesFrame(const Point& inPoint) const;

    private:
        ItemRef mTemporal;
        std::vector<CurveRef> mIncomingCurves;
        std::vector<CurveRef> mOutgoingCurves;
        std::vector<MarkRef> mMarks;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
