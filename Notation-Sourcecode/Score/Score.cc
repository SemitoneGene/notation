// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Score.h"

// System
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

// Local
#include "BeamIntf.h"
#include "ChangeTimeSignatureCommand.h"
#include "Chord.h"
#include "Clef.h"
#include "Command.h"
#include "Constants.h"
#include "CurrentState.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "Item.h"
#include "ItemImpl.h"
#include "KeySignature.h"
#include "Measure.h"
#include "Note.h"
#include "NoteSpacerIntf.h"
#include "Preferences.h"
#include "Geometry.h"
#include "Rest.h"
#include "ScoreColors.h"
#include "ScoreCreationInfo.h"
#include "ScoreLayout.h"
#include "ScoreLayoutInfo.h"
#include "Selection.h"
#include "Spanner.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "TempoMark.h"
#include "Text.h"
#include "Tie.h"
#include "Track.h"

// Namespaces
using namespace std;

namespace komp
{
// MARK: - ScoreLock

ScoreLock::ScoreLock(RecursiveLockPtr&& inLock)
: mLock{std::move(inLock)}
{
    
}

ScoreLock::ScoreLock()
: mLock{nullptr}
{
    
}

ScoreLock::~ScoreLock()
{
    
}

// MARK: - Score

// Constants
static const int MAX_PAGES = 10000;
static const int MAX_LINES_PER_PAGE = 1000;
static const int MAX_MEASURES_PER_LINE = 1000;

Score::Score(const ScoreInfo& inInfo)
: mCommandFactory{std::make_unique<CommandFactory>(*this)}
, mUndoManager{std::make_unique<UndoManager>(*this)}
, mShowInstrumentNames(inInfo.showInstrumentNames)
, mShowTempoMark(inInfo.showTempoMark)
, mScoreLayout{nullptr}
{
    const auto& prefs = Preferences::instance();
    mMeasureNumberInfo = prefs.MeasureNumbers;
    
    mSelection = std::make_unique<Selection>(*this);
    
    applyScoreInfo(inInfo, false);
}

void
Score::loaded() const
{
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().loaded(*this);
    }
}

ScoreLock
Score::getLock() const
{
    return ScoreLock{std::make_unique<RecursiveLock>(mMutex)};
}

ItemRegistry&
Score::getItemRegistry()
{
    return mItemRegistry;
}

DrawableRegistry&
Score::getDrawableRegistry()
{
    return mDrawableRegistry;
}

CommandFactory&
Score::getCommandFactory() const
{
    return *mCommandFactory.get();
}

UndoManager&
Score::getUndoManager() const
{
    return *mUndoManager.get();
}

// MARK: - Current State

/// \brief Finds the staff, line, measure and slot at the point.
/// \param inPoint The point in the Score coordinate space
/// \result A populated score state
CurrentState
Score::findPosition(const Point& inPoint)
{
    const auto& staff = mScoreLayout->findStaff(inPoint);
    if (staff) {
        return getCurrentState(inPoint, staff);
    } else {
        LOG_F(WARNING, "Unable to locate staff");
        const auto& systems = mScoreLayout->getStaffSystems();
        const auto& staff = systems.front()->getStaves().front();
        return getCurrentState(inPoint, staff.get());
    }
}

/// \brief Get the current state of the score
/// \result The current state
CurrentState
Score::getCurrentState(const Point& inPoint) const
{
    const auto& staff = mScoreLayout->findStaff(inPoint);
    return getCurrentState(inPoint, staff);
}

/// \brief Get the current state of the score
/// \result The current state
CurrentState
Score::getCurrentState(const Point& inPoint, Staff* inStaff) const
{
    // Make sure we're within margins before calculating measure objects.
    // Find the measure we are in by comparing point to barlines.
    auto posX = inPoint.x;
    
    // Set the current measure
    Measure* foundMeasure = nullptr;
    for (const auto& measure : mMeasures) {
        if (posX > measure->getX() && posX <= measure->getRight()) {
            foundMeasure = measure.get();
            break;
        }
    }
    
    if (!foundMeasure) {
        foundMeasure = getMeasuresFront().get();
    }
    
    // Set the current horizontal position.
    // It is the offset from the current measures getDataX()
    const auto slot = std::max(-128.0, inStaff->pixelToSlot(inPoint.y));
    const auto newX = std::max(0.0, posX - foundMeasure->getDataX());
    const CurrentState outState(mContext,
                                *foundMeasure,
                                *inStaff,
                                inStaff->getTrack(),
                                inStaff->getHand(),
                                mCurrentVoice,
                                slot,
                                newX,
                                inPoint);
    return outState;
}

// MARK: - Tap handling

