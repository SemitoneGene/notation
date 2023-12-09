// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <array>

// Local
#include "Constants.h"
#include "Forward.h"
#include "Geometry.h"
#include "Text.h"

namespace komp
{
    struct LayoutParams
    {
        double systemInset{0.0};
        double marginTop{100.0};
        double marginBottom{200.0};
        double marginRight{200.0};
        double interstaffSpace{50.0};
        double interstaffSpaceMin{10.0};
    };

    class ScoreLayout
    {
    public:
        ScoreLayout(Score& inScore, const ScoreInfo& inInfo);
        ~ScoreLayout() = default;
        
        void layout();
        
        // Drawing
        void draw(const DrawableContext& inContext);

        Size getContentSize() const;
        
        // Brackets
        void clearBrackets();
        void addBracket(const SystemBracketInfo& inInfo);

        // Lines
        void updateSystems();
        const std::vector<StaffSystemUPtr>& getStaffSystems() const { return mStaffSystems; }
                        
        void updateLayoutInfo(const ScoreLayoutInfo& layout);
        
        // Staves
        void indentStaves(double inSystemInset,
                                  double inNameInset,
                                  bool inShowName);
        Staff* findStaff(const Point& inPoint) const;

        // Score text items
        void setTitle(std::string_view inText, size_t inLine);
        const TextRecord& getTitle(size_t inLine) const;
        const std::vector<TextRecord>& getTitle() const { return mTitleText; }
        
        void setComposer(std::string_view inText, size_t inLine);
        const TextRecord& getComposer(size_t inLine) const;
        const std::vector<TextRecord>& getComposer() const { return mComposerText; }

        void setInstructions(std::string_view inText, size_t inLine);
        const TextRecord& getInstructions(size_t inLine) const;
        const std::vector<TextRecord>& getInstructions() const { return mInstructionsText; }

        void setHeader(std::string_view inText, size_t inLine);
        const TextRecord& getHeader(size_t inLine) const;
        const std::vector<TextRecord>& getHeader() const { return mHeaderText; }

        void setFooter(std::string_view inText, size_t inLine);
        const TextRecord& getFooter(size_t inLine) const;
        const std::vector<TextRecord>& getFooter() const { return mFooterText; }

        void setCopyright(std::string_view inText, size_t inLine);
        const TextRecord& getCopyRight(size_t inLine) const;
        const std::vector<TextRecord>& getCopyRight() const { return mCopyRightText; }
        
        const LayoutParams& getLayoutParams() const { return mLayoutParams; }
        
    private:
        void insertSystem(size_t inSystem, const ScoreInfo& inInfo);

        void initTextInfo();
        
        using RectList = std::vector<Rect>;
        using StaffMetrics = std::vector<std::pair<Staff*, RectList>>;
        
        StaffMetrics collectMetrics() const;
        void getExtents(const RectList& inExtents,
                        double& outTop,
                        double& outBottom) const;
        
        Score& mScore;
        std::vector<StaffSystemUPtr> mStaffSystems;
        LayoutParams mLayoutParams;
        
        std::vector<TextRecord> mTitleText = {
            TextRecord(),
            TextRecord(),
            TextRecord()
        };
        
        std::vector<TextRecord> mComposerText = {
            TextRecord(),
            TextRecord(),
            TextRecord(),
            TextRecord()
        };
        
        std::vector<TextRecord> mInstructionsText = {
            TextRecord(),
            TextRecord(),
            TextRecord()
        };
        
        std::vector<TextRecord> mHeaderText = {
            TextRecord(),
            TextRecord()
        };
        
        std::vector<TextRecord> mFooterText = {
            TextRecord(),
            TextRecord()
        };
        
        std::vector<TextRecord> mCopyRightText = {
            TextRecord(),
            TextRecord(),
            TextRecord(),
            TextRecord(),
            TextRecord(),
            TextRecord()
        };
        
        DrawableString mTitle;
        DrawableString mComposer;
        DrawableString mArranger;
                
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

