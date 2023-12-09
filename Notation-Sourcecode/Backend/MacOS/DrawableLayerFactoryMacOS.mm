// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLayerFactoryMacOS.h"

// MacOs
#include <CoreText/CoreText.h>
#include <QuartzCore/QuartzCore.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif

// Local
#include "DrawableLayer.h"
#include "EngraverMacOS.h"
#include "FontManager.h"

#if TARGET_OS_OSX
#import "NotationNSBezierPath+Cocoa.h"
#endif


namespace komp
{
static void clearLayerActions(CALayer* inLayer)
{
    NSDictionary* actions = @{
        kCAOnOrderIn: [NSNull null],
        kCAOnOrderOut: [NSNull null],
        @"frame": [NSNull null],
        @"position": [NSNull null],
        @"sublayers": [NSNull null],
        @"contents": [NSNull null],
        @"bounds": [NSNull null]
    };
    inLayer.actions = actions;
}

DrawableLayerFactoryMacOS::DrawableLayerFactoryMacOS()
{
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryMacOS::createCurve(const DrawableContext& inContext,
                                       const Region& inRegion) const
{
    CAShapeLayer* layer = [CAShapeLayer layer];
    layer.drawsAsynchronously = YES;
    layer.path = inRegion.getMutable();
    layer.lineCap = kCALineCapRound;
    layer.lineJoin = kCALineJoinRound;
    layer.miterLimit = 0.0;
    clearLayerActions(layer);
    return std::make_unique<DrawableLayer>(inContext,
                                           (__bridge void *)layer);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryMacOS::createEllipse(const DrawableContext& inContext,
                                         double inWidth,
                                         double inHeight) const
{
    const auto ellipse = CGRectMake(0, 0, inWidth, inHeight);
    KompBezierPath* path = [KompBezierPath bezierPathWithOvalInRect:ellipse];
    
    CAShapeLayer* layer = [CAShapeLayer layer];
    layer.drawsAsynchronously = YES;
    layer.miterLimit = 0.0;
    layer.path = path.CGPath;
    clearLayerActions(layer);
    return std::make_unique<DrawableLayer>(inContext,
                                           (__bridge void *)layer);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryMacOS::createLine(const DrawableContext& inContext,
                                      const Point& inStart,
                                      const Point& inEnd) const
{
    const komp::Region region = CGPathCreateMutable();
    const auto path = region.getMutable();
    ::CGPathMoveToPoint(path, NULL, inStart.x, inStart.y);
    ::CGPathAddLineToPoint(path, NULL, inEnd.x, inEnd.y);
    ::CGPathCloseSubpath(path);
    
    CAShapeLayer* layer = [CAShapeLayer layer];
    layer.drawsAsynchronously = YES;
    layer.lineCap = kCALineCapButt;
    layer.lineJoin = kCALineJoinMiter;
    layer.miterLimit = 10.0;
    layer.path = path;
    clearLayerActions(layer);
    return std::make_unique<DrawableLayer>(inContext,
                                           (__bridge void *)layer);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryMacOS::createRectangle(const DrawableContext& inContext,
                                           const Rect& inBounds) const {
    const komp::Region region = CGPathCreateMutable();
    const auto path = region.getMutable();
    ::CGPathAddRect(path, NULL, inBounds);
    
    CAShapeLayer* layer = [CAShapeLayer layer];
    layer.drawsAsynchronously = YES;
    layer.path = path;
    clearLayerActions(layer);
    return std::make_unique<DrawableLayer>(inContext,
                                           (__bridge void *)layer);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryMacOS::createText(const DrawableContext& inContext,
                                      std::string_view inFontName,
                                      double inFontSize,
                                      const char* inText,
                                      const Point& inPosition,
                                      const DrawableColor& inColor) const
{
    CATextLayer* layer = [CATextLayer layer];
    const auto scale = 4.0;
#if TARGET_OS_IPHONE
    layer.contentsScale = UIScreen.mainScreen.scale * scale;
#else
    layer.contentsScale = [[NSScreen mainScreen] backingScaleFactor] * scale;
#endif
    NSString* const drawString = [[NSString alloc] initWithCString:inText
                                                          encoding:NSUTF8StringEncoding];
    
    NSString* const fontName = [[NSString alloc] initWithCString:std::string(inFontName).c_str()
                                                        encoding:NSMacOSRomanStringEncoding];
    KompFont* font = [KompFont fontWithName:fontName size:inFontSize];
    
    CFMutableDictionaryRef  attrs = CFDictionaryCreateMutable(NULL,
                                                              0,
                                                              &kCFCopyStringDictionaryKeyCallBacks,
                                                              &kCFTypeDictionaryValueCallBacks);
    CFDictionaryAddValue(attrs,
                         kCTFontAttributeName,
                         (__bridge CTFontRef)font);
    CFDictionaryAddValue(attrs,
                         kCTForegroundColorAttributeName,
                         inColor);
    
    CFAttributedStringRef attributedString = CFAttributedStringCreate(NULL,
                                                                      (__bridge CFStringRef)drawString,
                                                                      attrs);
    
    layer.string = (__bridge NSMutableAttributedString*)attributedString;
    
    CFRelease(attrs);
    CFRelease(attributedString);
    
    // NOTE: We can't use the Engraver to measure here. The tight metrics
    // messes up the creation of the layer. Why? I don't know.
    KompFont* theFont = [KompFont fontWithName:fontName size:inFontSize];
    auto size = [drawString sizeWithAttributes:@{NSFontAttributeName: theFont}];
    size = komp::Size(ceil(size.width), ceil(size.height));
    
    layer.frame = CGRectMake(inPosition.x,
                             inPosition.y,
                             size.width,
                             size.height);
    
    return std::make_unique<DrawableLayer>(inContext,
                                           (__bridge void *)layer);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