/// \brief Update focus state and locate any items that were clicked on.
/// \param inPoint The tap location on the score
/// \result The item found or nullptr if none found
Item*
Score::handleTap(const Point& inPoint) const
{
    // Get the current score state
    const auto currentState = getCurrentState(inPoint);
    
    // Look for an item
    auto& staff = currentState.getStaff();
    auto& measure = currentState.getMeasure();
    const auto& hand = currentState.getHand();
    auto foundItem = staff.locateItem<Item>(measure,
                                            hand,
                                            inPoint);
    if (foundItem) {
        return foundItem;
    }
    
    // Set up a general purpose chord filter
    const auto chordFilter = [&](const Item& inItem) {
        const auto itemType = inItem.getItemTraits().getItemType();
        return itemType != ItemType::Chord;
    };
    
    // Do a more exhaustive seach in this measure track
    auto& track = staff.getTrack();
    const auto items = measure.items<Item>(track, chordFilter);
    for (Item& item : items) {
        if (&item.getLocation().getStaff() != &staff) {
            if (item.contains(inPoint, true)) {
                return &item;
            }
        }
    }
    
    // Do a more exhaustive seach in other staves for this measure
    for (const auto& scoreTrack : mTracks) {
        const auto& scoreItems = measure.items<Item>(*scoreTrack.get(),
                                                     chordFilter);
        for (Item& scoreItem : scoreItems) {
            if (scoreItem.contains(inPoint, true)) {
                return &scoreItem;
            }
        }
    }
    
    // Do a more exhaustive seach in this staff across all measures.
    // This is necessary in order to locate atached items that may have been dragged
    // out of the parents measure
    for (const auto& measure : getMeasures()) {
        const auto foundItem = staff.locateItem<Item>(*measure,
                                                      hand,
                                                      inPoint);
        if (foundItem) {
            return foundItem;
        }
    }
    
    // Check for items that may span multiple measures
    const auto spannerFilter = [&](const Item& inItem) {
        return inItem.is<Spanner>();
    };
    
    for (const auto& spannerMeasure : mMeasures) {
        auto& spannerTrack = staff.getTrack();
        const auto& spannerItems = spannerMeasure->items<Item>(spannerTrack,
                                                               spannerFilter);
        for (const auto& spannerItem : spannerItems) {
            if (spannerItem.get().contains(inPoint, true)) {
                return &spannerItem.get();
            }
        }
    }
    
    return nullptr;
}

// MARK: - Insertion

/// \brief Create and insert a new measure into the score
/// \param inIndex The index to insert the measure at
/// \result The inserted measure
Measure*
Score::insertMeasure(size_t inIndex)
{
    auto measure = std::make_unique<Measure>(*this);
    auto result = measure.get();
    insertMeasure(std::move(measure), inIndex);
    return result;
}

/// \brief Inserts a configured measure into the score
/// \param inMeasure The measure to insert
/// \param inIndex The index to insert the measure at
void
Score::insertMeasure(MeasureUPtr inMeasure, size_t inIndex)
{
    if (inIndex >= getMeasureCount()) {
        if (inIndex > getMeasureCount()) {
            LOG_F(WARNING, "Attempting to insert measure past end of score.");
        }
        measuresPushBack(std::move(inMeasure));
    } else {
        const auto iter = getMeasuresBegin() + inIndex;
        measuresInsert(iter, std::move(inMeasure));
    }
    
    const auto& measure = getMeasure(inIndex);
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().measureAdded(*measure.get());
    }
}

/// \brief Inserts a track in score.  Does not move data down.
/// \param inInfo The track info
/// \param inTrackIndex Track number to insert
void
Score::insertTrack(const TrackInfo& inInfo, int inTrackIndex)
{
    auto track = std::make_unique<Track>(*this, inInfo);
    auto iter = mTracks.cbegin() + inTrackIndex;
    auto insertedIter = mTracks.insert(iter, std::move(track));
    
    repopulateTrackHash();
    
    // Insert a staff in each system
    auto& insertedTrack = *insertedIter;
    const auto& instrument = insertedTrack->getInstrument();
    const auto& clefs = instrument.startingClefs;
    const auto lineCount = instrument.getStaffLines();
    
    for (int handIndex = 0; const auto& clefType : clefs) {
        insertStaff(*insertedTrack.get(),
                    clefType,
                    lineCount,
                    handIndex++);
    }
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().trackAdded(*insertedTrack.get());
    }
}


static StaffType
getStaffType(ClefType inClefType)
{
    switch (inClefType)
    {
    case ClefType::G:
    case ClefType::G_LINE1:
    case ClefType::G_8VA:
    case ClefType::G_8VB:
    case ClefType::G_15MA:
    case ClefType::G_15MB:
    case ClefType::F:
    case ClefType::F_LINE3:
    case ClefType::F_LINE5:
    case ClefType::F_8VA:
    case ClefType::F_8VB:
    case ClefType::F_15MA:
    case ClefType::F_15MB:
    case ClefType::C:
    case ClefType::C_LINE1:
    case ClefType::C_LINE2:
    case ClefType::C_LINE4:
    case ClefType::C_LINE5:
    case ClefType::UNKNOWN:
    default:
        return StaffType::NOTATION;
        
    case ClefType::PERCUSSION:
    case ClefType::PERCUSSION_2:
    case ClefType::PERCUSSION_3:
        return StaffType::PERCUSSION;
        
    case ClefType::TAB:
    case ClefType::TAB_2:
        return StaffType::TABLATURE;
    }
}

/// \brief Inserts a staff before all existing lines. We assume the line already exists.
/// \param inTrack The track we insert staff for.
/// \param inClefType The clef type
/// \param inLineCount The staff line count
/// \param inHand The hand within track.
void
Score::insertStaff(Track& inTrack,
                   ClefType inClefType,
                   int inLineCount,
                   int inHand)
{
    if (inTrack.getHands() == MAX_HANDS) {
        KOMP_THROW("Unable to insert track. MAX_HANDS exceeded");
    }
    
    const auto& systems = mScoreLayout->getStaffSystems();
    for (const auto& system : systems) {
        
        auto staff = std::make_unique<Staff>(*this,
                                             inTrack,
                                             system.get(),
                                             getStaffType(inClefType),
                                             inLineCount,
                                             inHand);
        system->addStaff(std::move(staff));
    }
    
    if (getMeasureCount() > 0) {
        const auto& measure = getMeasure(0);
        measure->insertClef(inClefType, inTrack, inHand, 0.0);
    }
}

