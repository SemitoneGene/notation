// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#if KOMP_BUILD_LINUX
#include <QtGui/QColor>
#endif

// Local
#include "Forward.h"
#include "Geometry.h"

namespace komp
{
enum class LineCap
{
    Butt,
    Round,
    Square
};

enum class LineJoin
{
    Miter,
    Round,
    Bevel
};

class DrawableColor
{
public:
    
    DrawableColor()
    {
        createColor();
    }
    
    DrawableColor(double inRed,
                  double inGreen,
                  double inBlue,
                  double inAlpha = 1.0)
    : mRed(inRed)
    , mGreen(inGreen)
    , mBlue(inBlue)
    , mAlpha(inAlpha)
    {
        createColor();
    }
    
    DrawableColor(int inRed,
                  int inGreen,
                  int inBlue,
                  int inAlpha = 255)
    : mRed(float(inRed) / 255.0)
    , mGreen(float(inGreen) / 255.0)
    , mBlue(float(inBlue) / 255.0)
    , mAlpha(float(inAlpha) / 255.0)
    {
        createColor();
    }
    
    DrawableColor(const DrawableColor& inColor)
    {
        mRed = inColor.mRed;
        mGreen = inColor.mGreen;
        mBlue = inColor.mBlue;
        mAlpha = inColor.mAlpha;
#ifdef KOMP_BUILD_APPLE
        mColor = CGColorRetain(inColor.mColor);
#elif KOMP_BUILD_LINUX
        mColor = inColor;
#endif
    }
    
    ~DrawableColor()
    {
#ifdef KOMP_BUILD_APPLE
        CGColorRelease(mColor);
#endif
    }
    
    bool operator==(const DrawableColor& inColor) const
    {
        if (this == &inColor)  {
            return true;
        }
        
        return (mRed == inColor.mRed
                && mGreen == inColor.mGreen
                && mBlue == inColor.mBlue
                && mAlpha == inColor.mAlpha);
    }
    
    DrawableColor& operator=(const DrawableColor& other)
    {
        if (this != &other) {
            mRed = other.mRed;
            mGreen = other.mGreen;
            mBlue = other.mBlue;
            mAlpha = other.mAlpha;
#ifdef KOMP_BUILD_APPLE
            CGColorRelease(mColor);
            mColor = CGColorRetain(other.mColor);
#elif KOMP_BUILD_LINUX
            mColor = other.mColor;
#endif
        }
        return *this;
    }
    
#ifdef KOMP_BUILD_APPLE
    DrawableColor& operator=(const CGColorRef inColor)
    {
        const CGFloat* components = CGColorGetComponents(inColor);
        mRed = components[0];
        mGreen = components[1];
        mBlue = components[2];
        mAlpha = 1.0;
        CGColorRelease(mColor);
        mColor = CGColorRetain(inColor);
        return *this;
    }
#endif
    
    
#ifdef KOMP_BUILD_APPLE
    operator CGColorRef() const
    {
        return mColor;
    }
#endif
    
#if KOMP_BUILD_LINUX
    operator QColor() const
    {
        return mColor;
    }
#endif
    
private:
    void createColor()
    {
#ifdef KOMP_BUILD_APPLE
        CGFloat components[4];
        components[0] = mRed;
        components[1] = mGreen;
        components[2] = mBlue;
        components[3] = mAlpha;
        
#if TARGET_OS_IPHONE
        CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
        CGColorRef theColor = CGColorCreate(space, components);
        mColor = CGColorRetain(theColor);
        CGColorRelease(theColor);
        CGColorSpaceRelease(space);
#else
        mColor = CGColorCreateGenericRGB(components[0],
                                         components[1],
                                         components[2],
                                         components[3]);
#endif
#elif KOMP_BUILD_LINUX
        mColor.setRgbF(mRed, mGreen, mBlue, mAlpha);
#endif
    }
    
    double mRed{0.0};
    double mGreen{0.0};
    double mBlue{0.0};
    double mAlpha{1.0};
#ifdef KOMP_BUILD_APPLE
    CGColorRef mColor{nullptr};
#elif KOMP_BUILD_LINUX
    QColor mColor;
#endif
    
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
