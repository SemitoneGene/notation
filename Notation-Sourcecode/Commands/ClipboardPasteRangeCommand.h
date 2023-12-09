// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "Forward.h"
#include "ItemRegistry.h"
#include "PasteParams.h"
#include "Score.h"


namespace komp
{
    /// \brief  Paste clipboard items at the measure
    ///
    class ClipboardPasteRangeCommand : public Command
    {
    public:
        ClipboardPasteRangeCommand(CommandFactory& inFactory,
                                   ItemRegistry& inRegistry,
                                   Score& inScore,
                                   PasteRangeParams inPasteParams);
        
        virtual ~ClipboardPasteRangeCommand() = default;
        
        virtual void undo() override;
        virtual void redo() override;

    private:
        class Data;

        void resetData(Measure* inOrigMeasure,
                       Measure* inDestMeasure,
                       Data& data);

        void copyStaves(Data& data);
        void deleteItems(Data& data);
        void copyItems(Data& data);
        TemporalRefVec getTemporals(Data& data);
        void stripClone(Data& data);
        void copyTemporal(Data& data);
        void parseChord(Data& data);
        void parseBeam(Data& data);
        void parseTuplet(Data& data);
        void insertClone(Data& data);
        void insertChord(Data& data);
        void insertBeam(Data& data);
        void insertTuplet(Data& data);
        void insertMarks();
        void copyChordSymbols(const Data& data);
        void enqueueSpanners(Data& data);
        void copySpanners(const Data& data);
        void copyWedges(const Data& data);

    private:

        struct Current
        {
            Temporal* origTemporal;
            Temporal* destTemporal;
            std::vector<Note*> chorded;
            std::vector<Temporal*> beamed;
            TupletIntf* tuplet;
            std::vector<Temporal*> tupleted;

            Current()
            : origTemporal{nullptr}
            , destTemporal{nullptr}
            , chorded{}
            , beamed{}
            , tuplet{nullptr}
            , tupleted{}
            {

            }
            
            bool isEmpty() const
            {
                return chorded.empty()
                    && tupleted.empty()
                    && beamed.empty()
                    && !tuplet;
            }
        };

        struct SpannerSpec
        {
            SpannerSpec()
            {
            }
            
            Temporal* originalTemporalStart;
            Temporal* clonedTemporalStart;
            const Item* spanner;
            Temporal* originalTemporalEnd;
            Temporal* clonedTemporalEnd;
        };

        struct Data
        {
            PasteRangeParams params;
            ItemRegistry& registry;
            CommandFactory& factory;
            Measure* origMeasure;
            Measure* destMeasure;
            Staff* origStaff;
            Staff* destStaff;
            int voice;
            Current current;
            std::vector<SpannerSpec> spannersFound;
            bool isTopStaffSelected;
            bool isTopStaffInDestination;

            Data(CommandFactory& inFactory,
                 ItemRegistry& inRegistry,
                 PasteRangeParams& inParams)
            : params{inParams}
            , registry{inRegistry}
            , factory{inFactory}
            , origMeasure{}
            , destMeasure{}
            , origStaff{}
            , destStaff{}
            , voice{0}
            , spannersFound{}
            , isTopStaffSelected{false}
            , isTopStaffInDestination{false}
            {

            }
        };

        struct MarkMemo
        {
            Temporal& clone;
            MarkType markType;
            double temporalRelativeX;
            double temporalRelativeSlots;
            PlacementType placementType;

            MarkMemo() = delete;
            
            MarkMemo(Temporal& inClone,
                     MarkType inType,
                     double inX,
                     double inSlots,
                     PlacementType inPlacement)
            : clone{inClone}
            , markType{inType}
            , temporalRelativeX{inX}
            , temporalRelativeSlots{inSlots}
            , placementType{inPlacement}
            {

            }
        };

    private:
        virtual CommandResult executeImpl() override;
        
        Score& mScore;
        PasteRangeParams mParams;
        std::vector<MarkMemo> mMarkMemos;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