// MARK: - Measure Numbers

/// \brief Converts a measure number to its UserNumber
size_t
Score::userMeasureNumber(size_t inMeasure) const
{
    // If the 1st bar is not a pickup bar, then the
    // user's measure numbers are 1-based.
    size_t userNumber = 0;
    if (true) {
        userNumber++;
    }
    
    return userNumber;
}

// MARK: - Creation

void
Score::validateScoreInfo(const ScoreInfo& inInfo)
{
    KOMP_ASSERT(inInfo.trackInfoList.size() > 0);
    KOMP_ASSERT(inInfo.linesPerPage > 0);
    KOMP_ASSERT(inInfo.measuresPerLine > 0);
    KOMP_ASSERT(inInfo.pages > 0);
    KOMP_ASSERT(inInfo.pages < MAX_PAGES);
    KOMP_ASSERT(inInfo.linesPerPage < MAX_LINES_PER_PAGE);
    KOMP_ASSERT(inInfo.measuresPerLine < MAX_MEASURES_PER_LINE);
    checkTrackUuids(inInfo);
    
    for (const auto& t : inInfo.trackInfoList) {
        KOMP_ASSERT(t.getNumStaves() > 0);
    }
}


bool
Score::getIsNewScore() const
{
    return getTracks().empty();
}


bool
Score::areTrackUuidsUnique(const ScoreInfo& inInfo) const
{
    std::set<std::string_view> ids;
    for (const auto& trackInfo : inInfo.trackInfoList) {
        const auto result = ids.insert(trackInfo.getUuid());
        if (!result.second) {
            return false;
        }
    }
    
    return true;
}


void
Score::checkTrackUuids(const ScoreInfo& inInfo) const
{
    if (!getIsNewScore()) {
        if (!areTrackUuidsUnique(inInfo)) {
            KOMP_THROW("the ScoreInfo tracks do not have unique UUID's");
        }
    }
}

void
Score::createScoreLayout(const ScoreInfo& inInfo)
{
    mScoreLayout = std::make_unique<ScoreLayout>(*this, inInfo);
}

ScoreInfo
Score::convertExistingScoreToLinear(const ScoreInfo& inInfo) const
{
    if (getIsNewScore()) {
        return inInfo;
    }
    
    auto result = inInfo;
    result.measuresPerLine = static_cast<int>(mMeasures.size());
    result.linesPerPage = 1;
    result.pages = 1;
    
    return result;
}

void
Score::addMeasuresToExistingScore(const ScoreInfo& inInfo)
{
    if (getIsNewScore()) {
        return;
    }
    
    const auto measures = mMeasures.size();
    const auto requiredMeasures = inInfo.measuresPerLine * inInfo.linesPerPage;
    if (measures < requiredMeasures) {
        for (auto index = measures; index < requiredMeasures; ++index) {
            validateMeasure(index);
        }
    }
}

void
Score::handleLayoutAndMeasureCounts(const bool inNewScore,
                                    const ScoreInfo& inInfo)
{
    if (inNewScore) {
        createScoreLayout(inInfo);
    } else {
        addMeasuresToExistingScore(inInfo);
    }
}

void
Score::applyScoreMetadata(const ScoreMetadata& inMetadata)
{
    mScoreLayout->setTitle(inMetadata.title, 0);
    mScoreLayout->setTitle(inMetadata.subtitle, 1);
    mScoreLayout->setComposer(inMetadata.composer, 0);
    mScoreLayout->setComposer(inMetadata.arranger, 1);
    mScoreLayout->setComposer(inMetadata.lyricist, 2);
    mScoreLayout->setComposer(inMetadata.publisher, 3);
    mScoreLayout->setCopyright(inMetadata.copyright, 0);
}

void
Score::configureTracks(const bool inNewScore, const ScoreInfo& inInfo)
{
    if (inNewScore) {
        configureNewScoreTracks(inInfo);
    } else {
        deleteAndAddTracks(inInfo);
    }
}


void
Score::configureNewScoreTracks(const ScoreInfo& inInfo)
{
    KOMP_ASSERT(getIsNewScore());
    int trackIndex = 0;
    
    for (const auto& trackInfo : inInfo.trackInfoList) {
        insertTrack(trackInfo, trackIndex++);
    }
    repopulateTrackHash();
}

void
Score::deleteAndAddTracks(const ScoreInfo& inInfo)
{
    KOMP_ASSERT(!getIsNewScore());
    
    const auto changes = getTrackChanges(inInfo);
    
    // Remove the tracks from the score
    for (auto& trackToRemove : changes.remove) {
        deleteTrack(trackToRemove);
    }
    
    // Create the new tracks
    auto trackIndex = mTracks.size();
    for (const auto& trackInfo : changes.add) {
        insertTrack(trackInfo, trackIndex++);
    }
    
    reconcileKeySignatures();
    sortTracksToUserOrder(inInfo);
    sortSystemStaves();
    
    // Update instruments
    KOMP_ASSERT(inInfo.trackInfoList.size() == mTracks.size());
    for (auto index = 0; index < inInfo.trackInfoList.size(); ++index) {
        const auto& trackInfo = inInfo.trackInfoList.at(index);
        auto& track = mTracks.at(index);
        track->setInstrument(trackInfo.getInstrument());
    }
    
    // Reset the undo stack.
    // TODO: Figure out how to undo this later
    mUndoManager->reset();
}


