// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <vector>

// Local
#include "CommandFactory.h"
#include "CurrentState.h"
#include "DrawableContext.h"
#include "Forward.h"
#include "ItemRegistry.h"
#include "MeasureNumberInfo.h"
#include "ScoreLayout.h"
#include "ScoreObserver.h"
#include "Selection.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "ScoreCreationInfo.h"
#include "StringViewComparator.h"
#include "UndoManager.h"

namespace komp
{
using DrawableRegistry = std::map<ItemID, DrawableUPtr>;
using RecursiveLock = std::unique_lock<std::recursive_mutex>;
using RecursiveLockPtr = std::unique_ptr<RecursiveLock>;

class ScoreLock
{
public:
    ScoreLock(RecursiveLockPtr&& inLock);
    ScoreLock();
    ~ScoreLock();
    
private:
    RecursiveLockPtr mLock;
};


class Score
{
public:
    static ScoreUPtr create(const ScoreInfo& info);
    
    Score(const ScoreInfo& inInfo);
    
    Score() = delete;
    Score(const Score& other) = delete;
    
    ~Score() = default;
    
    Score& operator=(const Score& other) = delete;
    Score(Score&&) = delete;
    Score& operator=(Score&&) = delete;
    
    void loaded() const;
    
    /// When an Item is Quiescent it will refrain from sending or responding
    /// to ItemObserver notifications. A Score is Quiescent when all of
    /// its inserted items are quiescent as well as all of its orphaned items.
    /// All Items must be polled to determine.
    bool getIsQuiescent() const;
    
    /// When an Item is Quiescent it will refrain from sending or responding
    /// to ItemObserver notifications.  A Score is Quiescent when all of
    /// its inserted items are quiescent as well as all of its orphaned items.
    void setIsQuiescent(bool inValue);
    
    /// Tell all the items in the score to re-calculate themselves.
    void updateItems();
    
    ScoreLock getLock() const;
    ItemRegistry& getItemRegistry();
    CommandFactory& getCommandFactory() const;
    UndoManager& getUndoManager() const;
    DrawableRegistry& getDrawableRegistry();
    
    // Creation
    ScoreInfo getScoreInfo() const;
    
    // Current state
    CurrentState findPosition(const Point& inPoint);
    CurrentState getCurrentState(const Point& inPoint) const;
    CurrentState getCurrentState(const Point& inPoint,
                                 Staff* inStaff) const;
    
    // Updating
    void update();
    
    void indentStaves();
    
    // Drag
    Item* handleTap(const Point& inPoint) const;
    
    // Deletion
    MeasureUPtr deleteMeasure(size_t inMeasure);
    void deleteStaff(Track* inTrack, const int inHand);
    void deleteTrack(Track* inTrack);
    
    // Insertion
    void insertStaff(Track& inTrack,
                     ClefType inClefType,
                     int inLineCount,
                     int inHand);
    
    Measure* insertMeasure(size_t inIndex);
    void insertMeasure(MeasureUPtr inMeasure, size_t inIndex);
    
    void layout();
    
    int calculateStaves() const;
    
    int getStaffIndex(const Track& inTrack) const;
    int getTrackIndex(const Track& inTrack) const;
    
    //@{ @name Measure Access
    int getMeasureCount() const;
    int getMeasureIndex(const Measure& inMeasure) const;
    const MeasureUPtr& getMeasuresFront() const;
    const MeasureUPtr& getMeasuresBack() const;
    MeasureUPtrVec::const_iterator getMeasuresBegin() const;
    MeasureUPtrVec::const_iterator getMeasuresEnd() const;
    const std::vector<MeasureUPtr>& getMeasures() const { return mMeasures; }
    const MeasureUPtr& getMeasure(int inMeasureIndex) const;
    const Measure* getMeasurePtr(int inMeasureIndex) const;
    //@}
    
    /// \brief vector::erase
    MeasureUPtrVec::const_iterator measuresErase(MeasureUPtrVec::const_iterator first,
                                                 MeasureUPtrVec::const_iterator end);
    
    /// \brief vector::erase
    MeasureUPtrVec::const_iterator measuresErase(MeasureUPtrVec::const_iterator iter);
    
    TrackUPtrVec& getTracks() { return mTracks; }
    const TrackUPtrVec& getTracks() const { return mTracks; }
    
    /// \brief Get the total number of hands in the score, e.g. a score with Piano and Violin would return 3
    int getHandCount() const;
    
    void draw();
        
    void validateMeasure(size_t inIndex);
    
    size_t userMeasureNumber(size_t inMeasure) const;
    
    const MeasureNumberInfo& measureNumberInfo() const { return mMeasureNumberInfo; }
    void setMeasureNumberInfo(const MeasureNumberInfo& inInfo) { mMeasureNumberInfo = inInfo; }
    
