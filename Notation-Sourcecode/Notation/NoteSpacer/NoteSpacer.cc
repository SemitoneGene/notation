// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "NoteSpacer.h"

// Local
#include "ApplySpacingVisitor.h"
#include "AttachedItem.h"
#include "CollisionVisitor.h"
#include "Curve.h"
#include "EventMatrix.h"
#include "IdealWidthsVisitor.h"
#include "Measure.h"
#include "MinimumMeasureWidthVisitor.h"
#include "OverviewVisitor.h"
#include "Score.h"
#include "Temporal.h"
#include "Track.h"

namespace
{
static constexpr const double END_OF_MEASURE_FUDGE = 2.0;
}

namespace komp
{

NoteSpacer&
NoteSpacer::operator=(const NoteSpacer& other)
{
    return *this;
}

void
NoteSpacer::applySpacing(Measure& outMeasure)
{
    EventMatrix matrix{outMeasure};
    
    if (matrix.getIsEmpty()) {
        outMeasure.setNoteSpace(MINIMUM_MEASURE_NOTESPACE);
        return;
    }
    
    OverviewVisitor overview{outMeasure};
    matrix.visitAllEvents(overview);
    
    IdealWidthsVisitor idealWidth{overview};
    matrix.visitSims(idealWidth);
    
    CollisionVisitor collision{};
    matrix.visitAllEvents(collision);
    
    MinimumMeasureWidthVisitor measureWidth{overview, MINIMUM_MEASURE_NOTESPACE};
    matrix.visitSims(measureWidth);
    
    ApplySpacingVisitor apply{};
    matrix.visitAllEvents(apply);
    
    const auto lastSim = matrix.getLastSim();
    
    if (lastSim) {
        const auto largestRight = lastSim->getLargestRightWidth();
        const auto lastSimX = lastSim->getX();
        const auto measureNoteSpace = lastSimX + largestRight + END_OF_MEASURE_FUDGE;
        const auto measureWidth = std::max(MINIMUM_MEASURE_NOTESPACE, measureNoteSpace);
        outMeasure.setNoteSpace(measureWidth);
    }
    
    centerMeasureRests(outMeasure);
    
    
}


void
NoteSpacer::centerMeasureRests(Measure& outMeasure) const
{
    // check to see if the only thing in the plane is a whole rest,
    // and if so, center it in the measure.
    auto& allTracks = outMeasure.getScore().getTracks();
    for (auto& track : allTracks ) {
        for (int handIndex = 0; handIndex < track->getHands(); ++handIndex) {
            for (int voiceIndex = 0; voiceIndex < MAX_VOICES; ++voiceIndex) {
                const auto filter = [&](Item& inItem) {
                    const auto& loc = inItem.getLocation();
                    return loc.getVoice() == voiceIndex
                    && loc.getHand() == handIndex;
                };
                auto temporals = outMeasure.items<Temporal>(*track.get(),
                                                            filter);
                if (temporals.size() == 1) {
                    Temporal& t = temporals.front();
                    const auto type = t.getDuration().getTemporalType();
                    if (type == TemporalType::WHOLE) {
                        if (t.getDuration().getDots() == 0) {
                            if (t.getDuration().getTupletRatios().empty()) {
                                if (t.getIsRest()) {
                                    auto frame = t.getFrame();
                                    const auto measureDataX = outMeasure.getDataX();
                                    const auto measureNoteSpace = outMeasure.getNoteSpace();
                                    frame.moveBy(-1 * measureDataX, 0.0);
                                    const auto measureCenterX = measureNoteSpace / 2.0;
                                    const auto frameCenter = frame.getCenter().x;
                                    const auto moveAmount = measureCenterX - frameCenter;
                                    const auto oldX = t.getX();
                                    const auto newX = oldX + moveAmount;
                                    t.setX(newX);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