// Ensure all measures in all tracks have matching key signatures.
// This is used when a new track is inserted.
void
Score::reconcileKeySignatures()
{
    for (const auto& measure : getMeasures()) {
        
        const auto keys = measure->items<KeySignature>(*mTracks.front());
        if (keys.empty()) {
            continue;
        }
        
        if (keys.size() > 1) {
            LOG_F(WARNING, "Found more than one key signature in a measure");
        }
        
        const KeySignature& keySignature = keys.front();
        for (auto& track : mTracks) {
            for (auto hand = 0; hand < track->getHands(); ++hand) {
                // This will also replace any existing key signature. It will
                // not add an additional key signature.
                measure->insertKeySignature(*track,
                                            hand,
                                            keySignature.getKey());
            }
        }
    }
}

void
Score::sortTracksToUserOrder(const ScoreInfo& inInfo)
{
    // Sort the tracks as the user specified them in the score builder
    auto originalTracks = std::move(mTracks);
    
    TrackUPtrVec sortedTracks;
    
    for (const auto& sortInfo : inInfo.trackInfoList) {
        const auto sortIter = std::find_if(originalTracks.begin(),
                                           originalTracks.end(),
                                           [sortInfo](const TrackUPtr& inTrack) {
            return sortInfo.getUuid() == inTrack->getUuid();
        });
        
        if (sortIter != originalTracks.end()) {
            auto& track = *sortIter;
            sortedTracks.emplace_back(std::move(track));
            originalTracks.erase(sortIter);
        }
    }
    
    mTracks = std::move(sortedTracks);
    
    repopulateTrackHash();
}

void
Score::setTrackNames(const TrackInfoList& inTrackInfoList)
{
    KOMP_ASSERT(inTrackInfoList.size() == mTracks.size());
    
    const auto& systems = mScoreLayout->getStaffSystems();
    const auto& staffSystem = systems.front();
    
    for (size_t index = 0; const auto& info : inTrackInfoList) {
        auto& track = mTracks.at(index++);
        auto instrument = track->getInstrument();
        instrument.name = info.getName();
        instrument.abbreviation = info.getAbbreviation();
        track->setInstrument(instrument);
        
        for (size_t hand = 0; hand < track->getHands(); ++hand) {
            auto& staff = staffSystem->getStaff(*track, hand);
            staff.updateStaffName();
        }
    }
}

void
Score::sortSystemStaves()
{
    const auto& systems = mScoreLayout->getStaffSystems();
    for (const auto& system : systems) {
        system->reconcile(mTracks);
    }
}


std::map<std::string, Track*, StringViewComparator>
Score::getExistingTracks()
{
    // Create a lookup table of tracks based on unique id
    std::map<std::string, Track*, StringViewComparator> existingTracks;
    
    for (const auto& track : mTracks) {
        existingTracks[std::string(track->getUuid())] = track.get();
    }
    
    return existingTracks;
}


TrackInfoVec
Score::getTracksToAdd(const ScoreInfo& inInfo,
                      const std::map<std::string, Track*, StringViewComparator>& inExistingTracksLookup)
{
    TrackInfoVec tracksToAdd;
    
    for (const auto& trackInfo : inInfo.trackInfoList) {
        const auto guid = trackInfo.getUuid();
        const auto iter = inExistingTracksLookup.find(guid);
        if (iter == inExistingTracksLookup.end()) {
            tracksToAdd.push_back(trackInfo);
        }
    }
    
    return tracksToAdd;
}


std::vector<Track*>
Score::getTracksToKeep(const ScoreInfo& inInfo,
                       const std::map<std::string, Track*, StringViewComparator>& inTracks)
{
    std::vector<Track*> tracksToKeep;
    
    for (const auto& trackInfo : inInfo.trackInfoList) {
        const auto iter = inTracks.find(trackInfo.getUuid());
        if (iter != inTracks.end()) {
            tracksToKeep.push_back(iter->second);
        }
    }
    
    return tracksToKeep;
}


std::vector<Track*>
Score::getTracksToRemove(const std::vector<Track*>& inTracksToKeep)
{
    std::vector<Track*> tracksToRemove;
    
    for (const auto& track : mTracks) {
        tracksToRemove.emplace_back(track.get());
    }
    
    for (const auto& keepTrack : inTracksToKeep) {
        
        tracksToRemove.erase(std::remove(tracksToRemove.begin(),
                                         tracksToRemove.end(),
                                         keepTrack),
                             tracksToRemove.end());
    }
    
    return tracksToRemove;
}


Score::TrackChanges
Score::getTrackChanges(const ScoreInfo& inInfo)
{
    TrackChanges changes;
    changes.scoreInfo = inInfo;
    changes.existing = getExistingTracks();
    changes.add = getTracksToAdd(inInfo, changes.existing);
    changes.keep = getTracksToKeep(inInfo, changes.existing);
    changes.remove = getTracksToRemove(changes.keep);
    return changes;
}


void
Score::configureStaffSystemBrackets(const ScoreInfo& inInfo)
{
    mScoreLayout->clearBrackets();
    for (auto bracket : inInfo.systemBracketInfoList) {
        mScoreLayout->addBracket(std::move(bracket));
    }
}