    bool getShowInstrumentNames() const { return mShowInstrumentNames; }
    void setShowInstrumentNames(bool inShowNames);
    
    void setTempoHidden(bool inHidden);
    
    int timeBase() const { return roundToInt(TIME_BASE); }
    
    const std::unique_ptr<Selection>& getSelection() { return mSelection; }
    const Selection* getSelectionPtr() { return mSelection.get(); }
    bool clearSelection();

    const std::unique_ptr<ScoreLayout>& getLayout() const { return mScoreLayout; }
    const std::vector<StaffSystemUPtr>& getStaffSystems() const;
    
    void setDirty(bool inDirty) { mDirty = inDirty; }
    bool getDirty() const { return mDirty; }
    
    DrawableContext& getContext() { return mContext; }
    
    void applyScoreInfo(const ScoreInfo& inInfo, bool isUndoable);
    void applyScoreMetadata(const ScoreMetadata& inMetadata);
    
    void registerObserver(ScoreObserver& inObserver);
    void unregisterObserver(ScoreObserver& inObserver);

    int getCurrentVoice() const { return mCurrentVoice; }
    void setCurrentVoice(int inVoice);

private:
    struct TrackChanges
    {
        ScoreInfo scoreInfo;
        std::map<std::string, Track*, StringViewComparator> existing;
        TrackInfoVec add;
        std::vector<Track*> keep;
        std::vector<Track*> remove;
    };
    
    void validateScoreInfo(const ScoreInfo& inInfo);
    bool getIsNewScore() const;
    bool areTrackUuidsUnique(const ScoreInfo& inInfo) const;
    void checkTrackUuids(const ScoreInfo& inInfo) const;
    void createScoreLayout(const ScoreInfo& inInfo);
    ScoreInfo convertExistingScoreToLinear(const ScoreInfo& inInfo) const;
    void addMeasuresToExistingScore(const ScoreInfo& inInfo);
    void handleLayoutAndMeasureCounts(const bool inNewScore, const ScoreInfo& inInfo);
    void configureTracks(const bool inNewScore, const ScoreInfo& inInfo);
    void configureNewScoreTracks(const ScoreInfo& inInfo);
    void deleteAndAddTracks(const ScoreInfo& inInfo);
    std::map<std::string, Track*, StringViewComparator> getExistingTracks();
    TrackInfoVec getTracksToAdd(const ScoreInfo& inInfo,
                                const std::map<std::string, Track*, StringViewComparator>& inTracks);
    std::vector<Track*> getTracksToKeep(const ScoreInfo& inInfo,
                                        const std::map<std::string, Track*, StringViewComparator>& inExistingTracksLookup);
    std::vector<Track*> getTracksToRemove(const std::vector<Track*>& inTracksToKeep);
    TrackChanges getTrackChanges(const ScoreInfo& inInfo);
    void reconcileKeySignatures();
    void sortTracksToUserOrder(const ScoreInfo& inInfo);
    void setTrackNames(const TrackInfoList& inTrackInfoList);
    
    void sortSystemStaves();
    void configureStaffSystemBrackets(const ScoreInfo& inInfo);
    void makeSureAllTheMeasuresAreCreated();
    void checkStaffSystemIntegrity() const;
    void configureTempo(const ScoreInfo& inInfo);
    void applyLayout(const bool inNewScore, const ScoreInfo& inInfo);
    bool deselectAllBarlines();
    void insertTrack(const TrackInfo& inInfo, int inTrackIndex);
    void layoutMeasures();
    void measuresPushBack(MeasureUPtr inMeasure);
    MeasureUPtrVec::const_iterator measuresInsert(MeasureUPtrVec::const_iterator iter,
                                                  MeasureUPtr inMeasure);
    void repopulateMeasureHash();
    void repopulateTrackHash();
    
    MeasureUPtr findLeftRepeat(const MeasureUPtr& inMeasure) const;
    
    void moveTempoMarks();
    void removeTempoMarks();
        
private:
    friend class ScoreLock;
    
    bool mDirty;
    std::unique_ptr<UndoManager> mUndoManager;
    ItemRegistry mItemRegistry;
    DrawableRegistry mDrawableRegistry;
    std::unique_ptr<CommandFactory> mCommandFactory;
    mutable std::recursive_mutex mMutex;
    MeasureNumberInfo mMeasureNumberInfo;
    bool mShowInstrumentNames;
    bool mShowTempoMark;
    int mCurrentVoice{0};
    
    std::vector<MeasureUPtr> mMeasures;
    std::map<const Measure*, int> mMeasureHash;
    
    TrackUPtrVec mTracks;
    std::map<const Track*, int> mTrackHash;
    
    std::unique_ptr<Selection> mSelection;
    std::unique_ptr<ScoreLayout> mScoreLayout;
    
    ScoreObserverRefVec mObservers;
    
    DrawableContext mContext;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
