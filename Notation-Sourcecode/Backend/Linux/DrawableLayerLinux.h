// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Qt
#include <QtWidgets/QtWidgets>

// Local
#include "DrawableColor.h"

namespace komp
{
    class DrawableLayerLinux
    {
    public:
        DrawableLayerLinux();
        DrawableLayerLinux(const Region& inPath);
        DrawableLayerLinux(QGraphicsItem* inLayer);
        ~DrawableLayerLinux();
        
        DrawableLayerLinux(const DrawableLayerLinux& other);

        DrawableLayerLinux& operator=(const DrawableLayerLinux& other);
        
        DrawableLayerLinux(DrawableLayerLinux&&) = default;
        DrawableLayerLinux& operator=(DrawableLayerLinux&&) = default;

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

        void setRotation(double inRotation);
        
        Region getPath() const;

        Rect getBounds() const;

        QGraphicsItem* getLayer() const;
        
    private:
        static QGraphicsItem* clone(QGraphicsItem* inItem);

        QGraphicsItem* mLayer{nullptr};
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

