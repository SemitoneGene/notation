// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>

// Local
#include "ClefType.h"
#include "DrawableText.h"
#include "Forward.h"
#include "Spatial.h"

namespace komp
{
    struct ClefSymbolInfo
    {
        ClefSymbolType type;
        
        // the for the line that the clef is delineating, how many diatonic
        // steps is that pitch from middle C?  For example the G-clef
        // delineates the G above middle C which is _4_ slots from middle-c
        // the C-Clef delineates middle c, which is _0_ slots from middle-c
        // the F-Clef delineates the f below middle c, which is _-4_
        // Perc Clef basically mirrors the G-Clef, which is _4_
        // Tab Clef, I'm not sure so copying the G-Clef
        int slotDistanceFromMiddleC;
        
        // the most common staff line for the symbol to delineate, e.g.
        // G clef most commonly delineates line 2, F clef usually delineates
        // line 4, and C clef normally delineates line 3.
        int standardDelineationLine;
        
        // converts the standard delineation line value into a slot value
        inline int standardDelineationSlot() const { return standardDelineationLine * 2; }
        
        ClefSymbolInfo(ClefSymbolType inClefSymbolType,
                       int inSlotDistanceFromMiddleC,
                       int inStandardDelineationLine)
        : type(inClefSymbolType)
        , slotDistanceFromMiddleC(inSlotDistanceFromMiddleC)
        , standardDelineationLine(inStandardDelineationLine)
        {}
    };
    
    struct ClefInfo
    {
        std::string name;
        
        // the slot at which we would find middle-C (C4, midiNote 60)
        // examples...
        // Treble:      0
        // Viola:       6
        // Bass:       12
        // Treble 8va: -7
        inline int middleCSlot() const
        {
            return (delineationSlot() - clefSymbolInfo.slotDistanceFromMiddleC) - (octaveDisplacement * 7);
        }
        
        // the line on a 5-line staff which the clef is defining
        // for example, treble clef defines line *2* as G, bass
        // clef defines line *4* as F
        int delineationLine;
        
        // the slot of the delineation line
        inline int delineationSlot() const { return delineationLine * 2; }
        
        ClefSymbolInfo clefSymbolInfo;
        
        // -2 = 15mb, -1 = 8vb, 0 = loco, 1 = 8va, 2 = 15ma
        int octaveDisplacement;
        
        // the octave for which a C would be written somewhere
        // inside the staff. For example treble C4, bass C3, alto C4
        int octaveForClef;
        
        ClefInfo(std::string inName,
                 int inDelineationLine,
                 ClefSymbolInfo inClefSymbolInfo,
                 int inOctaveDisplacement,
                 int inOctaveForClef)
        : name(inName)
        , delineationLine(inDelineationLine)
        , clefSymbolInfo(inClefSymbolInfo)
        , octaveDisplacement(inOctaveDisplacement)
        , octaveForClef(inOctaveForClef)
        {}
    };
    
    
    class KOMP_EXPORT Clef : public Spatial
    {
    public:        

        virtual ~Clef() {}

        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void update() override final;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override {}
        virtual void inserted() override;
        virtual void removed()override;
        virtual void removeSelf() override;
        virtual Rect getFrame() const override;
        inline virtual Region getRegion() const override;
        inline virtual Point getAnchor() const override { return POINT_ZERO; }

        virtual Point getDrawPoint() const override final;
        virtual double getScoreX() const override;
        virtual double getScoreY() const override;

        ClefType type() const;
        
        const ClefInfo& clefInfo() const;
        
        static const ClefInfo& clefInfo(ClefType clefType);
        static const std::map<ClefType, ClefInfo> ClefInfoTable;
        
        bool isSmall() const { return mSmall; }
        
    private:
        friend class DrawableClef;
        friend class ItemRegistry;
        
        Clef() = delete;

        Clef(const MusicLocation& inLocation,
             ClefType inType,
             bool inSmall);

        ///@{ Item
        virtual void createDrawable() override;
        ///@}

        const ClefData& findClefData() const;

        ClefType mType;
                
        bool mSmall;
    };
    
    inline ClefType Clef::type() const { return mType; }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
