// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxScoreReader.h"

// System
#include <sstream>

// komp
#include "Chord.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "Mark.h"
#include "Measure.h"
#include "MxConverter.h"
#include "MxMeasureReader.h"
#include "MxSpanners.h"
#include "Note.h"
#include "BeamedNoteIntf.h"
#include "BeamFactory.h"
#include "Rest.h"
#include "RoundPlaces.h"
#include "Score.h"
#include "ScoreCreationInfo.h"
#include "ScoreLayoutInfo.h"
#include "Selection.h"
#include "Slur.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "Temporal.h"
#include "Tie.h"
#include "Track.h"
#include "TrackInfo.h"
#include "Tuplet.h"

namespace komp
{
MxScoreReader::MxScoreReader(const mx::api::ScoreData& inData)
: mMxScoreData{inData}
, mTotalMxMeasures{mMxScoreData.getNumMeasures()}
, mMxConverter{std::make_unique<MxConverter>(MxConverterParams{})}
{
    KOMP_ASSERT(mTotalMxMeasures > 0);
}

ScoreUPtr
MxScoreReader::read()
{
    auto score = allocateScore();
    KOMP_ASSERT(score);
    mScore = score.get();
    auto scoreLock = score->getLock();
    importScore(score);
    return score;
}

MxSpannerCollector&
MxScoreReader::getSpannerCollector()
{
    return mMxSpannerCollector;
}

FileMessages&
MxScoreReader::getFileMessages()
{
    return mFileMessages;
}

const mx::api::ScoreData&
MxScoreReader::getScoreData() const
{
    return mMxScoreData;
}

MxLocation
MxScoreReader::createMxLocationObject() const
{
    KOMP_ASSERT(mTotalMxMeasures > 0);
    MxLocationParameters params;
    params.kompTicksPerQuarter = mScore->timeBase();
    params.mxTicksPerQuarter = mMxScoreData.ticksPerQuarter;
    params.totalMxMeasures = mTotalMxMeasures;
    params.multiMeasureRests = mMultiMeasureRests;
    return MxLocation{std::move(params)};
}

const MeasureUPtr&
MxScoreReader::getKompMeasure(int kompMeasureIndex)
{
    return mScore->getMeasure(kompMeasureIndex);
}

Track*
MxScoreReader::getTrack(int trackIndex)
{
    KOMP_ASSERT(trackIndex >= 0);
    const auto& tracks = mScore->getTracks();
    KOMP_ASSERT(trackIndex < static_cast<int>(tracks.size()));
    return tracks.at(static_cast<size_t>(trackIndex)).get();
}

ScoreUPtr
MxScoreReader::allocateScore()
{
    auto trackInfoList = TrackInfoList{};
    
    for (const auto& p : mMxScoreData.parts) {
        
        TrackInfo track;
        
        if (p.name.empty()) {
            track.setName(p.displayName);
        } else {
            track.setName(p.name);
        }
        
        if (p.abbreviation.empty()) {
            track.setAbbreviation(p.displayAbbreviation);
        } else {
            track.setAbbreviation(p.abbreviation);
        }
        
        if (p.instrumentData.soundID != mx::api::SoundID::unspecified) {
            if (p.instrumentData.soundID != mx::api::SoundID::errorBadString) {
                track.setSoundID(MxConverter::convertSoundID(p.instrumentData.soundID));
            }
        }
                
        const int numStaves = p.getNumStaves();
        for (int i = 0; i < numStaves; ++i) {
            track.addStaffInfo(ClefType::G);
        }
        
        if (p.instrumentData.soundID != mx::api::SoundID::unspecified) {
            if (p.instrumentData.soundID != mx::api::SoundID::errorBadString) {
                track.setSoundID(MxConverter::convertSoundID(p.instrumentData.soundID));
            }
        }
        
        trackInfoList.emplace_back(std::move(track));
    }
    
    
    auto pgSortCompare = [&](mx::api::PartGroupData& inLeft, mx::api::PartGroupData& inRight) {
        if (inLeft.firstPartIndex < inRight.firstPartIndex) {
            return true;
        }
        
        if (inLeft.firstPartIndex > inRight.firstPartIndex) {
            return false;
        }
        
        return inLeft.lastPartIndex > inRight.lastPartIndex;
    };
    
    auto partGroups = mMxScoreData.partGroups;
    std::ranges::sort(partGroups, pgSortCompare);
    ScoreInfo scoreInfo;
    
    scoreInfo.metadata.title = mMxScoreData.workTitle;
    scoreInfo.metadata.subtitle = mMxScoreData.movementTitle;
    scoreInfo.metadata.composer = mMxScoreData.composer;
    scoreInfo.metadata.arranger = mMxScoreData.arranger;
    scoreInfo.metadata.lyricist = mMxScoreData.lyricist;
    scoreInfo.metadata.publisher = mMxScoreData.publisher;
    scoreInfo.metadata.copyright = mMxScoreData.copyright;
        
    int previousLast = -1;
    for (const auto& pg : partGroups) {
        if (pg.firstPartIndex < previousLast) {
            LOG_F(ERROR, "nested part grouping is not supported");
            continue;
        }
        auto bracketType = SystemBracketType::NONE;
        if (pg.bracketType == mx::api::BracketType::brace) {
            bracketType = SystemBracketType::BRACE;
        } else if (pg.bracketType == mx::api::BracketType::bracket) {
            bracketType = SystemBracketType::BRACKET;
        } else if (pg.bracketType == mx::api::BracketType::line) {
            bracketType = SystemBracketType::LINE;
        } else if (pg.bracketType == mx::api::BracketType::square) {
            bracketType = SystemBracketType::SQUARE;
        }
        
        SystemBracketInfo bracketInfo{bracketType, 0, pg.firstPartIndex, pg.lastPartIndex};
        scoreInfo.systemBracketInfoList.push_back(bracketInfo);
    }
    
    scoreInfo.trackInfoList = std::move(trackInfoList);
    return Score::create(scoreInfo);
}

std::map<int, int>
MxScoreReader::findMultiMeasureRests() const
{
    std::map<int, int> multiMeasureRests;
    auto location = createMxLocationObject();
    for (const auto& partData : mMxScoreData.parts) {
        for (const auto& measureData : partData.measures) {
            if (measureData.multiMeasureRest > 0 && location.getKompMeasureIndex() >= 0) {
                multiMeasureRests.emplace(location.getKompMeasureIndex(), measureData.multiMeasureRest);
            }
            ++location.mxMeasureIndex;
        }
    }
    return multiMeasureRests;
}

void
MxScoreReader::importScore(ScoreUPtr& inScore)
{
    importKompVersion();
    
    mMultiMeasureRests = findMultiMeasureRests();
    const auto count = static_cast<int>(inScore->getMeasureCount());
    auto mxLocation = createMxLocationObject();
    const auto correctNumMeasures = mxLocation.getTotalKompMeasures();
    KOMP_ASSERT(correctNumMeasures > 0);
    
    // add measures if needed
    for (int i = count; i < correctNumMeasures; ++i) {
        inScore->insertMeasure(static_cast<size_t>(i));
    }
    
    // delete measures if needed
    while (inScore->getMeasureCount() > correctNumMeasures ) {
        inScore->deleteMeasure(inScore->getMeasureCount() - 1);
    }
    
    // TODO: Convert compressed rests into measures with rest
    
    //        auto location = createMxLocationObject();
    //        for (const auto& multiMeasureRestInfoPair : mMultiMeasureRests) {
    //            location.mxMeasureIndex = multiMeasureRestInfoPair.first;
    //            const int kompMeasureIndex = location.getKompMeasureIndex();
    //            const auto& kompMultiMeasureRestMeasure = inScore->getMeasure(kompMeasureIndex);
    //            kompMultiMeasureRestMeasure->addCompressedRest(multiMeasureRestInfoPair.second);
    //            kompMultiMeasureRestMeasure->setCompressedSize(multiMeasureRestInfoPair.second);
    //        }
    
    importScoreMetadata(inScore);
    importPageLayout();
    importLineWidths();
    importSystems();
    importMeasures();
    
    inScore->indentStaves();
    
    // Fix times before importing spanners in case anything
    // is currently out of order
    auto& factory = inScore->getCommandFactory();
    for (const auto& m : inScore->getMeasures()) {
        auto fix = factory.fixTimesCommand(*m.get());
        fix->execute();
    }
    
    auto& reg = inScore->getItemRegistry();
    auto& collector = getSpannerCollector();
    
    auto& beamMgr = collector.beamMgr;
    for (const auto& beamSpanner : beamMgr.getSpanners()) {
        auto& beamSpec = beamSpanner->getStart();
        if (beamSpec->getBeamedNoteCount() > 1) {
            
            bool isFailure = false;
            int hand = -1;
            int voice = -1;
            
            for (const auto& bn : beamSpec->getBeamedNotes()) {
                const auto& temporal = bn->getNote();
                const auto& loc = temporal.getLocation();
                
                if (hand < 0) {
                    hand = loc.getHand();
                    voice = loc.getVoice();
                } else if (hand != loc.getHand() || voice != loc.getVoice()) {
                    LOG_F(INFO, "all notes must be in the same hand and voice to be beamed together");
                    isFailure = true;
                }
            }
            
            if (!isFailure) {
                BeamFactory beamFactory;
                auto& beam = beamFactory.createBeam(reg, *beamSpec);
                beamFactory.insertBeam(beam);
                beam.setStemPolicy(beamSpanner->getStemPolicy());
            }
        }
    }
    
    collector.slurMgr.sortSpanners();
    
    for (const auto& spanner : collector.slurMgr.getSpanners()) {
        
        auto& leftHandNote = spanner->getStart();
        auto& rightHandNote = spanner->getStop();
        const CurveParams slurParams {
            leftHandNote,
            rightHandNote,
            spanner->getPlacementType()
        };
        const auto& loc = leftHandNote.getLocation();
        auto& slur = reg.createItem<Slur>(loc, std::move(slurParams));
        slur.setIsQuiescent(false);
        const auto time = leftHandNote.getTime();
        leftHandNote.getLocation().getMeasure().insertItem(slur, time);
    }
    
    collector.ties.insertTies(*inScore.get());
    collector.wedges.insertWedges(*this);
    collector.endings.insertEndings(reg);
    
    for (const auto& spanner : mMxSpannerCollector.ottavaMgr.getSpanners()) {
        const auto& note = spanner->getStart();
        const auto params = spanner->createSpannerParams();
        const MusicLocation loc(*note.measure, *note.track, note.hand);
        auto& ottava = reg.createItem<Octave>(loc,
                                              params,
                                              spanner->getOctaveType());
        insertSpanner(params, ottava);
    }
    
    mMxSpannerCollector.tuplets.insertTuplets();
    
    // Fix times after tupletizing
    for (const auto& m : inScore->getMeasures()) {
        auto fix = factory.fixTimesCommand(*m.get());
        fix->execute();
    }
    
    importUnhandledDirections();
    calculateAttributesForVoice();
    
    inScore->layout();
    inScore->updateItems();
}

void
MxScoreReader::importScoreMetadata(ScoreUPtr& inScore)
{
    mProperties.clear();
    
    for (const auto& misc : mMxScoreData.encoding.miscelaneousFields) {
        mProperties.emplace(misc.key, misc.value);
    }
}

void
MxScoreReader::importPageLayout()
{
    auto& engraver = Engraver::instance();
    const auto& millimeters = mMxScoreData.defaults.scalingMillimeters;
    
    if (millimeters > 0) {
        engraver.setMilimeters(static_cast<double>(millimeters));
        mIsScalingFound = true;
        mMillimeters = static_cast<double>(millimeters);
    } else {
        mIsScalingFound = false;
        return;
    }
    
    const auto& tenths = mMxScoreData.defaults.scalingTenths;
    
    if (tenths > 0) {
        engraver.setTenths(static_cast<double>(tenths));
        mIsScalingFound = true;
        mTenths = static_cast<double>(tenths);
    } else {
        mIsScalingFound = false;
        return;
    }
    
    MxConverterParams params;
    params.millimeters = millimeters;
    params.tenths = tenths;
    params.pixelsPerInch = PIXELS_PER_INCH;
    mMxConverter = std::make_unique<MxConverter>(MxConverter{params});
    
    //        const auto& layout = mScore->getLayout();
    //        const auto& pages = layout->getPages();
    //        auto pageLayout = pages.front()->getLayout();
    //        const auto scaleFactor = engraver.scaleFactor();
    
    //        if (!mMxScoreData.layout.areOddMarginsValid() ||
    //            !mMxScoreData.layout.areEvenMarginsValid()) {
    //            return;
    //        }
    
    //        auto leftUnscaled = mMxScoreData.layout.oddPageLeftMargin;
    //        auto rightUnscaled = mMxScoreData.layout.oddPageRightMargin;
    //        auto topUnscaled = mMxScoreData.layout.oddPageTopMargin;
    //
    //        if (!mMxScoreData.layout.areOddEvenMarginsTheSame()) {
    //            KOMP_FILE_INFO("unsupported margins - MusicXML specified different "
    //                           "margins for even and odd pages - "
    //                           "margins will be averaged");
    //            leftUnscaled =
    //                (leftUnscaled + mMxScoreData.layout.evenPageLeftMargin) / 2.0L;
    //            rightUnscaled =
    //                (rightUnscaled + mMxScoreData.layout.evenPageRightMargin) / 2.0L;
    //            topUnscaled =
    //                (topUnscaled + mMxScoreData.layout.evenPageTopMargin) / 2.0L;
    //        }
    
    //        auto leftPos = scaleFactor * static_cast<double>(leftUnscaled);
    //        auto rightPos = static_cast<float>(pageLayout.getPageWidth()) -
    //                         (scaleFactor * static_cast<double>(rightUnscaled));
    //        auto topPos = scaleFactor * static_cast<double>(topUnscaled);
    //        auto bottomPos = static_cast<double>(pageLayout.getPageHeight()) -
    //                          (scaleFactor * static_cast<float>(topUnscaled));
    //
    //        if (leftPos < 0 || topPos < 0 ||
    //            rightPos > static_cast<double>(pageLayout.getPageWidth()) ||
    //            bottomPos > static_cast<double>(pageLayout.getPageHeight())) {
    //            KOMP_FILE_ERROR("MusicXML file specified bad margin values, using "
    //                            "zeros instead");
    //            leftPos = 0;
    //            topPos = 0;
    //            rightPos = static_cast<float>(pageLayout.getPageWidth());
    //            bottomPos = static_cast<float>(pageLayout.getPageHeight());
    //        }
    //
    //        pageLayout.setPageFrame(
    //            komp::Rect(topPos, leftPos, bottomPos, rightPos));
}

void
MxScoreReader::importLineWidths()
{
    // TODO: import line widths if desired
}

void
MxScoreReader::importGroups()
{
    // TODO: importGroups function doesn't work at all
#if 0
    const auto& line = mScore->getStaffSystems().at(0);
    
    if (mMxScoreData.partGroups.empty()) {
        bool breakNextStaff = true;
        for (auto it = line->getStaves().rbegin(); it != line->getStaves().rend();
             ++it) {
            if (breakNextStaff) {
                //(*it)->setBarBreak(true);
            }
            breakNextStaff = (*it)->getTrack() == nullptr;
        }
        return;
    }
    
    for (const auto& grp : mMxScoreData.partGroups) {
        // TODO: get brace information from Mx and support different brace
        // types
        const auto bracketType = SystemBracketType::BRACE;
        
        const auto bracketInfo = SystemBracketInfo(bracketType,
                                                   0,
                                                   grp.firstPartIndex,
                                                   grp.lastPartIndex);
        
        // Add the bracket info to the score layout
        auto& layout = mScore->getLayout();
        layout->addBracket(bracketInfo);
        
        // TODO: break barlines per group as indicated in the file
        //            auto& lastTrack = mScore->getTracks().at(grp.lastPartIndex);
        
        // TODO: get the barline information from Mx and follow it
        //            lastTrack->setBarBreak(true);
    }
#endif
}

void
MxScoreReader::importSystems()
{
    // TODO: importSystems if MusicXML contains system info and skip the
    // call to autoSetupSystems
    autoSetupSystems();
}

void
MxScoreReader::autoSetupSystems()
{
    const int numStaves = mMxScoreData.getNumStavesPerSystem();
    
    int numSystemsPerPage = 4;
    
    // TODO: calculating numSystemsPerPage should be smarter
    if (numStaves > 10) {
        numSystemsPerPage = 1;
    } else if (numStaves > 5) {
        numSystemsPerPage = 2;
    } else if (numStaves > 2) {
        numSystemsPerPage = 3;
    } else if (numStaves == 2) {
        numSystemsPerPage = 4;
    } else if (numStaves == 1) {
        numSystemsPerPage = 8;
    }
    
    ScoreLayoutInfo scoreLayoutInfo;
    PageLayoutInfo pageLayoutInfo;
    
    const int totalMeasures = createMxLocationObject().getTotalKompMeasures();
    int remainingMeasures = totalMeasures;
    
    // TODO: calculate num measures per system or read it from MusicXML
    const int measuresPerSystem = 4;
    while (remainingMeasures > 0) {
        const int measuresInThisSystem =
        remainingMeasures > measuresPerSystem ? measuresPerSystem :
        remainingMeasures;
        StaffSystemLayoutInfo staffSystemLayoutInfo;
        staffSystemLayoutInfo.numMeasures = measuresInThisSystem;
        KOMP_ASSERT(staffSystemLayoutInfo.numMeasures > 0);
        pageLayoutInfo.staffSystems.emplace_back(std::move(staffSystemLayoutInfo));
        
        if (pageLayoutInfo.staffSystems.size() >= numSystemsPerPage) {
            scoreLayoutInfo.pages.emplace_back(std::move(pageLayoutInfo));
            pageLayoutInfo = PageLayoutInfo{};
        }
        
        remainingMeasures -= measuresPerSystem;
    }
    if (pageLayoutInfo.staffSystems.size() > 0) {
        scoreLayoutInfo.pages.emplace_back(std::move(pageLayoutInfo));
    }
    mScore->getLayout()->updateLayoutInfo(scoreLayoutInfo);
}

MxConverter
MxScoreReader::getMxConverter() const
{
    return *mMxConverter;
}

bool
MxScoreReader::getIsKompFile() const
{
    return mIsKompFile;
}

Version
MxScoreReader::getKompVersion() const
{
    return mKompVersion;
}

double
MxScoreReader::getIsScalingFound() const
{
    return mIsScalingFound;
}

double
MxScoreReader::getMillimeters() const
{
    return std::max(mMillimeters, 0.0001);
}

double
MxScoreReader::getTenths() const
{
    return std::max(mTenths, 0.0001);
}


double
MxScoreReader::getSpaceSize() const
{
    if (!getIsScalingFound() ||
        getMillimeters() <= 0.0) {
        return -1.0;
    }
    
    const auto tenths = getTenths();
    const double spaces = tenths / 10.0;
    const double millimetersPerSpace = getMillimeters() / spaces;
    
    // 25.4 mm per inch
    const double inchesPerSpace = millimetersPerSpace / 25.4;
    const double pixelsPerSpace = inchesPerSpace * PIXELS_PER_INCH;
    return pixelsPerSpace;
}


double
MxScoreReader::defaultYToUnscaledScoreY(double inDefaultY,
                                        const Staff& inStaff) const
{
    const auto spaceSize = Staff::getSpacing();
    const auto staffY = inStaff.getScoreY();
    const auto ySpaces = inDefaultY / 10;
    const auto yPixels = ySpaces * spaceSize * -1;
    const auto unscaledScoreY = staffY + yPixels;
    return unscaledScoreY;
}


double
MxScoreReader::measureDefaultXToMeasureX(double inDefaultX,
                                         const Staff& inStaff,
                                         const Measure& inMeasure) const
{
    const auto iter = measureWidthMap.find(inMeasure.getIndex());
    KOMP_ASSERT(iter != measureWidthMap.cend());
    const auto mxMeasureWidthPixels = iter->second;
    KOMP_ASSERT(mxMeasureWidthPixels != 0.0);
    const auto measureLeftGap = inMeasure.getLeftGap();
    const auto measureNoteSpacePixels = inMeasure.getNoteSpace();
    const auto measureWidthPixels = measureLeftGap + measureNoteSpacePixels;
    const auto ratio = measureWidthPixels / mxMeasureWidthPixels;
    const auto adjustedDefaultXTenths = inDefaultX * ratio;
    const auto defaultXSpaces = adjustedDefaultXTenths / 10.0;
    const auto spaceSize = getSpaceSize();
    KOMP_ASSERT(spaceSize > 0.0);
    const auto defaultXPixels = defaultXSpaces * spaceSize;
    const auto measureRelativeX = komp::roundPlaces<double>(defaultXPixels - measureLeftGap, 10);
    return measureRelativeX;
}


void
MxScoreReader::addDirection(MxDirection&& inMxDirection)
{
    mDirections.emplace_back(std::move(inMxDirection));
}


void
MxScoreReader::importMeasures()
{
    MxMeasureReader mxMeasureReader{*this};
    mxMeasureReader.importMeasures();
}

Version
MxScoreReader::parseKompVersion() const
{
    int major = 0;
    int minor = 0;
    int patch = 0;
    for (const auto& misc : mMxScoreData.encoding.miscelaneousFields) {
        if (misc.key == "komp-version-major") {
            std::stringstream ss;
            ss.str(misc.value);
            ss >> major;
        } else if (misc.key == "komp-version-minor") {
            std::stringstream ss;
            ss.str(misc.value);
            ss >> minor;
        } else if (misc.key == "komp-version-patch") {
            std::stringstream ss;
            ss.str(misc.value);
            ss >> patch;
        }
    }
    return Version{major, minor, patch};
}

void
MxScoreReader::importKompVersion()
{
    for (const auto& s : mMxScoreData.encoding.software) {
        if (s == "komp") {
            auto version = parseKompVersion();
            if (version.major() == 0 && version.minor() == 0 && version.patch() == 0) {
                mIsKompFile = false;
                return;
            } else {
                mIsKompFile = true;
                mKompVersion = version;
                return;
            }
        }
    }
}

void
MxScoreReader::importUnhandledDirections()
{
    for (auto& mxdirection : mDirections) {
        
        const auto& location = mxdirection.location;
        if (location.getKompMeasureIndex() < 0
            || location.getKompMeasureIndex() > (mScore->getMeasureCount() - 1)) {
            LOG_F(ERROR, "bad measure index");
            return;
        }
        
        const int trackCount = static_cast<int>(mScore->getTracks().size());
        
        if (location.partIndex < 0 || location.partIndex > (trackCount - 1)) {
            LOG_F(ERROR, "bad track index");
            return;
        }
        
        const auto& track = mScore->getTracks().at(location.partIndex);
        if (location.handIndex < 0 || location.handIndex > (track->getHands() - 1)) {
            LOG_F(ERROR, "bad hand index");
            return;
        }
        
        if (location.voiceIndex < 0 || location.voiceIndex > (MAX_VOICES - 1)) {
            LOG_F(ERROR, "bad voice index");
            return;
        }
        
        const auto& measure = mScore->getMeasure(location.getKompMeasureIndex());
        const MusicLocation musicLoc(*measure.get(),
                                     *track.get(),
                                     location.handIndex,
                                     location.voiceIndex,
                                     0,
                                     location.getKompTickPosition());
        auto& temporal = findNearestTemporal(musicLoc);
        
        bool isY = false;
        double scoreY = 0.0;
        bool isX = false;
        double xRelativeToTemporal = 0.0;
        
        for (const auto& mark : mxdirection.direction.marks) {
            
            calculateDirectionPosition(temporal,
                                       mark.positionData,
                                       musicLoc,
                                       isX,
                                       xRelativeToTemporal,
                                       isY,
                                       scoreY);
            
            importDirectionMark(temporal,
                                mark,
                                isX,
                                xRelativeToTemporal,
                                isY,
                                scoreY);
        }
        
        std::vector<mx::api::WordsData> unhandledWords;
        
        for (const auto& words : mxdirection.direction.words) {
            
            bool isY = false;
            double scoreY = 0.0;
            bool isX = false;
            double xRelativeToTemporal = 0.0;
            
            calculateDirectionPosition(temporal,
                                       words.positionData,
                                       musicLoc,
                                       isX,
                                       xRelativeToTemporal,
                                       isY,
                                       scoreY);
            
            if (!importExpressionWords(temporal,
                                       words,
                                       isX,
                                       xRelativeToTemporal,
                                       isY,
                                       scoreY)) {
                unhandledWords.emplace_back(words);
            }
        }
        
        auto& reg = mScore->getItemRegistry();
        for (const auto& words : unhandledWords) {
            
            auto placement = PlacementType::AUTOMATIC;
            if (mxdirection.direction.placement == mx::api::Placement::above) {
                placement  = PlacementType::ABOVE_STAFF;
            } else if (mxdirection.direction.placement == mx::api::Placement::below) {
                placement  = PlacementType::BELOW_STAFF;
            }
            
            auto& textItem = reg.createItem<Text>(musicLoc,
                                                  words.text,
                                                  placement);
            measure->insertItem(textItem);
        }
    }
}


void
MxScoreReader::importDirectionMark(Temporal& inTemporal,
                                   const mx::api::MarkData& inMark,
                                   bool inIsXCalculated,
                                   double inXRelativeToTemporal,
                                   bool inIsYCalculated,
                                   double inScoreY)
{
    if (mx::api::isMarkPedal(inMark.markType)) {
        
        auto pedalType = MarkType::PEDAL;
        if (inMark.markType == mx::api::MarkType::damp) {
            pedalType = MarkType::PEDAL_UP;
        }
        
        const auto mxp = inMark.positionData.placement;
        const auto placemement = mxp == mx::api::Placement::above ? PlacementType::ABOVE_STAFF : PlacementType::BELOW_STAFF;
        auto& reg = mScore->getItemRegistry();
        auto& attachments = inTemporal.getAttachments();
        auto& markItem = attachments.addMark(reg,
                                             inTemporal,
                                             pedalType,
                                             placemement);
        
        if (inIsYCalculated) {
            markItem.getRelativePoint().setScoreY(inScoreY);
        }
        
        if (inIsXCalculated) {
            markItem.getRelativePoint().setX(inXRelativeToTemporal);
        }
    }
}

bool
MxScoreReader::importExpressionWords(Temporal& inTemporal,
                                     const mx::api::WordsData& inWords,
                                     bool inIsXCalculated,
                                     double inXRelativeToTemporal,
                                     bool inIsYCalculated,
                                     double inScoreY)
{
    auto& reg = mScore->getItemRegistry();
    const auto& markMap = FontTheme::instance().MarkInfo;
    auto& attachments = inTemporal.getAttachments();
    
    for (const auto& pair : markMap) {
        
        if (pair.second.getMarkCategory() == MarkCategory::EXPRESSION) {
            if (pair.second.getString() == inWords.text) {
                const auto markType = pair.first;
                auto& mark = attachments.addMark(reg,
                                                 inTemporal,
                                                 markType,
                                                 PlacementType::AUTOMATIC);
                if (inIsYCalculated) {
                    mark.getRelativePoint().setScoreY(inScoreY);
                }
                
                if (inIsXCalculated) {
                    mark.getRelativePoint().setX(inXRelativeToTemporal);
                }
                return true;
            }
        }
    }    
    return false;
}

Temporal&
MxScoreReader::findNearestTemporal(const MusicLocation& inMusicLocation)
{
    auto& measure = inMusicLocation.getMeasure();
    auto temporals = measure.items<Temporal>(inMusicLocation.getTrack(),
                                             [&](Item& inFilterItem) {
        const auto& loc = inFilterItem.getLocation();
        return loc.getHand() == inMusicLocation.getHand();
    });
    
    // creates a placeholder rest in case the incoming file
    // doesn't have temporals to which we can attach the Mark
    if (temporals.empty()) {
        LOG_F(INFO, "adding a whole rest which is needed for attaching a mark");
        auto& reg = mScore->getItemRegistry();
        auto& rest = reg.createItem<Rest>(inMusicLocation,
                                          TemporalType::WHOLE);
        rest.setTime(0.0);
        measure.insertItem(rest);
        mScore->getCommandFactory().fixTimesCommand(measure)->execute();
        noteMxNoteMap.emplace(&rest, mx::api::NoteData{});
        return rest;
    }
    
    auto sortCompare = [&](Temporal& inLeft, Temporal& inRight) {
        
        const auto& locLeft = inLeft.getLocation();
        const auto& locRight = inRight.getLocation();
        
        const bool isLeftSameVoice = locLeft.getVoice() == inMusicLocation.getVoice();
        const bool isRightSameVoice = locRight.getVoice() == inMusicLocation.getVoice();
        
        if (isLeftSameVoice && !isRightSameVoice) {
            return true;
        }
        
        if (!isLeftSameVoice && isRightSameVoice) {
            return false;
        }
        
        const auto leftTimeDistance = std::abs(inLeft.getTime() - inMusicLocation.getTime());
        const auto rightTimeDistance = std::abs(inRight.getTime() - inMusicLocation.getTime());
        
        if (leftTimeDistance < rightTimeDistance) {
            return true;
        } else if (leftTimeDistance > rightTimeDistance) {
            return false;
        }
        
        const bool isLeftChord = inLeft.getIsChord();
        const bool isRightChord = inRight.getIsChord();
        
        if (isLeftChord && !isRightChord) {
            return true;
        } else if (!isLeftChord && isRightChord) {
            return false;
        }
        
        return inLeft.getSlot() < inRight.getSlot();
    };
    
    std::ranges::sort(temporals, sortCompare);
    
    return temporals.front();
}


void
MxScoreReader::importFirstSystemPositioning()
{
    const auto& fields = mMxScoreData.encoding.miscelaneousFields;
    
    double firstSystemY = 0.0;
    if (!importFirstSystemY(fields, firstSystemY)) {
        return;
    }
    
    std::vector<double> staffRelativeYPositions;
    if (!importFirstSystemStaffY(fields, staffRelativeYPositions)) {
        return;
    }
    
    if (mScore->getStaffSystems().empty()) {
        return;
    }
    
    const auto& system = mScore->getStaffSystems().front();
    
    if (system->getStaves().size() != staffRelativeYPositions.size()) {
        return;
    }
    
    // nope
    //        system->setY(firstSystemY);
    //
    //        int staffIndex = 0;
    //
    //        for (const auto& staff : system->getStaves()) {
    //            staff->setY(std::max(0.0,staffRelativeYPositions.at(staffIndex)));
    //            ++staffIndex;
    //        }
}



bool
MxScoreReader::importFirstSystemY(const std::vector<mx::api::MiscellaneousField>& inFields, double& ioFirstSystemY) const
{
    const auto finder = [&](const mx::api::MiscellaneousField& inField) {
        if (inField.key == "komp-first-system-y") {
            return true;
        }
        return false;
    };
    
    const auto iter = std::find_if(std::cbegin(inFields), std::cend(inFields), finder);
    
    if (iter == std::cend(inFields)) {
        return false;
    }
    
    try
    {
        ioFirstSystemY = std::stod(iter->value);
    }
    catch (...)
    {
        return false;
    }
    
    return true;
}


bool
MxScoreReader::importFirstSystemStaffY(const std::vector<mx::api::MiscellaneousField>& inFields, std::vector<double>& ioStaffYVec) const
{
    try
    {
        const std::string firstPart = "komp-staff";
        const std::string lastPart = "relative-y";
        
        const auto finder = [&](const mx::api::MiscellaneousField& inField) {
            if (inField.key.size() > (firstPart.size() + lastPart.size())) {
                if (inField.key.substr(0, firstPart.size()) == firstPart) {
                    if (inField.key.substr(inField.key.size() - lastPart.size()) == lastPart) {
                        return true;
                    }
                }
            }
            return false;
        };
        
        std::vector<mx::api::MiscellaneousField> positions;
        std::copy_if(std::cbegin(inFields), std::cend(inFields), std::back_inserter(positions), finder);
        
        if (mScore->getStaffSystems().empty()) {
            return false;
        }
        
        const auto& system = mScore->getStaffSystems().front();
        
        if (positions.size() != system->getStaves().size()) {
            return false;
        }
        
        std::map<int, double> sorted;
        
        for (const auto& p : positions) {
            const auto leftStripped = p.key.substr(firstPart.size() + 1);
            const auto rightStripped = leftStripped.substr(0, leftStripped.size() - lastPart.size() - 1);
            std::stringstream ss{rightStripped};
            int index = 0;
            ss >> index;
            sorted[index] = std::stod(p.value);
        }
        
        if (sorted.size() != system->getStaves().size()) {
            return false;
        }
        
        ioStaffYVec.clear();
        for (const auto& pair : sorted) {
            ioStaffYVec.push_back(pair.second);
        }
    }
    catch(...)
    {
        return false;
    }
    
    return true;
}


mx::api::NoteData*
MxScoreReader::findNearestMxNoteForDirection(MxDirection& inMxDirection)
{
    int voice = inMxDirection.location.voiceIndex;
    
    const auto voiceMapIter = inMxDirection.voiceMap.find(voice);
    
    if (voiceMapIter == inMxDirection.voiceMap.cend()) {
        return nullptr;
    }
    
    auto& voicePair = *voiceMapIter;
    auto& noteVector = voicePair.second.notes;
    
    if (noteVector.empty()) {
        return nullptr;
    }
    
    const auto sortLambda = [&](mx::api::NoteData& inLeft, mx::api::NoteData& inRight) {
        const auto leftDistance = std::abs(inLeft.tickTimePosition - inMxDirection.direction.tickTimePosition);
        const auto rightDistance = std::abs(inRight.tickTimePosition - inMxDirection.direction.tickTimePosition);
        return leftDistance < rightDistance;
    };
    
    std::ranges::sort(noteVector, sortLambda);
    auto& nearestNote = noteVector.front();
    return &nearestNote;
}


bool
MxScoreReader::calculateDirectionPosition(Temporal& inTemporal,
                                          const mx::api::PositionData& inPositionData,
                                          const MusicLocation& inMusicLocation,
                                          bool& ioIsXCalculated,
                                          double& ioXRelativeToTemporal,
                                          bool& ioIsYCalculated,
                                          double& ioScoreY) const
{
    const auto& mxNoteData = noteMxNoteMap.at(&inTemporal);
    
    ioIsYCalculated = false;
    ioScoreY = 0.0;
    
    if (inPositionData.isDefaultYSpecified) {
        const auto& staff = inMusicLocation.getStaff();
        const auto defaultYTenths = inPositionData.defaultY;
        const auto unscaledScoreY = defaultYToUnscaledScoreY(defaultYTenths, staff);
        ioIsYCalculated = true;
        ioScoreY = unscaledScoreY;
    }
    
    
    ioIsXCalculated = false;
    ioXRelativeToTemporal = 0.0;
    
    if (inPositionData.isDefaultXSpecified) {
        const auto& staff = inMusicLocation.getStaff();
        auto& measure = inMusicLocation.getMeasure();
        
        if (mxNoteData.positionData.isDefaultXSpecified) {
            
            const auto noteDefaultX = mxNoteData.positionData.defaultX;
            const auto directionDefaultX = inPositionData.defaultX;
            const auto deltaDefaultX = directionDefaultX - noteDefaultX;
            const auto deltaXSpaces = deltaDefaultX / 10.0;
            const auto deltaXPixels = deltaXSpaces * Staff::getSpacing();
            ioIsXCalculated = true;
            ioXRelativeToTemporal = deltaXPixels;
            
        } else if (measureWidthMap.find(measure.getIndex()) != measureWidthMap.cend()) {
            const auto defaultXTenths = inPositionData.defaultX;
            const auto measureRelativeX = measureDefaultXToMeasureX(defaultXTenths,
                                                                    staff,
                                                                    measure);
            const auto temporalX = inTemporal.getX();
            const auto temporalRelativeX = measureRelativeX - temporalX;
            ioIsXCalculated = true;
            ioXRelativeToTemporal = temporalRelativeX;
        }
    }
    
    return ioIsXCalculated || ioIsYCalculated;
}

/// \discussion Set stem directions based on number of voices in a staff.
/// If there is a single voice, use existing stem directions.
/// If more than one voice, use the following logic:
///     Voice 1: Stems Down
///     Voice 2: Stems Up
///     Voice 3: Stems Down
///     Voice 4: Stems Up
/// TODO: This should be configurable
void
MxScoreReader::calculateAttributesForVoice() const
{
    for (const auto& measure : mScore->getMeasures()) {
        auto& tracks = mScore->getTracks();
        for (auto& track : tracks) {
            
            const auto hands = track->getHands();
            for (size_t hand = 0; hand < hands; ++hand) {
                
                const auto voices = measure->voices(*track.get(), hand);
                if (voices > 1) {
                    
                    for (auto voice = 0; voice < voices; ++voice) {
                        const auto stemDirection = voice % 2 ? StemDirection::DOWN : StemDirection::UP;
                        
                        const auto temporals = measure->getTemporals(*track.get(),
                                                                     hand,
                                                                     voice);
                        for (Temporal& temporal : temporals) {
                            if (temporal.getItemTraits().getItemType() == ItemType::Note) {
                                auto& note = temporal.as<Note>();
                                note.setStemPolicy({stemDirection, Policy::SPECIFIED});
                                
                                if (note.getIsBeamed()) {
                                    const auto& beam = note.getBeam();
                                    beam->setStemPolicy({stemDirection, Policy::SPECIFIED});
                                }
                            } else if (temporal.getItemTraits().getItemType() == ItemType::Rest) {
                                auto& rest = temporal.as<Rest>();
                                rest.setSlot(rest.slotForVoice(voices));
                            }
                        }
                    }
                }
            }
        }
    }
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
