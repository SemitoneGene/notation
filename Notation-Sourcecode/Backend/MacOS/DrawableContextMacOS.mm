// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableContextMacOS.h"

// System
#include <cstdlib>

// MacOs
#include <CoreText/CoreText.h>
#include <QuartzCore/QuartzCore.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#else
#import <XUIKit/XUIBezierPathAdditions.h>
#endif

// Local
#include "DrawableColor.h"
#include "Preferences.h"
#include "Throw.h"

namespace komp
{
    static void clearLayerActions(CALayer* inLayer)
    {
        NSDictionary* actions = @{
            @"onOrderIn": [NSNull null],
            @"onOrderOut": [NSNull null],
            @"position": [NSNull null],
            @"sublayers": [NSNull null],
            @"contents": [NSNull null],
            @"bounds": [NSNull null]
        };
        inLayer.actions = actions;
    }

    DrawableContextMacOS::DrawableContextMacOS()
    {
        auto layer = [CALayer layer];
        layer.drawsAsynchronously = YES;
        clearLayerActions(layer);
        mRoot = (__bridge_retained void*)layer;
    }
    
    DrawableContextMacOS::~DrawableContextMacOS()
    {
        CFRelease(mRoot);
    }
    
// MARK: - Layers
    
    void
    DrawableContextMacOS::addLayer(DrawableLayer* inLayer) const
    {
        const auto parent = (__bridge CALayer*)mRoot;
        const auto child = (__bridge CALayer*)inLayer->getLayer();
        if (child.superlayer != parent) {
            [parent addSublayer:child];
        }
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
