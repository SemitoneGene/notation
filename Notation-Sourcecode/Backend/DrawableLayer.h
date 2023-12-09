// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableColor.h"

#ifdef KOMP_BUILD_APPLE
#include "DrawableLayerMacOS.h"
#elif KOMP_BUILD_LINUX
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include "DrawableLayerLinux.h"
#endif

namespace komp
{
#ifdef KOMP_BUILD_APPLE
using DrawableContextItem = void;
#elif KOMP_BUILD_LINUX
using DrawableContextItem = QGraphicsItem;
#endif
}

namespace komp
{
class DrawableLayer
{
public:
    DrawableLayer() = delete;
    DrawableLayer(const DrawableContext& inParent);
    DrawableLayer(const DrawableContext& inParent, const Region& inPath);
    DrawableLayer(const DrawableContext& inParent, DrawableContextItem* inItem);
    ~DrawableLayer();
    
    DrawableLayer(const DrawableLayer& other) = delete;
    DrawableLayer& operator=(const DrawableLayer& other) = delete;
    DrawableLayer(DrawableLayer&&) = delete;
    DrawableLayer& operator=(DrawableLayer&&) = delete;
    
    void removeFromParent();
    void restoreParent();
    
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
    
    DrawableContextItem* getLayer() const;
    
    bool isValid() const;
    
private:
    const DrawableContext& mParent;
    
#ifdef KOMP_BUILD_APPLE
    std::unique_ptr<DrawableLayerMacOS> mDrawableLayerImpl;
#elif KOMP_BUILD_LINUX
    std::unique_ptr<DrawableLayerLinux> mDrawableLayerImpl;
#endif
};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