void
Score::makeSureAllTheMeasuresAreCreated()
{
    const auto& systems = mScoreLayout->getStaffSystems();
    for (const auto& system : systems) {
        for (int index = 0; index < system->getMeasureCount(); ++index) {
            validateMeasure(system->getFirstMeasure() + index);
        }
    }
}


void
Score::checkStaffSystemIntegrity() const
{
    const auto& currentSystems = getStaffSystems();
    const auto handCount = getHandCount();
    
    for (const auto& system : currentSystems) {
        KOMP_ASSERT( static_cast<int>(system->getStaves().size()) == handCount );
    }
}

void
Score::applyLayout(const bool inNewScore, const ScoreInfo& inInfo)
{
    setTrackNames(inInfo.trackInfoList);
    layoutMeasures();
    indentStaves();
    
    setDirty(true);
    
    if (!inNewScore) {
        layout();
    }
}

/// \brief Apply the score info to the score.
/// \discussion Pre-condition: The trackInfo list uuid's must be unique.
/// \param inInfo The ScoreInfo
void
Score::applyScoreInfo(const ScoreInfo& inInfo, bool isUndoable)
{
    const auto lock = getLock();
    
    validateScoreInfo(inInfo);
    
    // Fix inconsistencies
    auto scoreInfo = convertExistingScoreToLinear(inInfo);
    
    const auto isNewScore = getIsNewScore();
    handleLayoutAndMeasureCounts(isNewScore, scoreInfo);
    applyScoreMetadata(scoreInfo.metadata);
    configureTracks(isNewScore, scoreInfo);
    configureStaffSystemBrackets(scoreInfo);
    makeSureAllTheMeasuresAreCreated();
    checkStaffSystemIntegrity();
    
    if (inInfo.showTempoMark) {
        moveTempoMarks();
    } else {
        removeTempoMarks();
    }
    
    auto scoreCommand = mCommandFactory->genericCommand("Configure Score");
    auto& measure = getMeasuresFront();
    
    if (inInfo.showTempoMark) {
        const auto& track = getTracks().front();
        auto command = mCommandFactory->tempoCommand(*measure.get(),
                                                     *track,
                                                     0.0,
                                                     scoreInfo.initialTempoMark);
        command->execute();
        scoreCommand->addSubCommand(std::move(command));
    }
    
    {
        auto command = mCommandFactory->keySignatureCommand(*measure.get(),
                                                            scoreInfo.keySignature);
        command->execute();
        scoreCommand->addSubCommand(std::move(command));
    }
    
    {
        auto command = mCommandFactory->changeTimeSignatureCommand(0, inInfo.timeSignature);
        command->execute();
        scoreCommand->addSubCommand(std::move(command));
    }
    
    if (isUndoable) {
        mUndoManager->addCommand(std::move(scoreCommand));
    }
    
    applyLayout(isNewScore, scoreInfo);
}

ScoreInfo
Score::getScoreInfo() const
{
    // Get the general score info
    ScoreInfo info;
    info.measuresPerLine = 3;
    info.linesPerPage = 4;
    info.systemInset = mScoreLayout->getLayoutParams().systemInset;
    info.showTempoMark = mShowTempoMark;
    info.showInstrumentNames = mShowInstrumentNames;
    
    // Get the main tempo
    const auto& measure = getMeasuresFront();
    info.initialTempoMark = measure->getTempoValue();
    
    // Get the intial time signature
    info.timeSignature = measure->getTimeSignatureParams();
    
    // Get the bracket info
    const auto& systems = mScoreLayout->getStaffSystems();
    const auto& staffSystem = systems.front();
    for (const auto& bracket : staffSystem->getBrackets()) {
        info.systemBracketInfoList.push_back(bracket->getInfo());
    }
    
    // Grab the first key signature from the first staff
    // of the first system of the score.
    komp::MusicLocation loc(*measure, *mTracks.front());
    info.keySignature = loc.getKeySignature();
    
    // Get the text info
    const auto& title = mScoreLayout->getTitle(0);
    info.metadata.title = title.Text;
    
    const auto& subtitle = mScoreLayout->getTitle(1);
    info.metadata.subtitle = subtitle.Text;
    
    const auto& composer = mScoreLayout->getComposer(0);
    info.metadata.composer = composer.Text;
    
    const auto& arranger = mScoreLayout->getComposer(1);
    info.metadata.arranger = arranger.Text;
    
    const auto& copyright = mScoreLayout->getCopyRight(0);
    info.metadata.copyright = copyright.Text;
    
    for (const auto& track : mTracks) {
        const TrackInfo trackInfo{track->getInstrument(), track->getUuid()};
        info.trackInfoList.push_back(trackInfo);
    }
    
    return info;
}

/// \brief Factory function to create a score
/// \param info The scor info
/// \result The unique_ptr to the Score
ScoreUPtr
Score::create(const ScoreInfo& info)
{
    if (info.trackInfoList.size() == 0) {
        KOMP_THROW("ScoreInfo has zero tracks - we need at least 1 - hint use minimumScoreInfo()");
    }
    
    return std::make_unique<Score>(info);
}

// MARK: - Layout

