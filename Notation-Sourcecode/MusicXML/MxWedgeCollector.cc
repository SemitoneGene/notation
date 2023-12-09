// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// self
#include "MxWedgeCollector.h"

// Local
#include "Forward.h"
#include "Constants.h"
#include "Measure.h"
#include "MxConverter.h"
#include "MxScoreReader.h"
#include "Score.h"
#include "Temporal.h"
#include "Throw.h"
#include "Wedge.h"


namespace komp
{
    MxWedgeCollector::MxWedgeCollector()
    : mOpenWedges{}
    , mClosedWedges{}
    {

    }


    void
    MxWedgeCollector::startWedge(Score& inScore,
                                 mx::api::WedgeStart inStart,
                                 mx::api::Placement inPlacement,
                                 mx::api::NoteData inFirstNote,
                                 MxLocation inLocation)
    {
        MxWedgeSpec spec{inScore};
        spec.placementType = inPlacement == mx::api::Placement::above ?
                                            PlacementType::ABOVE_STAFF : PlacementType::BELOW_STAFF;
        spec.setMxWedgeStart(std::move(inStart),
                             std::move(inLocation));
        spec.mxFirstNote = std::move(inFirstNote);
        mOpenWedges.emplace_back(std::move(spec));
    }

    void
    MxWedgeCollector::stopWedge(mx::api::WedgeStop inWedgeStop,
                                MxLocation inMxLocation)
    {
        if (mOpenWedges.empty()) {
            LOG_F(WARNING, "attempted to stop a wedge when no wedges were active");
            return;
        }

        // if numberLevel is not available, use the most recent wedge
        auto iter = mOpenWedges.begin();

        // try to use numberLevel if it is available
        if (inWedgeStop.numberLevel > 0) {
            for (auto it = mOpenWedges.begin(); it != mOpenWedges.end(); ++it) {
                if (it->mxWedgeStart.numberLevel == inWedgeStop.numberLevel) {
                    iter = it;
                    break;
                }
            }
        }

        iter->endMeasure = iter->score.getMeasure(inMxLocation.getKompMeasureIndex()).get();
        iter->endTime = inMxLocation.getKompTickPosition();
        iter->stopLocation = std::move(inMxLocation);
        iter->mxWedgeStop = std::move(inWedgeStop);
        mClosedWedges.push_back(*iter);
        mOpenWedges.erase(iter);
    }


    MxWedgeVec
    MxWedgeCollector::getWedgeSpecs() const
    {
        return mClosedWedges;
    }


    void
    MxWedgeCollector::insertWedges(const MxScoreReader& inMxScoreReader)
    {
        warnIfOpenWedgesExist();

        auto iter = mClosedWedges.begin();
        const auto end = mClosedWedges.end();

        for ( ; iter != end; ++iter) {

            const auto& wedgeSpec = *iter;
            const auto wedgeParams = wedgeSpec.getWedgeParams();

            if (!validateAndWarn(wedgeParams)) {
                LOG_F(ERROR, "invalid wedge, skipping");
                continue;
            }

            insertWedge(inMxScoreReader, wedgeParams, wedgeSpec);
        }
    }


    void
    MxWedgeCollector::warnIfOpenWedgesExist() const
    {
        if (mOpenWedges.size() > 0) {
            LOG_F(WARNING, "unfinished wedges still exist");
        }
    }


    bool
    MxWedgeCollector::validateAndWarn(const WedgeParams& inParams) const
    {
        if (!validateAndWarnNulls(inParams)) {
            return false;
        }

        const int startMeasureIndex = inParams.spannerParams.startMeasure->getIndex();
        const int endMeasureIndex = inParams.spannerParams.endMeasure->getIndex();

        if (startMeasureIndex > endMeasureIndex) {
            LOG_F(ERROR, "start measure cannot be greater than end measure");
            return false;
        }

        if (startMeasureIndex < endMeasureIndex) {
            return true;
        }

        // startMeasure == endMeasure
        if (inParams.spannerParams.startTime > inParams.spannerParams.endTime) {
            LOG_F(ERROR, "wedge start time cannot be after end time");
            return false;
        }

        return true;
    }


    bool
    MxWedgeCollector::validateAndWarnNulls(const WedgeParams& inParams) const
    {
        if (!inParams.spannerParams.startMeasure) {
            LOG_F(ERROR, "null startMeasure");
            return false;
        }

        if (!inParams.spannerParams.endMeasure) {
            LOG_F(ERROR, "null endMeasure");
            return false;
        }

        return true;
    }


    void
    MxWedgeCollector::insertWedge(const MxScoreReader& inMxScoreReader,
                                  const WedgeParams& inParams,
                                  const MxWedgeSpec& inSpec) const
    {
        auto& score = inMxScoreReader.getScore();
        auto& reg = score.getItemRegistry();
        const auto& measure = inParams.spannerParams.startMeasure;
        const auto& track = inParams.spannerParams.track;
        const MusicLocation loc(*measure, *track);
        auto& wedge = reg.createItem<Wedge>(loc, inParams);
        measure->insertItem(wedge);

        if (inSpec.mxWedgeStart.positionData.isDefaultYSpecified) {
            const auto& staff = wedge.getLocation().getStaff();
            const auto defaultYTenths = inSpec.mxWedgeStart.positionData.defaultY;

            const auto unscaledScoreY = inMxScoreReader.defaultYToUnscaledScoreY(defaultYTenths, staff);
            wedge.setSlot(staff.pixelToSlot(unscaledScoreY));
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
