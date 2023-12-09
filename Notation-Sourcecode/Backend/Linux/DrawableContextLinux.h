// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableLayer.h"

namespace komp
{
    class DrawableContextLinux
    {
    public:
        DrawableContextLinux();
        ~DrawableContextLinux();
      
      void addLayer(DrawableLayer* inLayer) const;
    
    private:
        friend class DrawableContext;
        
        QGraphicsScene* getLayer() const { return mRoot.get(); }

        std::unique_ptr<QGraphicsScene> mRoot;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