/// \brief Update line and measure state to the current score state
void
Score::update()
{
    const auto& systems = mScoreLayout->getStaffSystems();
    if (systems.empty()) {
        return;
    }
    
    Measure* previousMeasure = nullptr;
    
    // Keep track of the last meter state in order to determine if an updated
    // time signature needs to be displayed on the measure
    int lastMeterH = 0;
    int lastMeterL = 0;
    auto lastSymbol = TimeSignatureType::NUMERIC;
    
    for (const auto& system : systems) {
        
        // Make sure the bars exist
        validateMeasure(std::max((size_t)0, system->getLastMeasure()));
        
        // Initialize all staves for this line
        const auto& staves = system->getStaves();
        const auto staffCount = getHandCount();
        KOMP_ASSERT(staffCount <= staves.size());
        
        // Set the starting bar position. This will get updated as we iterate
        // over the measures in a line.
        auto barX = system->getX();
        
        const auto firstMeasure = system->getFirstMeasure();
        const auto lastMeasure = firstMeasure + system->getMeasureCount();
        
        // Handle each measure in the line
        for (auto measureIndex = firstMeasure; measureIndex < lastMeasure; ++measureIndex) {
            
            const auto& measure = getMeasure(static_cast<int>(measureIndex));
            
            if (measureIndex == 0) {
                // Set the values using the first measure
                lastMeterH = measure->getBeats();
                lastMeterL = measure->getBeatUnit();
                lastSymbol = measure->getTimeSignatureType();
            }
            
            // Look for clefs and key signatures
            const auto isFirstMeasure = measureIndex == system->getFirstMeasure();
            
            // Set meter visibility flags for the line and measure
            if (isFirstMeasure) {
                
                auto hasBeatsChanged = true;
                auto hasTimebaseChanged = true;
                auto hasSymbolChanged = true;
                
                if (previousMeasure) {
                    hasBeatsChanged = previousMeasure->getBeats() != measure->getBeats();
                    hasTimebaseChanged = previousMeasure->getBeatUnit() != measure->getBeatUnit();
                    hasSymbolChanged = previousMeasure->getTimeSignatureType() != measure->getTimeSignatureType();
                }
                
                if (!measure->getMeterVisible()) {
                    if (hasBeatsChanged || hasTimebaseChanged || hasSymbolChanged) {
                        // If some property of the meter has changed across measures,
                        // we want to show the meter symbol again
                        measure->setMeterVisible(true);
                    } else {
                        measure->setMeterVisible(false);
                    }
                }
                
            } else {
                
                // Hide the meter until we determine if we need to show a changed time signature
                if (measure->getTimeSignatureType() != TimeSignatureType::NUMERIC) {
                    measure->setMeterVisible(false);
                    if (measure->getBeats() != lastMeterH
                        || measure->getBeatUnit() != lastMeterL
                        || measure->getTimeSignatureType() != lastSymbol) {
                        lastMeterH = measure->getBeats();
                        lastMeterL = measure->getBeatUnit();
                        lastSymbol = measure->getTimeSignatureType();
                        measure->setMeterVisible(true);
                    }
                }
            }
            
            // Set the new measure start location in the line
            measure->setX(barX);
            
            // Shift the start location to the end of the current measure
            barX += measure->getWidth();
            
            // Remember the current measure for the next pass
            previousMeasure = measure.get();
        }
    }
}


/// \brief layoutMeasures()
/// Position measure items at the ideal horizontal position based
/// on clock time and measure width
void
Score::layoutMeasures()
{
    setIsQuiescent(true);
    
    for (const auto& measure : getMeasures()) {
        measure->updateSpacing();
        measure->layout();
    }
    
    setIsQuiescent(false);
}


/// \brief Changes line inset to accomodate staff names
void
Score::indentStaves()
{
    const auto& fontInfo = Preferences::instance().StaffFont;
    const auto scoreInfo = getScoreInfo();
    const auto systemInset = scoreInfo.systemInset;
    
    // Set the default inset amount of the instrument name
    auto nameInset = scoreInfo.systemInset;
    
    if (getShowInstrumentNames()) {
        
        for (const auto& track : mTracks) {
            
            // Adjust for name widths
            if (!track->getInstrument().name.empty()) {
                
                // TODO: Handle long track names
                const auto trackName = track->getInstrument().name.c_str();
                const auto textWidth = TextWidth(fontInfo.Font,
                                                 fontInfo.Size,
                                                 trackName);
                const auto textPlusInset = textWidth + systemInset;
                nameInset = std::max(nameInset, textPlusInset);
            }
        }
    }
    
    mScoreLayout->indentStaves(systemInset,
                               nameInset,
                               getShowInstrumentNames());
}

/// \brief Layout the score
void
Score::layout()
{
    layoutMeasures();
    mScoreLayout->updateSystems();
    mScoreLayout->layout();
    draw();
}

// MARK: - Item Finding

/// \brief Calculates the number of staves up to given track.
/// \discussion This is because a track can have up to 4 staves (hands).
/// \param inTrack The destination track index
/// \result The staff this track appears at
int
Score::getStaffIndex(const Track& inTrack) const
{
    const auto trackIndex = getTrackIndex(inTrack);
    int staffIndex = 0;
    for (auto index = 0; index < trackIndex; ++index) {
        staffIndex += mTracks.at(index)->getHands();
    }
    return staffIndex;
}

/// \brief Return the index of the track in the score
/// \param inTrack The destination track index
/// \result The track index in the score
int
Score::getTrackIndex(const Track& inTrack) const
{
    const auto iter = mTrackHash.find(&inTrack);
    if (iter == mTrackHash.cend()) {
        KOMP_THROW("Unable to locate track");
    }
    return iter->second;
}

// MARK: - Removal

