// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableColor.h"
#include "DrawableLayer.h"
#include "Forward.h"
#include "Geometry.h"

namespace komp
{
#ifdef KOMP_BUILD_APPLE
class DrawableContextMacOS;
#elif KOMP_BUILD_LINUX
class DrawableContextLinux;
#endif
}

namespace komp
{
    class DrawableContext
    {
    public:
        DrawableContext();        
        ~DrawableContext();
                
        void* getLayer() const;
                
    private:
        friend class DrawableLayer;
        
        void addLayer(DrawableLayer* inLayer) const;
#ifdef KOMP_BUILD_APPLE
        std::unique_ptr<DrawableContextMacOS> mDrawableContextImpl;
#elif KOMP_BUILD_LINUX
        std::unique_ptr<DrawableContextLinux> mDrawableContextImpl;
#endif

    };    
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
