// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxScoreWriter.h"

// Komp
#include "Measure.h"
#include "MxPartWriter.h"
#include "RoundPlaces.h"
#include "Score.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "SystemBracket.h"
#include "Track.h"
#include "Version.h"

namespace komp
{
    MxScoreWriter::MxScoreWriter(const FileSaveParams& inParams,
                                 FileMessages& outFileMessages)
    : mScore{inParams.score}
    , mOutFileMessages(outFileMessages)
    , mMxLocationPtr(nullptr)
    , mTieStack()
    , mSlurStack()
    , mMillimeters(8.0)
    , mTenths(40.0)
    , mStaffSpaceSizePixels(4.0)
    , mProperties{inParams.properties}
    {
        initializeMxLocation();
        KOMP_ASSERT(mMxLocationPtr != nullptr);

        const auto lock = mScore.getLock();
        
        if (mScore.getStaffSystems().size() > 0) {
            const auto& staffSystem = mScore.getStaffSystems().front();
            for (const auto& staff : staffSystem->getStaves()) {
                if (staff->type() == StaffType::NOTATION) {
                    mStaffSpaceSizePixels = Staff::getSpacing();
                    break;
                }
            }
        }
    }
    
    Score& MxScoreWriter::getScore()
    {
        return mScore;
    }
    
    FileMessages& MxScoreWriter::getFileMessages()
    {
        return mOutFileMessages;
    }
    
    std::list<WedgeRef>&
    MxScoreWriter::getWedges()
    {
        return mWedges;
    }

    
    std::list<EndingRef>&
    MxScoreWriter::getEndings()
    {
        return mEndings;
    }


    MxCurveStack&
    MxScoreWriter::getTieStack()
    {
        return mTieStack;
    }


    MxCurveStack&
    MxScoreWriter::getSlurStack()
    {
        return mSlurStack;
    }


    double
    MxScoreWriter::getMillimeters() const
    {
        return mMillimeters;
    }


    double
    MxScoreWriter::getTenths() const
    {
        return mTenths;
    }

    double
    MxScoreWriter::getSpaceSize() const
    {
        return mStaffSpaceSizePixels;
    }


    double
    MxScoreWriter::measureRelativeXToMeasureDefaultX(double inMeasureRelativeX,
                                                     const Measure& inMeasure) const
    {
        const auto spaceSize = Staff::getSpacing();
        const auto measureRelativeXSpaces = inMeasureRelativeX / spaceSize;
        const auto measureLeftGap = inMeasure.getLeftGap();
        const auto measureDeadSpaceSpaces = measureLeftGap / spaceSize;
        const auto defaultXSpaces = measureRelativeXSpaces + measureDeadSpaceSpaces;
        const auto defaultX = komp::roundPlaces(defaultXSpaces * 10, 5);
        return defaultX;
    }


    double
    MxScoreWriter::unscaledScoreYToDefaultY(double inUnscaledScoreY,
                                            const Staff& inStaff) const
    {
        const auto spaceSize = Staff::getSpacing();
        const auto staffY = inStaff.getScoreY();
        const auto distancePixels = staffY - inUnscaledScoreY;
        const auto distanceSpaces = distancePixels / spaceSize;
        const auto distanceTenths = komp::roundPlaces<double>(distanceSpaces * 10.0, 5) ;
        return distanceTenths;
    }


