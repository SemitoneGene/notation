// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableLayer.h"

namespace komp
{
    class DrawableContextMacOS
    {
    public:
        DrawableContextMacOS();
        ~DrawableContextMacOS();
        
        void addLayer(DrawableLayer* inLayer) const;

    private:
        friend class DrawableContext;
        
        void* getLayer() const { return mRoot; }
        
        void* mRoot;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

