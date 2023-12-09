// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableColor.h"

namespace komp
{
    class DrawableLayerMacOS
    {
    public:
        DrawableLayerMacOS();
        DrawableLayerMacOS(const Region& inPath);
        DrawableLayerMacOS(void* inLayer);
        ~DrawableLayerMacOS();
        
        DrawableLayerMacOS(const DrawableLayerMacOS& other);

        DrawableLayerMacOS& operator=(const DrawableLayerMacOS& other);
        
        DrawableLayerMacOS(DrawableLayerMacOS&&) = default;
        DrawableLayerMacOS& operator=(DrawableLayerMacOS&&) = default;

        void removeFromParent();
        
        void setLineWidth(double inWidth);
        
        void setMiterLimit(double inLimit);
        
        void setFillColor(const DrawableColor& inColor);
        void disableFillColor();

        void setStrokeColor(const DrawableColor& inColor);
        void disableStrokeColor();
        
        void moveTo(const Point& inPosition);
        void moveBy(const Point& inAmount);

        void setZOrder(int inOrder);
        
        void setScale(double inScale);
        
        void setRotation(double inDegrees);

        Region getPath() const;

        Rect getBounds() const;

        void* getLayer() const { return mLayer; }

        bool getParented() const { return mParented; }
        void setParented(bool inValue) { mParented = inValue; }

    private:
        void* mLayer{nullptr};
        bool mParented{false};
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

