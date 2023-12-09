// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


#pragma once

// Local
#include "DrawableComposite.h"
#include "Spanner.h"

namespace komp
{    
    enum class OctaveType
    {
                    //   symbol |shft | char   | name
                    //   ---------------------------------------------
        O = 0,      ///< '8'    | +12 | U+E510 | ottava
        O_ALTA,     ///< '8va'  | +12 | U+E511 | ottavaAlta
        O_BASSA,    ///< '8va'  | -12 | U+E512 | ottavaBassa
        O_BASSA_BA, ///< '8ba'  | -12 | U+E513 | ottavaBassaBa
        O_BASSA_VB, ///< '8vb'  | -12 | U+E51C | ottavaBassaVb
        Q,          ///< '15'   | +24 | U+E514 | quindicesima
        Q_ALTA,     ///< '15ma' | +24 | U+E515 | quindicesimaAlta
        Q_BASSA,    ///< '15ma' | -24 | U+E516 | quindicesimaBassa
        Q_BASSA_MB, ///< '15mb' | -24 | U+E51D | quindicesimaBassaMb
        V,          ///< '22'   | +36 | U+E514 | ventiduesima
        V_ALTA,     ///< '22ma' | +36 | U+E518 | ventiduesimaAlta
        V_BASSA,    ///< '22ma' | -36 | U+E519 | ventiduesimaBassa
        V_BASSA_MB, ///< '22mb' | -36 | U+E51E | ventiduesimaBassaMb
        UNKNOWN
    };

    class KOMP_EXPORT Octave : public Spanner
    {
    public:        
        
        virtual ~Octave() = default;
        
        virtual Octave& operator=(const Octave& other);
        Octave(const Octave& inOctave);
        
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override {}
        
        virtual Rect getFrame() const override;
        inline virtual Region getRegion() const override { return {}; }
        inline virtual Point getAnchor() const override { return POINT_ZERO; }

        virtual Point getDrawPoint() const override;

        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        
        uint8_t getOctave() const { return mOctave; }

    protected:
        virtual Point calculateDragOffset() const override;

    private:
        friend class ItemRegistry;
        Octave() = delete;
        Octave(const MusicLocation& inLocation,
               const SpannerParams& inParams,
               OctaveType inType);
        
        /// operator=() implementation
        void copyState(const Octave& other);
        
        void init();
        
        virtual void createDrawable() override;
        
        OctaveType mType;
        PlacementType mPlacement;
        uint8_t mOctave;
        
        mutable Rect mControlLeft;
        mutable Rect mControlRight;
        
        DrawableComposite mDrawables;

        enum class ResizeType
        {
            LEFT,
            RIGHT,
            NONE
        };
        
        ResizeType mResizeType;
        static const std::map<OctaveType, int> sShiftMap;
    };
        
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
