// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLayerMacOS.h"

// MacOs
#import <QuartzCore/QuartzCore.h>

namespace komp
{

template<typename T> inline T* objc_cast(id from) {
    if ([from isKindOfClass:[T class]]) {
        return static_cast<T*>(from);
    }
    return nil;
}

DrawableLayerMacOS::DrawableLayerMacOS()
{
    auto layer = [CAShapeLayer layer];
    layer.drawsAsynchronously = YES;
    NSDictionary* actions = @{
        @"onOrderIn": [NSNull null],
        @"onOrderOut": [NSNull null],
        @"position": [NSNull null],
        @"sublayers": [NSNull null],
        @"contents": [NSNull null],
        @"bounds": [NSNull null]
    };
    layer.actions = actions;
    mLayer = (__bridge_retained void*)layer;
}

DrawableLayerMacOS::DrawableLayerMacOS(const Region& inPath)
{
    auto layer = [CAShapeLayer layer];
    layer.drawsAsynchronously = YES;
    NSDictionary* actions = @{
        @"onOrderIn": [NSNull null],
        @"onOrderOut": [NSNull null],
        @"position": [NSNull null],
        @"sublayers": [NSNull null],
        @"contents": [NSNull null],
        @"bounds": [NSNull null]
    };
    layer.actions = actions;
    layer.path = inPath.getMutable();
    mLayer = (__bridge_retained void*)layer;
}

DrawableLayerMacOS::DrawableLayerMacOS(void* inLayer)
{
    CALayer* layer = (__bridge CALayer*) inLayer;
    mLayer = (__bridge_retained void*)layer;
}

DrawableLayerMacOS::~DrawableLayerMacOS()
{
    CFRelease(mLayer);
}

DrawableLayerMacOS::DrawableLayerMacOS(const DrawableLayerMacOS& other)
{
    CALayer* layer = (__bridge CALayer*) other.mLayer;
    mLayer = (__bridge_retained void*)layer;
}

DrawableLayerMacOS&
DrawableLayerMacOS::operator=(const DrawableLayerMacOS& other)
{
    if (other.mLayer) {
        CALayer* layer = (__bridge CALayer*) other.mLayer;
        mLayer = (__bridge_retained void*)layer;
    }
    return *this;
}

void
DrawableLayerMacOS::removeFromParent()
{
    CALayer* layer = (__bridge CALayer*)mLayer;
    [layer removeFromSuperlayer];
}

void
DrawableLayerMacOS::setLineWidth(double inWidth)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.lineWidth = inWidth;
}

void
DrawableLayerMacOS::setMiterLimit(double inLimit)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.miterLimit = inLimit;
}

void
DrawableLayerMacOS::setFillColor(const DrawableColor& inColor)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.fillColor = inColor;
}

void
DrawableLayerMacOS::disableFillColor()
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.fillColor = nil;
}

void
DrawableLayerMacOS::setStrokeColor(const DrawableColor& inColor)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.strokeColor = inColor;
}

void
DrawableLayerMacOS::disableStrokeColor()
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*)mLayer;
    layer.strokeColor = nil;
}

void
DrawableLayerMacOS::moveTo(const Point& inPosition)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.position = inPosition;
}

void
DrawableLayerMacOS::moveBy(const Point& inAmount)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    auto position = layer.position;
    position.x += inAmount.x;
    position.y += inAmount.y;
    layer.position = position;
}

void
DrawableLayerMacOS::setZOrder(int inOrder)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.zPosition = inOrder;
}

void
DrawableLayerMacOS::setScale(double inScale)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.transform = CATransform3DMakeScale(inScale, inScale, 1.0);
}

void
DrawableLayerMacOS::setRotation(double inDegrees)
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    layer.transform = CATransform3DMakeRotation(inDegrees / 180.0 * M_PI,
                                                0.0,
                                                0.0,
                                                1.0);
}

Region
DrawableLayerMacOS::getPath() const
{
    CAShapeLayer* layer = (__bridge CAShapeLayer*) mLayer;
    return Region(layer.path);
}

Rect
DrawableLayerMacOS::getBounds() const
{
    CALayer* layer = (__bridge CALayer*) mLayer;
    return Rect(layer.bounds);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