    mx::api::ScoreData
    MxScoreWriter::getScoreData()
    {
        const auto lock = mScore.getLock();

        mx::api::ScoreData scoreData;
        scoreData.ticksPerQuarter = mScore.timeBase();
        auto params = setEncodingInfo(scoreData);
        MxConverter converter{params};

//        const auto& layout = mScore.getLayout()->getPageLayout();
//        long double pageHeighPixelsFloat = static_cast<long double>(layout.getPageHeight());
//        long double pageWidthPixelsFloat = static_cast<long double>(layout.getPageWidth());
//        
//        scoreData.layout.pageHeight = roundPlaces(converter.convertPixelsToTenths(pageHeighPixelsFloat), 2);
//        scoreData.layout.pageWidth = roundPlaces(converter.convertPixelsToTenths(pageWidthPixelsFloat), 2);
//        
//        long double rightMargin = roundPlaces(converter.convertPixelsToTenths(layout.rightMarginSize()), 3);
//        long double leftMargin = roundPlaces(converter.convertPixelsToTenths(layout.leftMarginSize()), 3);
//        long double topMargin = roundPlaces(converter.convertPixelsToTenths(layout.topMarginSize()), 3);
//        long double bottomMargin = roundPlaces(converter.convertPixelsToTenths(layout.bottomMarginSize()), 3);
//        scoreData.layout.evenPageRightMargin = rightMargin;
//        scoreData.layout.oddPageRightMargin = rightMargin;
//        scoreData.layout.evenPageLeftMargin = leftMargin;
//        scoreData.layout.oddPageLeftMargin = leftMargin;
//        scoreData.layout.evenPageTopMargin = topMargin;
//        scoreData.layout.oddPageTopMargin = topMargin;
//        scoreData.layout.evenPageBottomMargin = bottomMargin;
//        scoreData.layout.oddPageBottomMargin = bottomMargin;
        
        // TODO: Do a better job of correctly categorizing the different "title"
        // and "composer" metadata.  Also the placement of these objects should be
        // specified with <credit-words> entries in addition to the meta-data fields
        for (const auto& titleItem : mScore.getLayout()->getTitle()) {
            const auto& title = titleItem.Text;
            if (title.size() > 0) {
                if (scoreData.workTitle.empty()) {
                    scoreData.workTitle = title;
                } else if (scoreData.movementTitle.empty()) {
                    scoreData.movementTitle = title;
                } else {
                    continue;
                }
            }
        }
        
        if (scoreData.composer.empty()) {
            const auto& value = mScore.getLayout()->getComposer(0);
            scoreData.composer = value.Text;
        }

        if (scoreData.arranger.empty()) {
            const auto& value = mScore.getLayout()->getComposer(1);
            scoreData.arranger = value.Text;
        }

        if (scoreData.lyricist.empty()) {
            const auto& value = mScore.getLayout()->getComposer(2);
            scoreData.lyricist = value.Text;
        }

        if (scoreData.publisher.empty()) {
            const auto& value = mScore.getLayout()->getComposer(3);
            scoreData.publisher = value.Text;
        }
                
        const auto copyright = mScore.getLayout()->getCopyRight(0).Text;
        if (!copyright.empty()) {
            scoreData.copyright = copyright;
        }
        
        const int measureCount = static_cast<int>(mScore.getMeasureCount());
        
        // TODO: deal with compressed rests
        MxLocationParameters locParams;
        locParams.totalMxMeasures = measureCount;
        locParams.kompTicksPerQuarter = mScore.timeBase();
        locParams.mxTicksPerQuarter = mScore.timeBase();
        MxLocation location{std::move(locParams)};
        
        mMxLocationPtr->partIndex = 0;
        mMxLocationPtr->handIndex = 0;
        mMxLocationPtr->mxTickPosition = 0;
        mMxLocationPtr->mxMeasureIndex = 0;

        for ( ; mMxLocationPtr->partIndex < mScore.getTracks().size(); ++mMxLocationPtr->partIndex) {
            MxPartWriter partWriter{*this, *mMxLocationPtr};
            auto partData = partWriter.getPartData();
            scoreData.parts.emplace_back(std::move(partData));
        }

        KOMP_ASSERT(mScore.getStaffSystems().size() > 0);
        const auto& staffSystem = mScore.getStaffSystems().front();
        KOMP_ASSERT(staffSystem);
        
        std::vector<SystemBracket*> brackets;
        for (const auto& bracket : staffSystem->getBrackets()) {
            brackets.emplace_back(bracket.get());
        }
                
        std::ranges::sort(brackets,
                          [&](SystemBracket* inLeft,
                              SystemBracket* inRight) {

            if (inLeft->getStartStaff().getIndex() < inRight->getStartStaff().getIndex()) {
                return true;
            }

            if (inLeft->getStartStaff().getIndex() > inRight->getStartStaff().getIndex()) {
                return false;
            }

            if (inLeft->getStopStaff().getIndex() > inRight->getStopStaff().getIndex()) {
                return true;
            }

            if (inLeft->getStopStaff().getIndex() < inRight->getStopStaff().getIndex()) {
                return false;
            }

            return inLeft < inRight;
        });

        int previousStop = -1;

        for (const auto& bracket : brackets) {
            const auto& startStaff = bracket->getStartStaff();
            const auto& stopStaff = bracket->getStopStaff();
            const int currentStart = startStaff.getIndex();
            const int currentStop = stopStaff.getIndex();

            if (currentStart < previousStop) {
                LOG_F(ERROR, "nested brackets are not yet supported in MusicXML export");
                continue;
            }

            if (currentStart > currentStop) {
                LOG_F(ERROR, "illogical bracket start and stop");
                continue;
            }

            const auto& startTrack = startStaff.getTrack();
            const int startHand = startStaff.getHand();
            const auto& stopTrack = stopStaff.getTrack();
            const auto& stopHand = stopStaff.getHand();

            if (startHand != 0) {
                LOG_F(ERROR, "a bracket must span the entire MusicXML part");
                continue;
            }

            if (stopHand != (stopTrack.getHands() - 1)) {
                LOG_F(ERROR, "a bracket must span the entire MusicXML part");
                continue;
            }

            mx::api::PartGroupData pgData;
            pgData.firstPartIndex = mScore.getTrackIndex(startTrack);
            pgData.lastPartIndex = mScore.getTrackIndex(stopTrack);
            
            auto bracketType = mx::api::BracketType::none;
            if (bracket->getType() == SystemBracketType::BRACE) {
                bracketType = mx::api::BracketType::brace;
            } else if (bracket->getType() == SystemBracketType::BRACKET) {
                bracketType = mx::api::BracketType::bracket;
            } else if (bracket->getType() == SystemBracketType::LINE) {
                bracketType = mx::api::BracketType::line;
            } else if (bracket->getType() == SystemBracketType::SQUARE) {
                bracketType = mx::api::BracketType::square;
            }
            
            pgData.bracketType = bracketType;

            if (pgData.firstPartIndex > pgData.lastPartIndex) {
                LOG_F(ERROR, "tracks are out of the expected order");
                continue;
            }

            scoreData.partGroups.emplace_back(std::move(pgData));

            previousStop = currentStop;
        }

        auto pgDataCompare = [&](mx::api::PartGroupData& inLeft, mx::api::PartGroupData& inRight) {
            if (inLeft.firstPartIndex < inRight.firstPartIndex) {
                return true;
            }

            if (inLeft.firstPartIndex > inRight.firstPartIndex) {
                return false;
            }

            if (inLeft.lastPartIndex > inRight.lastPartIndex) {
                return true;
            }

            if (inLeft.lastPartIndex < inRight.lastPartIndex) {
                return false;
            }

            return &inLeft < &inRight;
        };

        std::sort(std::begin(scoreData.partGroups),
                  std::end(scoreData.partGroups),
                  pgDataCompare);

        previousStop = -1;
        for (const auto& pgData : scoreData.partGroups) {
            if (pgData.firstPartIndex < previousStop) {
                LOG_F(ERROR, "part group data got messed up");
                scoreData.partGroups.clear();
                break;
            }
        }

        addClientProperties(scoreData);

        LOG_F(3, "done saving");
        return scoreData;
    }
    
