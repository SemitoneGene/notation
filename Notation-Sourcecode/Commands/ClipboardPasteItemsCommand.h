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
class ClipboardPasteItemsCommand : public Command
{
public:
    ClipboardPasteItemsCommand(CommandFactory& inFactory,
                               ItemRegistry& inRegistry,
                               Score& inScore,
                               PasteItemsContext inPasteContext);
    
    virtual ~ClipboardPasteItemsCommand() = default;
    
    virtual void undo() override;
    virtual void redo() override;
    
private:
    class Data;
    
    void insertTime(Data& inData);
    void copyItems(Data& inData);
    void stripClone(Data& inData);
    void copyTemporal(Data& inData);
    void parseChord(Data& inData);
    void parseBeam(Data& inData);
    void parseTuplet(Data& inData);
    void insertClone(Data& inData);
    void insertChord(Data& inData);
    void insertBeam(Data& inData);
    void insertTuplet(Data& inData);
    void insertMarks();
    void copyChordSymbols(const Data& inData);
    void enqueueSpanners(Data& inData);
    void copySpanners(const Data& inData);
    void copyWedges(const Data& inData);
    void fixTimes(const Data& inData);
    void setTime(const Data& inData, const TemporalRefVec& inTemporals);

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
        PasteItemsContext context;
        ItemRegistry& registry;
        CommandFactory& factory;
        Measure* measure;
        Staff* staff;
        int voice;
        Current current;
        double startX;
        std::vector<SpannerSpec> spannersFound;
        
        Data(CommandFactory& inFactory,
             ItemRegistry& inRegistry,
             PasteItemsContext& inContext)
        : context{inContext}
        , registry{inRegistry}
        , factory{inFactory}
        , measure{&inContext.measure.get()}
        , staff{&inContext.staff.get()}
        , voice{0}
        , startX{inContext.startX}
        , spannersFound{}
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
    PasteItemsContext mContext;
    std::vector<MarkMemo> mMarkMemos;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
