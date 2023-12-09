// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "MxConverter.h"
#include "MxLocation.h"
#include "MxSpannerCollector.h"
#include "StringViewComparator.h"
#include "Throw.h"
#include "Version.h"

// Mx
#include "mx/api/ScoreData.h"

namespace komp
{
    class Score;

    struct MxDirection
    {
        MxLocation location;
        mx::api::DirectionData direction;
        std::map<int, mx::api::VoiceData> voiceMap;
        MxDirection() : location{MxLocationParameters{}}, direction{} {}
    };

    using NoteMxNotePair = std::pair<Temporal*, mx::api::NoteData>;
    using NoteMxNoteMap = std::map<Temporal*, mx::api::NoteData>;


    class MxScoreReader
    {
    public:
        MxScoreReader(const mx::api::ScoreData& inData);
        ScoreUPtr read();
        Score& getScore() const { KOMP_ASSERT(mScore); return *mScore; }
        MxSpannerCollector& getSpannerCollector();
        FileMessages& getFileMessages();
        const mx::api::ScoreData& getScoreData() const;
        MxLocation createMxLocationObject() const;
        const MeasureUPtr& getKompMeasure(int kompMeasureIndex);
        Track* getTrack(int trackIndex);
        MxConverter getMxConverter() const;
        bool getIsKompFile() const;
        Version getKompVersion() const;
        double getIsScalingFound() const;
        double getMillimeters() const;
        double getTenths() const;
        double getSpaceSize() const;
        double defaultYToUnscaledScoreY(double inDefaultY,
                                        const Staff& inStaff) const;

        /// convert defaultX to item->x, the measureWidthMap must be
        /// populated for inMeasure or it will throw
        double measureDefaultXToMeasureX(double inDefaultX,
                                         const Staff& inStaff,
                                         const Measure& inMeasure) const;

        /// a direction to be processed after the rest of the score
        /// has been processed, i.e. Pedal marks which should be attached
        /// to notes in Komp but are not attached to notes in MusicXML
        void addDirection(MxDirection&& inMxDirection);

        /// measure widths in score pixels
        std::map<int, double> measureWidthMap;

        /// a map of all notes in the score along with the MusicXML
        /// information that was used to create them
        NoteMxNoteMap noteMxNoteMap;

    private:
        ScoreUPtr allocateScore();
        std::map<int, int> findMultiMeasureRests() const;
        void importScore(ScoreUPtr& inScore);
        void importScoreMetadata(ScoreUPtr& inScore);
        void importPageLayout();
        void importLineWidths();
        void importGroups();
        void importSystems();
        void autoSetupSystems();
        void importMeasures();
        Version parseKompVersion() const;
        void importKompVersion();
        void importUnhandledDirections();

        void importDirectionMark(Temporal& inTemporal,
                                 const mx::api::MarkData& inMark,
                                 bool inIsXCalculated,
                                 double inXRelativeToTemporal,
                                 bool inIsYCalculated,
                                 double inScoreY);

        bool importExpressionWords(Temporal& inTemporal,
                                   const mx::api::WordsData& inWord,
                                   bool inIsXCalculated,
                                   double inXRelativeToTemporal,
                                   bool inIsYCalculated,
                                   double inScoreY);

        Temporal& findNearestTemporal(const MusicLocation& inMusicLocation);

        void importFirstSystemPositioning();
        bool importFirstSystemY(const std::vector<mx::api::MiscellaneousField>& inFields, double& ioFirstSystemY) const;
        bool importFirstSystemStaffY(const std::vector<mx::api::MiscellaneousField>& inFields, std::vector<double>& ioStaffYVec) const;

        mx::api::NoteData* findNearestMxNoteForDirection(MxDirection& inMxDirection);

        bool calculateDirectionPosition(Temporal& inTemporal,
                                        const mx::api::PositionData& inPositionData,
                                        const MusicLocation& inMusicLocation,
                                        bool& ioIsXCalculated,
                                        double& ioXRelativeToTemporal,
                                        bool& ioIsYCalculated,
                                        double& ioScoreY) const;
        
        void calculateAttributesForVoice() const;
        
        Score* mScore{nullptr};
        const mx::api::ScoreData& mMxScoreData;
        FileMessages mFileMessages;
        MxSpannerCollector mMxSpannerCollector;
        std::map<int, int> mMultiMeasureRests;
        const int mTotalMxMeasures;
        std::unique_ptr<MxConverter> mMxConverter;
        bool mIsScalingFound{false};
        double mMillimeters{7.0};
        double mTenths{40.0};
        bool mIsKompFile{false};
        Version mKompVersion{0, 0, 0};
        std::vector<MxDirection> mDirections;
        std::map<std::string, std::string, StringViewComparator> mProperties;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