    void
    MxScoreWriter::initializeMxLocation()
    {
        MxLocationParameters params;
        params.totalMxMeasures = mScore.getMeasures().size();
        params.kompTicksPerQuarter = mScore.timeBase();
        params.mxTicksPerQuarter = mScore.timeBase();
        mMxLocationPtr = std::make_unique<MxLocation>(std::move(params));
    }
    
    MxConverterParams
    MxScoreWriter::setEncodingInfo(mx::api::ScoreData& outScore)
    {
        outScore.encoding.software.emplace_back( "komp" );
        mx::api::SupportedItem supported;
        supported.elementName = "accidental";
        supported.isSupported = "yes";
        supported.specificValue = "";
        supported.attributeName = "";
        outScore.encoding.supportedItems.emplace_back(supported);
        supported.elementName = "beam";
        outScore.encoding.supportedItems.emplace_back(supported);
        supported.elementName = "stem";
        outScore.encoding.supportedItems.emplace_back(supported);
        
        outScore.encoding.miscelaneousFields.emplace_back("komp-version-major", std::to_string(Version::getCurrent().major()));
        outScore.encoding.miscelaneousFields.emplace_back("komp-version-minor", std::to_string(Version::getCurrent().minor()));
        outScore.encoding.miscelaneousFields.emplace_back("komp-version-patch", std::to_string(Version::getCurrent().patch()));

        if (!mScore.getStaffSystems().empty()) {
            const auto& system = mScore.getStaffSystems().front();
            const double firstSystemY = system->getY();
            std::stringstream ss;
            ss << std::setprecision(5) << komp::roundPlaces(firstSystemY, 4);
            outScore.encoding.miscelaneousFields.emplace_back("komp-first-system-y", ss.str());

            int staffIndex = 0;
            for (const auto& staff : system->getStaves()) {
                const double staffRelativeY = staff->getY();
                ss.str("");
                ss << std::setprecision(5) << komp::roundPlaces(staffRelativeY, 4);
                std::stringstream keyName;
                keyName << "komp-staff-" << staffIndex << "-relative-y";
                outScore.encoding.miscelaneousFields.emplace_back(keyName.str(), ss.str());
                ++staffIndex;
            }
        }

        // calculate the scaling
        long double staffHeight = mScore.getStaffSystems().at(0)->getStaves().at(0)->getHeight();
        outScore.defaults.scalingTenths = 40.0f;
        long double unroundedMillimeters = staffHeight / static_cast<long double>(PIXELS_PER_INCH) * static_cast<long double>(MILLIMETERS_PER_INCH);
        outScore.defaults.scalingMillimeters = roundPlaces<float>(unroundedMillimeters, 6);
        MxConverterParams params;
        params.millimeters = static_cast<long double>(outScore.defaults.scalingMillimeters);
        params.tenths = static_cast<long double>(outScore.defaults.scalingTenths);
        params.pixelsPerInch = static_cast<long double>(PIXELS_PER_INCH);
        mMillimeters = params.millimeters;
        mTenths = params.tenths;
        return params;
    }


    void
    MxScoreWriter::addClientProperties(mx::api::ScoreData& ioScoreData)
    {
        for (const auto& pair : mProperties) {
            ioScoreData.encoding.miscelaneousFields.emplace_back(pair.first, pair.second);
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
