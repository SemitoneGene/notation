// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Curve.h"

namespace komp
{
    class KOMP_EXPORT Tie : public Curve
    {
    public:
        virtual ~Tie() {}
        
        virtual void inserted() override;
        
        virtual const ItemTraits& getItemTraits() const override;
        
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        
    private:
        friend class ItemRegistry;
        Tie() = delete;
        
        // TODO: We need to support ties that start on a note but "float"
        // without terminating on a note.  Here are some examples
        // http://www.sibeliusblog.com/wp-content/uploads/2015/08/lv-3.png
        // http://forum.makemusic.com/attach.aspx/19841/ties_for_rings.JPG
        // https://www.pivotaltracker.com/story/show/138698277
        
        Tie(const MusicLocation& inLocation,
            const CurveParams& inParams);
        
        /// operator=() implementation
        void copyState(const Tie& other);
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
