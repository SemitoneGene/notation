// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Geometry.h"

// System
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>
#include <numbers>
#include <vector>

#if KOMP_BUILD_LINUX
#include <opencv2/opencv.hpp>
#endif

// Local
#include "Throw.h"

namespace komp
{

// MARK: - Point

Point::Point(double inX, double inY)
: x(inX)
, y(inY)
{
}

Point::Point()
{
}

#ifdef KOMP_BUILD_APPLE
Point::Point(const CGPoint& inPoint)
{
    x = inPoint.x;
    y = inPoint.y;
}
#elif KOMP_BUILD_LINUX
Point::Point(const QPointF& inPoint)
{
    x = inPoint.x();
    y = inPoint.y();
}
#endif

void
Point::setX(double inX)
{
    x = inX;
}

void
Point::setY(double inY)
{
    y = inY;
}

/// \brief Checks to see in the points are different.
/// \result true if points differ
bool
Point::moved(const Point& inPoint) const
{
    return x != inPoint.x || y != inPoint.y;
}

/// \brief Return the distance between the points
/// \result The distance
double
Point::distance(const Point& inPoint) const
{
    return std::sqrt(std::pow(x - inPoint.x, 2) + std::pow(y - inPoint.y, 2));
}

/// \brief Rotates the point by the given angle around the origin (0, 0)

void
Point::rotate(double inAngle) {
    
    const auto angleSin = std::sin(inAngle);
    const auto angleCos = std::cos(inAngle);
    
    const auto new_x = x * angleCos - y * angleSin;
    const auto new_y = x * angleSin + y * angleCos;
    
    x = new_x;
    y = new_y;
}

/// \brief adds inPoint.x to this->x and inPoint.y to this->y
/// \result this
Point&
Point::moveBy(const Point& inPoint)
{
    x += inPoint.x;
    y += inPoint.y;
    return *this;
}


Point&
Point::moveBy(double inXAmount, double inYAmount)
{
    moveBy(Point{inXAmount, inYAmount});
    return *this;
}

void
Point::scaleBy(double inAmount)
{
    x *= inAmount;
    y *= inAmount;
}

void
Point::normalize()
{
    const auto magnitude = getMagnitude();
    x /= magnitude;
    y /= magnitude;
}

double
Point::getMagnitude() const
{
    return x * x + y * y;
}

double
Point::getDotProduct(const Point& inPoint) const
{
    return x * inPoint.x + y * inPoint.y;
}

Point
Point::getMidpoint(const Point& inPoint)
{
    return Point((x + inPoint.x) / 2.0, (y + inPoint.y) / 2.0);
}

void
Point::printToStream(const char* message) const
{
    printf("%s %f %f\n", message, x, y);
}


// MARK: - Operators

bool operator==(const Point& point1, const Point& point2)
{
    return point1.x == point2.x && point1.y == point2.y;
}

bool operator!=(const Point& point1, const Point& point2)
{
    return !(point1 == point2);
}

double operator*(const Point& point1, const Point& point2)
{
    return (point1.x * point2.y - point2.x * point1.y);
}

// MARK: - Rect

#ifdef KOMP_BUILD_APPLE
Rect::Rect(const CGRect& rect)
: top(CGRectGetMinY(rect))
, left(CGRectGetMinX(rect))
, bottom(CGRectGetMaxY(rect))
, right(CGRectGetMaxX(rect))
{
    normalize();
}
#elif KOMP_BUILD_LINUX
Rect::Rect(const QRectF& rect)
: top(rect.top())
, left(rect.left())
, bottom(rect.bottom())
, right(rect.right())
{
    normalize();
}
#endif

Rect::Rect(double inTop,
           double inLeft,
           double inBottom,
           double inRight)
: top(inTop)
, left(inLeft)
, bottom(inBottom)
, right(inRight)
{
    normalize();
}

Rect::Rect()
{
}

Rect::Rect(const Point& leftTop,
           const Point& rightBottom)
{
    left = leftTop.x;
    top = leftTop.y;
    right = rightBottom.x;
    bottom = rightBottom.y;
    
    normalize();
}

//    Rect& Rect::operator=(const CGRect& rect)
//    {
//        left = CGRectGetMinX(rect);
//        top = CGRectGetMinY(rect);
//        right = CGRectGetMaxX(rect);
//        bottom = CGRectGetMaxY(rect);
//        
//        return *this;
//    }

double
Rect::getWidth() const
{
    return right - left;
}

double
Rect::getHeight() const
{
    return bottom - top;
}

Rect&
Rect::setWidth(double newWidth)
{
    assert(newWidth >= 0);
    
    right = left + newWidth;
    return *this;
}

Rect&
Rect::setHeight(double newHeight)
{
    assert(newHeight >= 0);
    
    bottom = top + newHeight;
    return *this;
}

void
Rect::setLeft(double newLeft)
{
    left = newLeft;
}

void Rect::setTop(double newTop)
{
    top = newTop;
}

void
Rect::setRight(double newRight)
{
    KOMP_ASSERT(newRight >= left);
    right = newRight;
}

void
Rect::setBottom(double newBottom)
{
    // TODO assert(newBottom >= top);
    bottom = newBottom;
}

Point
Rect::leftTop() const
{
    Point result;
    result.x = left;
    result.y = top;
    
    return result;
}

Point
Rect::rightTop() const
{
    Point result;
    result.x = right;
    result.y = top;
    
    return result;
}

Point
Rect::leftBottom() const
{
    Point result;
    result.x = left;
    result.y = bottom;
    
    return result;
}

Point
Rect::rightBottom() const
{
    Point result;
    result.x = right;
    result.y = bottom;
    
    return result;
}

Point
Rect::getCenter() const
{
    Point result;
    result.x = left + (getWidth() / 2);
    result.y = top + (getHeight() / 2);
    
    return result;
}

Size
Rect::getSize() const
{
    return Size(getWidth(), getHeight());
}


Rect&
Rect::moveTo(double inNewLeftX, double inNewTopX)
{
    const double xOffset = inNewLeftX - left;
    const double yOffset = inNewTopX - top;
    moveBy(xOffset, yOffset);
    return *this;
}

/// \brief Adds x to left and right, adds y to top and bottom.
Rect&
Rect::moveBy(double inXAmount, double inYAmount)
{
    left += inXAmount;
    top += inYAmount;
    right += inXAmount;
    bottom += inYAmount;
    return *this;
}

#ifdef KOMP_BUILD_APPLE
Rect&
Rect::moveTo(CGPoint point)
{
    return moveTo(point.x, point.y);
}

Rect& Rect::moveBy(CGPoint point)
{
    return moveBy(point.x, point.y);
}
#endif

Rect& Rect::insetBy(double x, double y)
{
    left += x;
    right -= x;
    top += y;
    bottom -= y;
    
    return *this;
}

#ifdef KOMP_BUILD_APPLE
Rect& Rect::insetBy(CGPoint point)
{
    return insetBy(point.x, point.y);
}

void
Rect::scaleBy(double inAmount)
{
    top *= inAmount;
    left *= inAmount;
    bottom *= inAmount;
    right *= inAmount;
}

bool
Rect::contains(CGPoint point) const
{
    return point.x >= left
    && point.x <= right
    && point.y >= top
    && point.y <= bottom;
}
#endif

bool
Rect::contains(const Point& point) const
{
    return point.x >= left
    && point.x <= right
    && point.y >= top
    && point.y <= bottom;
}

bool
Rect::contains(const Rect& inRect) const
{
    return inRect.left >= left
    && inRect.right <= right
    && inRect.top >= top
    && inRect.bottom <= bottom;
}

#ifdef KOMP_BUILD_APPLE
bool
Rect::contains(CGRect inRect) const
{
    return contains(Rect(inRect));
}
#endif

static bool
valueInRange(double value, double min, double max)
{
    return (value >= min) && (value <= max);
}

bool
Rect::intersects(const Rect& inRect) const
{
    const bool xOverlap = valueInRange(left, inRect.left, inRect.right)
    || valueInRange(inRect.left, left, right);
    
    const bool yOverlap = valueInRange(top, inRect.top, inRect.bottom)
    || valueInRange(inRect.top, top, bottom);
    
    return xOverlap && yOverlap;
}

bool
Rect::intersects(const komp::Point& inA,
                 const komp::Point& inB) const
{
    const auto x1 = inA.x;
    const auto x2 = inB.x;
    
    const auto y1 = inA.y;
    const auto y2 = inB.y;
    
    const auto minX = left;
    const auto minY = top;
    const auto maxX = right;
    const auto maxY = bottom;
    
    if ((x1 <= minX && x2 <= minX)
        || (y1 <= minY && y2 <= minY)
        || (x1 >= maxX && x2 >= maxX)
        || (y1 >= maxY && y2 >= maxY)) {
        // Completely outside
        return false;
    }
    
    const auto m = (y2 - y1) / (x2 - x1);
    
    auto y = m * (minX - x1) + y1;
    if (y > minY && y < maxY) {
        return true;
    }
    
    y = m * (maxX - x1) + y1;
    if (y > minY && y < maxY) {
        return true;
    }
    
    auto x = (minY - y1) / m + x1;
    if (x > minX && x < maxX) {
        return true;
    }
    
    x = (maxY - y1) / m + x1;
    if (x > minX && x < maxX) {
        return true;
    }
    
    return false;
}

#ifdef KOMP_BUILD_APPLE
bool
Rect::intersects(CGRect inRect) const
{
    return intersects(Rect(inRect));
}

bool
Rect::intersect(CGRect inRect, CGRect outRect) const
{
    CGRect thisRect = ::CGRectMake(left, top, getWidth(), getHeight());
    outRect = ::CGRectIntersection(thisRect, inRect);
    return !::CGRectIsNull(outRect);
}

bool
Rect::intersect(const Rect& inRect, Rect& outRect) const
{
    CGRect thisRect = ::CGRectMake(left, top, getWidth(), getHeight());
    CGRect cgRect = ::CGRectIntersection(thisRect, inRect);
    outRect = cgRect;
    return !::CGRectIsNull(cgRect);
}
#endif

bool
Rect::getIsEmpty() const
{
    return (getWidth() <= 0 && getHeight() <= 0);
}

void
Rect::normalize()
{
    double temp;
    
    if (left > right) {
        temp = left;
        setLeft(right);
        setRight(temp);
    }
    
    if (top > bottom) {
        temp = top;
        setTop(bottom);
        setBottom(temp);
    }
}


void
Rect::invert()
{
    double temp = right;
    right = left;
    left = temp;
    
    temp = bottom;
    bottom = top;
    top = temp;
}


void
Rect::unionRect(const Rect& inRect)
{
#ifdef KOMP_BUILD_APPLE
    const auto theRect = CGRectUnion(*this, inRect);
    *this = theRect;
#else
    cv::Rect2d r1(left,
                  top,
                  getWidth(),
                  getHeight());
    cv::Rect2d r2(inRect.left,
                  inRect.top,
                  inRect.getWidth(),
                  inRect.getHeight());
    cv::Rect2d r3 = r1 | r2;
    
    left = r3.x;
    top = r3.y;
#endif
}

void
Rect::initializeOrUnion(const Rect& inRect)
{
    if (inRect.getIsEmpty()) {
        return;
    }
    
    if (getIsEmpty()) {
        this->operator=(inRect);
    } else {
        this->unionRect(inRect);
    }
}

Rect&
Rect::grow(double inHorizontal, double inVertical)
{
    left -= inHorizontal;
    top -= inVertical;
    right += inHorizontal;
    bottom += inVertical;
    return *this;
}


void
Rect::printToStream(std::string message) const
{
    if (message.size() > 0) {
        std::cout << std::move(message);
        std::cout << " ";
    }
    std::cout << *this;
    std::cout << std::endl;
}

double
Point::degrees(const Point &inOther) const
{
    const auto normalizedY = inOther.y - y;
    const auto normalizedX = inOther.x - x;
    const auto arctan = std::atan2(normalizedY, normalizedX);
    const auto degrees = arctan * (180.0 / std::numbers::pi);
    return degrees;
}

// MARK: - Operators

#ifdef KOMP_BUILD_APPLE
Rect operator+(const Rect &rect, CGPoint point)
{
    Rect result = rect;
    result.left += point.x;
    result.right += point.x;
    result.top += point.y;
    result.bottom += point.y;
    return result;
}

Rect operator-(const Rect &rect, CGPoint point)
{
    Rect result = rect;
    result.left -= point.x;
    result.right -= point.x;
    result.top -= point.y;
    result.bottom -= point.y;
    return result;
}
#endif

bool operator==(const Rect& rect1, const Rect& rect2)
{
    return rect1.left == rect2.left
    && rect1.top == rect2.top
    && rect1.right == rect2.right
    && rect1.bottom == rect2.bottom;
}

bool operator!=(const Rect&rect1, const Rect &rect2)
{
    return !(rect1 == rect2);
}

std::ostream& operator<<(std::ostream& os, const Rect& inRect)
{
    os << std::setprecision(1) << std::fixed;
    os << "L " << inRect.left;
    os << ", R " << inRect.right;
    os << ", T " << inRect.top;
    os << ", B " << inRect.bottom;
    os << ", W " << inRect.getWidth();
    os << ", H " << inRect.getHeight();
    return os;
}

// MARK: - Extents

bool operator==(const Extents& extents1, const Extents& extents2)
{
    return extents1.left == extents2.left
    && extents1.top == extents2.top
    && extents1.right == extents2.right
    && extents1.bottom == extents2.bottom;
}

bool operator!=(const Extents& extents1, const Extents& extents2)
{
    return !(extents1 == extents2);
}


// MARK: - Rectangles

#ifdef KOMP_BUILD_APPLE
static CGPoint
CGRectTopLeftPoint(const CGRect& rect)
{
    return CGPointMake(CGRectGetMinX(rect), CGRectGetMinY(rect));
}

static CGPoint
CGRectTopRightPoint(const CGRect& rect)
{
    return CGPointMake(CGRectGetMaxX(rect), CGRectGetMinY(rect));
}

static CGPoint
CGRectBottomLeftPoint(const CGRect& rect)
{
    return CGPointMake(CGRectGetMinX(rect), CGRectGetMaxY(rect));
}

static CGPoint
CGRectBottomRightPoint(const CGRect& rect)
{
    return CGPointMake(CGRectGetMaxX(rect), CGRectGetMaxY(rect));
}
#endif

// MARK: - Region

#ifdef KOMP_BUILD_APPLE
Region::Region(CGPathRef inCGPathRef)
: mPath(inCGPathRef, CGPathRelease)
{
}
#elif KOMP_BUILD_LINUX
Region::Region(const QPainterPath& inPath)
: mPath{inPath}
{
}
#endif

Region::Region(const std::vector<Point>& inPoints)
#ifdef KOMP_BUILD_APPLE
: mPath(CGPathCreateMutable(), CGPathRelease)
{
    if (!inPoints.empty()) {
        
        CGMutablePathRef path = getMutable();
        const auto& first = inPoints.front();
        CGPathMoveToPoint(path, NULL, first.x, first.y);
        
        for (int index = 1; index < inPoints.size(); ++index) {
            
            const auto& point = inPoints.at(index);
            CGPathAddLineToPoint(path,
                                 NULL,
                                 point.x,
                                 point.y);
            
        }
        
        CGPathCloseSubpath(path);
    }
}
#elif KOMP_BUILD_LINUX
{
    KOMP_ASSERT("FIX ME!");
}
#endif

Region::Region()
{
}

Region::~Region()
{
}

bool
Region::intersects(const Region& inRegion) const
{
    return intersects(inRegion.getBounds());
}

bool
Region::intersects(const Rect& inRect) const
{
#ifdef KOMP_BUILD_APPLE
    const CGRect boundingRect = inRect;
    const std::vector<CGPoint> points = {
        CGRectTopLeftPoint(boundingRect),
        CGRectTopRightPoint(boundingRect),
        CGRectBottomRightPoint(boundingRect),
        CGRectBottomLeftPoint(boundingRect),
    };
    
    for (const auto& point : points) {
        if (::CGPathContainsPoint(const_cast<CGPathRef>(mPath.get()),
                                  NULL,
                                  point,
                                  false)) {
            return true;
        }
    }
#elif KOMP_BUILD_LINUX
    
    const QRectF boundingRect = inRect;
    const std::vector<QPointF> points = {
        boundingRect.topLeft(),
        boundingRect.topRight(),
        boundingRect.bottomRight(),
        boundingRect.bottomLeft()
    };

    for (const auto& point : points) {
        if (mPath.contains(point)) {
            return true;
        }
    }
#endif
    return false;
}

bool
Region::contains(const Region& inRegion) const
{
    const auto boundingRect = inRegion.getBounds();
    return contains(boundingRect);
}

bool
Region::contains(const Rect& inRect) const
{
    size_t count = 0;
    
#ifdef KOMP_BUILD_APPLE
    const CGRect boundingRect = inRect;
    const std::vector<CGPoint> points = {
        CGRectTopLeftPoint(boundingRect),
        CGRectTopRightPoint(boundingRect),
        CGRectBottomRightPoint(boundingRect),
        CGRectBottomLeftPoint(boundingRect),
    };
    
    for (const auto& point : points) {
        if (::CGPathContainsPoint(const_cast<CGPathRef>(mPath.get()),
                                  NULL,
                                  point,
                                  false)) {
            ++count;
        }
    }
#elif KOMP_BUILD_LINUX
    const QRectF boundingRect = inRect;
    const std::vector<QPointF> points = {
        boundingRect.topLeft(),
        boundingRect.topRight(),
        boundingRect.bottomRight(),
        boundingRect.bottomLeft()
    };

    for (const auto& point : points) {
        if (mPath.contains(point)) {
            ++count;
        }
    }
#endif
    return count == points.size();
}

bool
Region::contains(const Point& inPoint) const
{
#ifdef VERBOSE
    const auto tester = getBounds();
    const auto test = tester.contains(inPoint);
    if (test != result) {
        LOG_F(ERROR, "Rect and region containment should be the same.");
    }
#endif

#ifdef KOMP_BUILD_APPLE
    if (!isValid()) {
        return false;
    }
    return ::CGPathContainsPoint(const_cast<CGPathRef>(mPath.get()),
                                 NULL,
                                 inPoint,
                                 false);
#elif KOMP_BUILD_LINUX
    return mPath.contains({inPoint.x, inPoint.y});
#endif
}

Region&
Region::moveBy(const komp::Point& inAmount)
{
    return moveBy(inAmount.x, inAmount.y);
}


Region&
Region::moveBy(double inXAmount, double inYAmount)
{
#ifdef KOMP_BUILD_APPLE
    if (inXAmount != 0 || inYAmount != 0) {
        
        const auto translation = ::CGAffineTransformMakeTranslation(inXAmount,
                                                                    inYAmount);
        
        const auto movedPath = ::CGPathCreateCopyByTransformingPath(getImmutable(),
                                                                    &translation);
        mPath = CGPathPtr{movedPath, CGPathRelease};
    }
#elif KOMP_BUILD_LINUX
    mPath.translate(inXAmount, inYAmount);
#endif
    return *this;
}

Rect
Region::getBounds() const
{
#ifdef KOMP_BUILD_APPLE
    const auto bounds = CGPathGetPathBoundingBox(mPath.get());
    return Rect(bounds);
#elif KOMP_BUILD_LINUX
    return Rect(mPath.boundingRect());
#endif
}

void
Region::addRect(const Rect& inRect)
{
#ifdef KOMP_BUILD_APPLE
    CGPathAddRect(getMutable(), NULL, inRect);
#elif KOMP_BUILD_LINUX
    mPath.addRect(inRect);
#endif
}

void
Region::addPath(Region& inRegion)
{
#ifdef KOMP_BUILD_APPLE
    CGPathAddPath(getMutable(), NULL, inRegion);
#elif KOMP_BUILD_LINUX
    mPath.addPath(inRegion.getPath());
#endif
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