/// \brief Removes the measure.
/// \param inMeasure The measure to delete
MeasureUPtr
Score::deleteMeasure(size_t inMeasure)
{
    const auto measureIndex = static_cast<int>(inMeasure);
    if (measureIndex > getMeasureCount()) {
        KOMP_THROW("Attempting to delete non-existent measure");
    }
    
    auto iter = mMeasures.begin() + inMeasure;
    if (iter == getMeasuresEnd()) {
        KOMP_THROW("Attempting to delete non-existent measure");
    }
    
    const auto& measure = *iter;
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().measureRemoved(*measure.get());
    }
    
    measure->removed();
    
    auto result = std::move(*iter);
    measuresErase(iter);
    
    return result;
}

/// \brief Deletes the given staff from score.
/// \param inTrack The track we delete staff from.
/// \param inHand The hand within track.
void
Score::deleteStaff(Track* inTrack, const int inHand)
{
    // Remove the items from the measure
    for (const auto& measure : getMeasures()) {
        const auto items = measure->items<Item>(*inTrack,
                                                [&](Item& inFilterItem) {
            const auto& loc = inFilterItem.getLocation();
            return loc.getHand() == inHand;
        });
        for (Item& invalidateItem : items) {
            // Invalidate items first so they don't cause chain reactions on removal
            invalidateItem.itemIDInvalidated();
        }
        
        for (const auto& removeItem : items) {
            // Remove the item, but don't bother adjusting item times
            // or updating any internal state
            measure->removeItem(removeItem, false);
        }
        
        for (auto& staffSystem : getStaffSystems()) {
            const auto& staff = staffSystem->getStaff(*inTrack, inHand);
            measure->removeStaff(staff);
        }
    }
    
    // TODO: Deal with brackets
    //const auto& bracketInfo = inTrack->getBracket(inHand);
    
    for (auto& staffSystem : getStaffSystems()) {
        staffSystem->removeStaff(*inTrack, inHand);
    }
    
    inTrack->removeHand(inHand);
}

/// \brief Removes the track from all lines
/// \param inTrack The track to remove
void
Score::deleteTrack(Track* inTrack)
{
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().trackRemoved(*inTrack);
    }
    
    // Remove the hands
    while (inTrack->getHands() > 0) {
        const auto lastHandIndex = inTrack->getHands() - 1;
        deleteStaff(inTrack, lastHandIndex);
    }
    
    // TODO: Remove the staff from the bracket
    
    // Remove the track from the measures
    for (const auto& measure : getMeasures()) {
        measure->removeTrack(*inTrack);
    }
    
    // Remove the track from the score
    const auto trackIter = std::find_if(mTracks.cbegin(),
                                        mTracks.cend(),
                                        [&](const TrackUPtr& iterTrack) {
        return iterTrack.get() == inTrack;
    });
    
    if (trackIter != mTracks.end()) {
        mTracks.erase(trackIter);
        
        repopulateTrackHash();
        
    } else {
        KOMP_THROW("Unable to find track to remove");
    }
}

/// \brief Calculates the number of staves in score and returns the value.
int
Score::calculateStaves() const
{
    int count = 0;
    for (const auto& track : mTracks) {
        count += track->getHands();
    }
    return count;
}

// MARK: - Layout

/// \brief Set the display of staff names
/// \param inShowNames If true, show the names otherwise hide them
void
Score::setShowInstrumentNames(bool inShowNames)
{
    if (mShowInstrumentNames == inShowNames) {
        return;
    }
    
    mShowInstrumentNames = inShowNames;
    indentStaves();
    update();
}

// MARK: - Visibility

/// \brief Deselect all items and barlines
/// \result true is there was a selection
bool
Score::clearSelection()
{
    const auto hadSelection = !mSelection->isEmpty();
    if (hadSelection) {
        mSelection->clear();
    }
    
    const auto hadBarlineSelection = deselectAllBarlines();
    
    return hadSelection || hadBarlineSelection;
}

/// \brief Deselect all barlines in all measures
/// \result true is there was a selection
bool
Score::deselectAllBarlines()
{
    bool hadSelection = false;
    
    // Reset the barline selection
    for (const auto& measure : getMeasures()) {
        if (measure->getBarlineSelection() != BarlineLocation::NONE) {
            measure->setBarlineSelection(BarlineLocation::NONE);
            hadSelection = false;
        }
    }
    
    return hadSelection;
}

void
Score::moveTempoMarks()
{
    const auto& measure = getMeasuresFront();
    for (const auto& track : mTracks) {
        auto tempo = measure->firstMatch<TempoMark>(*track.get(), 0);
        if (tempo) {
            measure->removeItem(*tempo);
        }
    }
}

void
Score::removeTempoMarks()
{
    const auto& measure = getMeasuresFront();
    for (const auto& track : mTracks) {
        auto tempo = measure->firstMatch<TempoMark>(*track.get(), 0);
        if (tempo) {
            measure->removeItem(*tempo);
        }
    }
}

// Voice range: 1-8
static constexpr const auto MinVoice = 0;
static constexpr const auto MaxVoice = 7;

void
Score::setCurrentVoice(int inVoice)
{
    if (inVoice >= MinVoice && inVoice <= MaxVoice) {
        mCurrentVoice = inVoice;
    } else {
        LOG_F(WARNING, "Current voice is out of range.");
    }
}

const MeasureUPtr&
Score::getMeasure(int inMeasureIndex) const
{
    KOMP_ASSERT(inMeasureIndex >= 0);
    KOMP_ASSERT(inMeasureIndex < getMeasureCount());
    return mMeasures.at(inMeasureIndex);
}

