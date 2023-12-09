// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <assert.h>
#include <stdio.h>
#include <iosfwd>
#include <string>
#include <vector>

#ifdef KOMP_BUILD_APPLE
#include <CoreGraphics/CoreGraphics.h>
#include <CoreGraphics/CGPath.h>
#elif KOMP_BUILD_LINUX
#include <QtCore/QRectF>
#include <QtGui/QPainterPath>
#endif

namespace komp
{
    
// MARK: - Point
    
    struct Point
    {
        double x{0.0};
        double y{0.0};
        
        Point(double inX, double inY);
        Point();
        
#ifdef KOMP_BUILD_APPLE
        Point(const CGPoint& inPoint);
        
        operator CGPoint() const
        {
            return ::CGPointMake(x, y);
        }
#elif KOMP_BUILD_LINUX
        Point(const QPointF& inPoint);
        
        operator QPointF() const
        {
            return QPointF{x, y};
        }

#endif
        
        void setX(double);
        void setY(double);
        
        bool moved(const Point& inPoint) const;
        double distance(const Point& inPoint) const;
        void rotate(double inAngle);

        /// adds inPoint.x to this->x and inPoint.y to this->y
        Point& moveBy(const Point& inPoint);
        Point& moveBy(double inXAmount, double inYAmount);
        
        void scaleBy(double inAmount);
        
        void normalize();
        double getMagnitude() const;
        double getDotProduct(const Point& inPoint) const;        
        Point getMidpoint(const Point& inPoint);

        /// prints to std::cout
        void printToStream(const char * = "") const;
        
        inline bool getIsPointZero() const { return x == 0.0 && y == 0.0; }

        // TODO: probably not working correctly, not sure if we have a way to run unit tests these days.
        /// get the angle of the line formed with `inOther`
        ///   0 degrees - the line is horizontal from left to right        e.g. this{0, 0}, inOther{1, 0}
        ///  45 degrees - the line is sloping downward left to right       e.g. this{0, 0}, inOther{1, 1}
        ///  90 degrees - the line is vertical from top to bottom          e.g. this{0, 0}, inOther{0, 1}
        /// 135 degrees - the line is sloping downward from right to left  e.g. this{0, 0}, inOther{-1, 1}
        /// 180 degrees - the line is horizontal from right to left        e.g. this{0, 0}, inOther{-1, 0}
        /// 225 degrees - the line is sloping upwards from right to left   e.g. this{0, 0}, inOther{-1, -1}
        /// 270 degrees - the line is vertical from bottom to top          e.g. this{0, 0}, inOther{0, -1}
        /// 315 degrees - the line is sloping upwards from left to right   e.g. this{0, 0}, inOther{1, -1}
        /// Any value outside the range of 0 - 360 will be stored as its 0 - 360 equivalent
        double degrees(const Point& inOther) const;
        
        Point operator+(const Point& other) const
        {
            return {x + other.x, y + other.y};
        }

        Point operator-(const Point& other) const
        {
            return {x - other.x, y - other.y};
        }

        Point operator+=(double value) const
        {
            return {x + value, y + value};
        }

        Point operator+=(const Point& other) const
        {
            return {x + other.x, y + other.y};

        }

        Point operator*(double scalar) const
        {
            return {x * scalar, y * scalar};
        }
        
        double dot(const Point& other) const
        {
            return x * other.x + y * other.y;
        }
    };
    
    bool operator==(const Point& point1, const Point& point2);
    bool operator!=(const Point& point1, const Point& point2);
    double operator*(const Point& point1, const Point& point2);
    
    static const Point POINT_ZERO = {0, 0};
    
    using Points = std::vector<Point>;
    using Paths = std::vector<Points>;
    
// MARK: - Size
    
    struct Size
    {
        double width;
        double height;
        
        Size()
        : width(0)
        , height(0) {}
        
        Size(double inWidth, double inHeight)
        : width(inWidth)
        , height(inHeight) {}
        
#ifdef KOMP_BUILD_APPLE
        operator CGSize() const
        {
            return ::CGSizeMake(width, height);
        }
#endif
    };
    
// MARK: - Rect
    
    struct Rect
    {
        double top{0.0};
        double left{0.0};
        double bottom{0.0};
        double right{0.0};
        
        Rect();
#ifdef KOMP_BUILD_APPLE
        Rect(const CGRect& rect);
#elif KOMP_BUILD_LINUX
        Rect(const QRectF& rect);
#endif
        Rect(double top, double left, double bottom, double right);
        Rect(const Point& leftTop, const Point& rightBottom);
        
#ifdef KOMP_BUILD_APPLE
        operator CGRect() const
        {
            return ::CGRectMake(left, top, getWidth(), getHeight());
        }
#elif KOMP_BUILD_LINUX
        operator QRectF() const
        {
            return QRectF(left, top, getWidth(), getHeight());
        }
#endif
        double getWidth() const;
        double getHeight() const;
        inline double getHalfWidth() const { return getWidth() / 2.0; }
        inline double getHalfHeight() const { return getHeight() / 2.0; }
        
        Rect& setWidth(double);
        Rect& setHeight(double);
        
        Point leftTop() const;
        Point rightTop() const;
        Point leftBottom() const;
        Point rightBottom() const;
        
        void setLeft(double);
        void setTop(double);
        void setRight(double);
        void setBottom(double);
        
        Point getCenter() const;
        
        Size getSize() const;
        
        /// Moves the rect such that left will equal
        /// inNewLeftX and top will equal inNewTopX
        Rect& moveTo(double inNewLeftX, double inNewTopX);
        
        /// Adds x to left and right, adds y to top and bottom.
        Rect& moveBy(double inXAmount, double inYAmount);
        
#ifdef KOMP_BUILD_APPLE
        Rect& moveTo(CGPoint);
        Rect& moveBy(CGPoint);
#endif
        
        Rect& insetBy(double x, double y);
#ifdef KOMP_BUILD_APPLE
        Rect& insetBy(CGPoint);
#endif
        
        void scaleBy(double inAmount);
        
#ifdef KOMP_BUILD_APPLE
        bool contains(CGPoint) const;
#endif
        bool contains(const Point& Point) const;
        bool contains(const Rect& inRect) const;
#ifdef KOMP_BUILD_APPLE
        bool contains(CGRect inRect) const;
        bool intersects(CGRect) const;
#endif
        bool intersects(const Rect& inRect) const;
        bool intersects(const komp::Point& inA, const komp::Point& inB) const;
#ifdef KOMP_BUILD_APPLE
        bool intersect(CGRect, CGRect) const;
#endif
        bool intersect(const Rect&, Rect&) const;
        
        bool getIsEmpty() const;
        
        void normalize();
        void invert();
        
        void unionRect(const Rect& inRect);
        
        /// if this Rect is empty, then the value of inRect
        /// will be taken.  if this Rect is not empty, then
        /// the value of inRect will be unioned with this one.
        /// also ignores the inRect if it is empty
        void initializeOrUnion(const Rect& inRect);
        
        Rect& grow(double inHorizontal, double inVertical);
        
        void printToStream(std::string message = "") const;
    };
    
#ifdef KOMP_BUILD_APPLE
    Rect operator+(const Rect &rect, CGPoint point);
    Rect operator-(const Rect &rect, CGPoint point);
#endif
    bool operator==(const Rect &rect1, const Rect &rect2);
    bool operator!=(const Rect& rect1, const Rect &rect2);
    std::ostream& operator<<(std::ostream& os, const Rect& inRect);
    
// MARK: - Region
    
    class Region
    {
    public:
#ifdef KOMP_BUILD_APPLE
        Region(CGPathRef inCGPathRef);
#else
        Region(const QPainterPath& inPath);
#endif
        Region(const std::vector<Point>& inPoints);
        Region();
        Region(const Region&) = default;
        Region(Region&&) noexcept = default;
        Region& operator=(const Region&) = default;
        Region& operator=(Region&&) noexcept = default;
        ~Region();
        
        bool contains(const Region& inRegion) const;
        bool contains(const Rect& inRect) const;
        bool contains(const Point& inPoint) const;
        
        bool intersects(const Region& inRegion) const;
        bool intersects(const Rect& inRect) const;
        
        Region& moveBy(double inXAmount, double inYAmount);
        Region& moveBy(const komp::Point& inAmount);
        
        void addRect(const Rect& inRect);
        
        void addPath(Region& inRegion);
        
        Rect getBounds() const;
        
#ifdef KOMP_BUILD_APPLE
        inline operator CGPathRef() const { return const_cast<CGPathRef>(mPath.get()); }
        inline CGPathRef getImmutable() const { return const_cast<CGPathRef>(mPath.get());}
        inline CGMutablePathRef getMutable() const { return const_cast<CGMutablePathRef>(getImmutable()); }
        bool isValid() const { return mPath != nullptr; }
#endif
        
#if KOMP_BUILD_LINUX
        inline operator QPainterPath*() const { return const_cast<QPainterPath*>(&mPath); }
        QPainterPath& getPath() { return mPath; }
        bool isValid() const { return true; }
#endif
        
        
    private:
#ifdef KOMP_BUILD_APPLE
        using CGPathPtr = std::shared_ptr<std::remove_pointer<CGPathRef>::type>;
        CGPathPtr mPath{nullptr};
#else
        QPainterPath mPath;
#endif
    };
    
// MARK: - Extents
    
    struct Extents
    {
        double top;
        double left;
        double bottom;
        double right;
        
        Extents()
        : top(0)
        , left(0)
        , bottom(0)
        , right(0)
        {
        }
        
        Extents(double inTop,
                double inLeft,
                double inBottom,
                double inRight)
        : top(inTop)
        , left(inLeft)
        , bottom(inBottom)
        , right(inRight)
        {
        }
    };
    
    bool operator==(const Extents& extents1, const Extents& extents2);
    bool operator!=(const Extents& extents1, const Extents& extents2);
    
    static const Extents EXTENTS_ZERO = {0, 0, 0, 0};
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