const Measure*
Score::getMeasurePtr(int inMeasureIndex) const
{
    KOMP_ASSERT(inMeasureIndex >= 0);
    KOMP_ASSERT(inMeasureIndex < getMeasureCount());
    return mMeasures.at(inMeasureIndex).get();
}

int
Score::getMeasureCount() const
{
    return static_cast<int>(mMeasures.size());
}

int
Score::getMeasureIndex(const Measure& inMeasure) const
{
    auto iter = mMeasureHash.find(&inMeasure);
    if (iter == mMeasureHash.cend()) {
        //LOG_F(WARNING, "Unable to locate measure");
        return -1;
    }
    return iter->second;
}

const MeasureUPtr&
Score::getMeasuresFront() const
{
    KOMP_ASSERT(getMeasureCount() > 0);
    return mMeasures.front();
}

const MeasureUPtr&
Score::getMeasuresBack() const
{
    KOMP_ASSERT(getMeasureCount() > 0);
    return mMeasures.back();
}

MeasureUPtrVec::const_iterator
Score::getMeasuresBegin() const
{
    return mMeasures.cbegin();
}

MeasureUPtrVec::const_iterator
Score::getMeasuresEnd() const
{
    return mMeasures.cend();
}

MeasureUPtrVec::const_iterator
Score::measuresErase(MeasureUPtrVec::const_iterator first,
                     MeasureUPtrVec::const_iterator end)
{
    const auto returnIter = mMeasures.erase(first, end);
    repopulateMeasureHash();
    return returnIter;
}

MeasureUPtrVec::const_iterator
Score::measuresErase(MeasureUPtrVec::const_iterator iter)
{
    const auto outIter = mMeasures.erase(iter);
    repopulateMeasureHash();
    return outIter;
}

void
Score::measuresPushBack(MeasureUPtr inMeasure)
{
    KOMP_ASSERT(inMeasure != nullptr);
    mMeasureHash[inMeasure.get()] = getMeasureCount();
    mMeasures.emplace_back(std::move(inMeasure));
    mMeasures.back()->inserted();
}

MeasureUPtrVec::const_iterator
Score::measuresInsert(MeasureUPtrVec::const_iterator iter,
                      MeasureUPtr inMeasure)
{
    auto result = mMeasures.insert(iter, std::move(inMeasure));
    repopulateMeasureHash();
    const auto& measure = *result;
    measure->inserted();
    return result;
}

void
Score::repopulateMeasureHash()
{
    mMeasureHash.clear();
    int measureIndex= 0;
    for (const auto& measure : getMeasures()) {
        mMeasureHash[measure.get()] = measureIndex++;
    }
}

// MARK: - Measures

/// \brief Makes sure the measure exists.
/// \discussion Will create extra measures if measure index is past the end of score.
/// \param inMeasure The measure index to validate.
void
Score::validateMeasure(size_t inMeasure)
{
    if (inMeasure < getMeasureCount()) {
        // Measure exists and is valid
        return;
    }
    
    const auto span = inMeasure + 1 - getMeasureCount();
    for (auto index = 0; index < span; index++) {
        auto measure = std::make_unique<Measure>(*this);
        if (mMeasures.empty()) {
            measuresPushBack(std::move(measure));
        } else {
            // Keep measure copy, not reference, as measuresPushBack may
            // modify the iterators
            const auto previous = getMeasuresBack().get();
            const auto bpp = previous->getBeats();
            const auto bpu = previous->getBeatUnit();
            measure->setMeter(bpp, bpu);
            measuresPushBack(std::move(measure));
        }
    }
}

// MARK: - Tracks

void
Score::repopulateTrackHash()
{
    mTrackHash.clear();
    int index = 0;
    for (const auto& track : mTracks) {
        mTrackHash[track.get()] = index;
        ++index;
    }
}


int
Score::getHandCount() const
{
    int handCount = 0;
    
    for (const auto& track : getTracks()) {
        handCount += track->getHands();
    }
    
    return handCount;
}

// MARK: - Drawing

void
Score::draw()
{
    const auto lock = getLock();
    mScoreLayout->draw(mContext);
}

bool
Score::getIsQuiescent() const
{
    return mItemRegistry.getIsQuiescent();
}


void
Score::setIsQuiescent(bool inValue)
{
    getItemRegistry().setIsQuiescent(inValue);
}

void
Score::updateItems()
{
    for (const auto& measure : getMeasures()) {
        for (Item& item : measure->allItems()) {
            item.update();
            ItemImpl::setNeedsDisplay(item);
        }
    }
}

// MARK: - Score Navigation

/// \brief Get all of the lines in the score layout
/// \result A list of all the lines in the score layout
const std::vector<StaffSystemUPtr>&
Score::getStaffSystems() const
{
    return mScoreLayout->getStaffSystems();
}

// MARK: - Observation

void
Score::registerObserver(ScoreObserver& inObserver)
{
    auto predicate = [&](ScoreObserverRef& observer) {
        return &observer.get() == &inObserver;
    };
    
    auto iter = std::find_if(mObservers.begin(),
                             mObservers.end(),
                             predicate);
    
    if (iter == mObservers.end()) {
        mObservers.emplace_back(inObserver);
    }
}

void
Score::unregisterObserver(ScoreObserver& inObserver)
{
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        if (&observer.get() == &inObserver) {
            it = mObservers.erase(it);
            --it;
            continue;
        }
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
