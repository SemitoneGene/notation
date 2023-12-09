// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Curve.h"

// Local
#include "DrawableCurve.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "Geometry.h"
#include "Item.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Note.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "StaffSystem.h"
#include "Staff.h"
#include "Temporal.h"

namespace
{
static constexpr const auto CurveMemberPad = 5.0;
static constexpr const double Thickness = 4.0;
static constexpr const double StemOffset = 10.0;
static constexpr const double VerticalOffset = 3.0;
static constexpr const auto a30 = M_PI / 8.0;
static constexpr const auto a45 = M_PI / 4.0;
static constexpr const auto a90 = M_PI / 2.0;
}

namespace komp
{

static double
tenthsToStaff(double inValue)
{
    return (inValue * Staff::getSpacing()) / 10.0;
}

// MARK: - Curve

Curve::Curve(const MusicLocation& inLocation,
             const CurveParams& inParams)
: Spatial(inLocation)
, mBeginTemporal(inParams.beginTemporal)
, mEndTemporal(inParams.endTemporal)
, mPlacement{inParams.placementType}
{
}

Curve::Curve(const Curve& copy)
: Spatial(copy)
, mEndTime(copy.mEndTime)
, mBeginTemporal(copy.mBeginTemporal)
, mEndTemporal(copy.mEndTemporal)
, mPlacement{copy.mPlacement}
{
    copyState(copy);
}

Curve&
Curve::operator=(const Curve& other)
{
    copyState(other);
    return *this;
}

ItemUPtr
Curve::getMemento() const
{
    return std::make_unique<Curve>(*this);
}

void
Curve::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Curve* downcastedP = dynamic_cast<const Curve*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
    update();
}

void
Curve::observedItemChanged(const Item& observedItem,
                           const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        return;
    }
    
    auto doUpdate = false;
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::X);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Slot);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::TemporalType);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::StemDirection);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Updated);
    
    if (doUpdate) {
        update();
        // TODO: We need items that are encompassed by the curve to
        // somehow invalidate the curve.
        ItemImpl::setNeedsDisplay(*this);
    }
}

/// \brief Find the enclosing rectangle for the curve.
/// \result The rectangle of the curve

Rect
Curve::getFrame() const {
    return getBoundingBox();
}

/// \brief Find the enclosing region for the curve.
/// \result The region of the curve
Region
Curve::getRegion() const
{
    return ItemImpl::getRegion<DrawableCurve>(*this);
}

void
Curve::createDrawable()
{
    ItemImpl::createDrawable<DrawableCurve>(*this);
}

void
Curve::inserted()
{
    Spatial::inserted();
    configure();
    mBeginTemporal.registerObserver(*this);
    mEndTemporal.registerObserver(*this);
    const auto members = getMembers();
    for (Temporal& member : members) {
        member.registerObserver(*this);
    }
    
    notifyObservers({
        ItemChangeType::Inserted,
        ItemChangeType::Updated
    });
}

void
Curve::removed()
{
    reset();
    Spatial::removed();
    notifyObservers({ItemChangeType::Removed});
}

// MARK: - Drawing

/// \brief Return the temporals that the curve spans, excluding start and end temporals.
komp::TemporalRefVec
Curve::getMembers() const
{
    komp::TemporalRefVec result;
    
    const auto& startMeasure = mBeginTemporal.getLocation().getMeasure();
    const auto& endMeasure = mEndTemporal.getLocation().getMeasure();
    
    const auto& score = startMeasure.getScore();
    
    // Get all temporals between the start and end items
    const auto startIndex = startMeasure.getIndex();
    const auto endIndex = endMeasure.getIndex();
    
    const auto& location = mBeginTemporal.getLocation();
    const auto& track = location.getTrack();
    const auto hand = location.getHand();
    const auto voice = location.getVoice();
    
    const double afterTime = mBeginTemporal.getTime();
    const double beforeTime = mEndTemporal.getTime();
    
    for (auto index = startIndex; index <= endIndex; ++index) {
        const auto& measure = score.getMeasure(index);
        const auto temporals = measure->getTemporals(track, hand, voice);
        for (const auto& temporal : temporals) {
            const double time = temporal.get().getTime();
            if (time > afterTime && time < beforeTime) {
                result.emplace_back(temporal);
            }
        }
    }
    
    return result;
}

// MARK: - Engraving

/// \brief Configure the curve using the items it is attached to.
void
Curve::configure()
{
    setTime(mBeginTemporal.getTime());
    
    mBeginTemporal.getAttachments().addOutgoingCurve(*this);
    mEndTemporal.getAttachments().addIncomingCurve(*this);

    update();
}

void
Curve::update()
{
    if (getIsQuiescent()) {
        return;
    }
    computePoints();
    notifyObservers({ItemChangeType::Updated});
}

void
Curve::getPoints(Point& outStart,
                 Point& outControlOne,
                 Point& outControlTwo,
                 Point& outEnd) const
{
    outStart = {_x0, _y0};
    outControlOne = {_x1, _y1};
    outControlTwo = {_x2, _y2};
    outEnd = {_x3, _y3};
}

void
Curve::computePoints()
{
    computePoint(true, _x0, _y0);
    computePoint(false, _x3, _y3);
    //applyRulesForStartEndPoints();
    findContourReferencePoints();
    computeBaseline();
    computeControlPoints();
}

void
Curve::computePoint(bool inBegin,
                    double& ioX,
                    double& ioY)
{
    // If the note has a stem and the stem is at the same position as
    // the curve, place the curve over the stem.
    // Otherwise, place the curve over the notehead.
    auto startOverStem = false;
    
    auto& temporal = inBegin ? mBeginTemporal : mEndTemporal;
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    const auto isChord = temporal.getIsChordMember();
    
    const auto stemPolicy = temporal.getStemPolicy();
    if (stemPolicy.getHasStem()) {
        const auto stemUp = stemPolicy.getIsStemUp();
        startOverStem = (isBelow && !stemUp) || (!isBelow && stemUp);
    }
    
    // For chords, the received start note is the base note, but for
    // computations it is necessary to use the top or the bottom note,
    // not the base note.
    komp::Rect temporalFrame;
    if (temporal.getItemTraits().getItemType() == ItemType::Note) {
        const auto& note = getRelevantNoteForChord(temporal.as<Note>());
        temporalFrame = note.getFrames().head;
    } else {
        temporalFrame = temporal.getFrame();
    }
    
    const auto frameHalfWidth = temporalFrame.getWidth() / 2.0;
    const auto entireFrame = temporal.getFrame();

    if (isChord) {
        
        // Offset horizontally to the inside of the noteheads
        static constexpr const auto offsetX = 1.0;
        static constexpr const auto offsetY = 2.0;
        
        if (inBegin) {
            ioX = temporalFrame.right + offsetX;
        } else {
            ioX = temporalFrame.left - offsetX;
        }
        
        ioY = isBelow ? temporalFrame.bottom - offsetY
                      : entireFrame.top + offsetY;
        
    } else {
        
        const auto offsetV = tenthsToStaff(VerticalOffset);

        if (startOverStem) {
            
            // Center horizontally on the stem by default
            ioX = isBelow ? temporalFrame.left : temporalFrame.right;
            
            if (stemPolicy.getIsStemUp()) {
                const auto amount = tenthsToStaff(StemOffset);
                const auto offset = inBegin ? amount : -amount;
                ioX += tenthsToStaff(offset);
            }
            
            if (isBelow) {
                ioY = entireFrame.bottom + offsetV;
            } else {
                if (stemPolicy.getIsStemUp()) {
                    ioY = entireFrame.top + offsetV;
                } else {
                    ioY = temporalFrame.top - offsetV;
                }
            }

        } else {
            // Center on notehead
            ioX = temporalFrame.left + frameHalfWidth;
            
            ioY = isBelow ? temporalFrame.bottom + offsetV
                          : entireFrame.top - offsetV;
        }
    }
}

//void
//Curve::applyRulesForStartEndPoints()
//{
//    // When outer notes have opposite stems directions, the base line is
//    // horizontally placed on the notehead and the curve attachment point
//    // is moved to the left or right of the stem.
//    //
//    // When the curve tilts contrary to the outer note pitches, move the
//    // curve at the stem end towards the noteheads, unless the note is beamed.
//    
//    const auto firstFrame = mBeginTemporal.getFrames().head;
//    const auto lastFrame = mEndTemporal.getFrames().head;
//    
//    if (firstFrame == lastFrame) {
//        return;
//    }
//    
//    // Determine angle of baseline: y = m.x + b
//    const auto m = (_y3 - _y0) / (_x3 - _x0);
//    
//    // Determine angle of line between start/end noteheads
//    const auto mn = (lastFrame.top - firstFrame.top)
//    / (lastFrame.left - firstFrame.left);
//    
//    // If angles have a different sign, the curve goes in bad direction.
//    // The only possibility for this scenario is that one of the start/end
//    // points is attached to a stem and the other one to a note. In these
//    // cases, prefer a horizontal curve, unless the stem attached point
//    // is a beamed note.
//    if (!isEqualDouble(m, 0.0) && ((m > 0.0 && mn < 0.0)
//                                   || (m < 0.0 && mn > 0.0))) {
//        
//        if (_startOverStem) {
//            
//            // End point is below (NOTE: y axis is reversed)
//            // Move down start point (stem) and shift it to right of stem
//            _y0 = _y3;
//            _x0 = firstFrame.right + tenthsToStaff(5.0);
//            
//        } else if (_endOverStem) {
//            
//            // Start point is below (NOTE: y axis is reversed)
//            // Move down end point (stem) and shift it to left of stem
//            _y3 = _y0;
//            _x3 = lastFrame.left - tenthsToStaff(5.0);
//        }
//    }
//}

void
Curve::findContourReferencePoints()
{
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    
    std::vector<Point> data;
    
    const auto& beginFrame = mBeginTemporal.getFrame();
    if (isBelow) {
        data.push_back({beginFrame.left, beginFrame.bottom});
    } else {
        data.push_back({beginFrame.left, beginFrame.top});
    }
    
    const auto members = getMembers();
    
    for (const Temporal& member : members) {
        const auto& frame = member.getFrame();
        if (isBelow) {
            data.push_back({frame.left, frame.bottom});
        } else {
            data.push_back({frame.left, frame.top});
        }
    }
    
    const auto& endFrame = mEndTemporal.getFrame();
    if (isBelow) {
        data.push_back({endFrame.left, endFrame.bottom});
    } else {
        data.push_back({endFrame.left, endFrame.top});
    }
    
    // Fix x coordinates for start and end points
    if (data.size() > 1) {
        
        if (data[0].x < _x0) {
            data[0].x = _x0;
        } else if (data[0].x > _x0) {
            data.insert(data.begin(), Point(_x0, _y0));
        }
        
        if (data.back().x + tenthsToStaff(10.0) < _x3) {
            data.push_back(Point(_x3, _y3));
        } else {
            data.back() = Point(_x3, _y3);
        }
    }
    
    // Remove intermediate no-peak points
    auto iMax = data.size() + 1;
    while (data.size() > 2 && data.size() < iMax) {
        
        size_t iPrev = 0;
        size_t iCur = 1;
        iMax = data.size();
        
        for (size_t i = 2; i < data.size(); ++i) {
            
            if (isBelow) {
                
                if (data[iCur].y < data[iPrev].y && data[iCur].y < data[i].y) {
                    data.erase(data.begin() + iCur);
                    i = iCur;
                } else {
                    iPrev = iCur;
                    iCur = i;
                }
                
            } else {
                
                if (data[iCur].y > data[iPrev].y && data[iCur].y > data[i].y) {
                    data.erase(data.begin() + iCur);
                    i = iCur;
                } else {
                    iPrev = iCur;
                    iCur = i;
                }
            }
        }
    }
    
    // Compute baseline
    if (data.size() > 2) {
        
        // Determine base line: y = m.x + b
        const auto m = (_y3 - _y0) / (_x3 - _x0);
        const auto b = _y0 - m*_x0;
        
        // Remove points not between curve and base line
        size_t i = 1;
        while (i < data.size() - 1) {
            
            // Compute base line point for current x position
            const auto x = data[i].x;
            const auto y = m * x + b;
            
            // Remove point if not between curve and base line
            if ((isBelow && (data[i].y <= y))
                || (!isBelow && (data[i].y >= y))) {
                data.erase(data.begin() + i);
            } else {
                ++i;
            }
        }
    }
    
    // Add space between reference points and curve points
    for (auto& point : data) {
        point.y += (isBelow ? CurveMemberPad : -CurveMemberPad);
    }
    
    _dataPoints = data;
}

void
Curve::computeBaseline()
{
    const auto distance = _x3 - _x0;
    const auto slope = _y3 - _y0;
    
    _pixelDistance = sqrt(distance * distance + slope * slope);
    _distance = _pixelDistance / tenthsToStaff(1.0f);
    
    _cosb = distance / _pixelDistance;
    _sinb = slope / _pixelDistance;
}

void
Curve::computeControlPoints()
{
    if (_dataPoints.size() <= 2) {
        methodForTwoPoints();
    } else {
        computeMultiplePointsCurve();
    }
}

void
Curve::methodForTwoPoints()
{
    // Curvature increases with height and center is moved toward peak point
    // when greater height vertical/horizontal
    // NOTE: All formulas marked as "y axis reversed" have the sign changed to
    //       take into account the fact that y axis is reversed.
    
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    
    // Determine base line angle. NOTE: y axis is reversed
    const auto baseLineAngle = atan((_y0 -_y3) / (_x3 -_x0));
    
    // For short curves or curves with small angles use the default slur.
    // It has low height, is symmetrical, and requires less computations
    
    if (abs(baseLineAngle) <= a30 || _distance < 50.0) {
        computeDefaultCurve();
    } else {
        
        // Decide on angles for control lines (referred to horizontal line)
        // empirical formulas that provide good results
        const auto baseLess45 = a45 - baseLineAngle;
        const auto basePlus45 = a45 + baseLineAngle;
        
        const auto baseLess90 = a90 - baseLineAngle;
        const auto basePlus90 = a90 + baseLineAngle;
        
        auto a1 = 0.0;
        auto a2 = 0.0;
        
        if (isBelow) {
            if (baseLineAngle > 0.0) {
                a2 = std::min(a90, basePlus45);
                a1 = (baseLineAngle >= a45 ? baseLess90 : a45);
            } else {
                a1 = std::min(a90, baseLess45);
                a2 = (baseLineAngle <= -a45 ? basePlus90 : a45);
            }
        } else {
            if (baseLineAngle > 0.0) {
                a1 = std::min(a90, basePlus45);
                a2 = (baseLineAngle >= a45 ? baseLess90 : a45);
            } else {
                a2 = std::min(a90, baseLess45);
                a1 = (baseLineAngle <= -a45 ? basePlus90 : a45);
            }
        }
        
        // Decide on curve height. It will be 1/3 of the distance, with
        // a maximum of 3.0 lines and a minimum of 1.5 lines
        const auto run = _x3 - _x0;
        const auto rise = _y3 - _y0;
        const auto d = sqrt(run * run + rise * rise);
        auto q = std::min(tenthsToStaff(30.0), d * 0.33f);
        q = std::max(q, tenthsToStaff(15.0));
        
        // Reduce height by half if one angle is horizontal (less than 3º)
        if (a1 < 0.05 || a2 < 0.05) {
            q *= 0.5;
        }
        
        // Bezier tangents lengths
        auto b1 = 0.0;
        auto b2 = 0.0;
        if (isBelow) {
            b1 = a90 - a1 - baseLineAngle;
            b2 = a90 - a2 + baseLineAngle;
        } else {
            b1 = a90 - a1 + baseLineAngle;
            b2 = a90 - a2 - baseLineAngle;
        }
        
        const auto t1 = abs(q / cos(b1));
        const auto t2 = abs(q / cos(b2));
        
        const auto xa1 = t1 * cos(a1);
        const auto ya1 = t1 * sin(a1);
        
        const auto xa2 = t2 * cos(a2);
        const auto ya2 = t2 * sin(a2);
        
        // Compute Bezier points
        if (isBelow) {
            _x1 = _x0 + xa1;
            _y1 = _y0 + ya1;  // y axis reversed
            
            _x2 = _x3 - xa2;
            _y2 = _y3 + ya2;  // y axis reversed
        } else {
            _x1 = _x0 + xa1;
            _y1 = _y0 - ya1;  // y axis reversed
            
            _x2 = _x3 - xa2;
            _y2 = _y3 - ya2;  // y axis reversed
        }
    }
}

void
Curve::methodAsymmetricCurve(double inHeight)
{
    // Compute angles to intermediate points.
    // Remember the greatest angles and double them.
    
    // Avoid drawing the approximate circle
    _radius = 0.0;
    
    // Determine max angles a1 & a2 (referred to the horizontal)
    
    // Get max angle from the start point (referred to the horizontal)
    auto a1 = 0.0;       //angles (in radians) for the control points
    auto a2 = 0.0;
    const auto numPoints = _dataPoints.size() - 2;
    for (size_t i = 1; i <= numPoints; ++i) {
        const auto dx = _dataPoints[i].x - _x0;
        const auto dy = abs(_y0 - _dataPoints[i].y);
        const auto a = atan2(dy, dx);               //get angle
        
        if (a > a1) {
            // Save data for greatest angle
            a1 = a;
        }
    }
    
    // Get max angle from the end point
    for (size_t i=1; i <= numPoints; ++i) {
        const auto dx = _x3 - _dataPoints[i].x;
        const auto dy = abs(_y3 - _dataPoints[i].y);
        const auto a = atan2(dy, dx);
        if (a2 < a) {
            a2 = a;
        }
    }
    
    // Double the angles
    a1 += a1;
    a2 += a2;
    
    // Limit angles to 90º
    a1 = std::min(a1, a90);
    a2 = std::min(a2, a90);
    
    // Compute slur (p0, p3, a1, a2, height)
    // a1, a2 references to the horizontal
    
    // Compute Bezier tangents' length
    const auto b = atan((_y0 - _y3) / (_x3 - _x0));   //y axis reversed
    const auto t1 = inHeight / sin(a1+b);
    const auto t2 = inHeight / sin(a2+b);
    
    // Calculate the control points
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    if (isBelow) {
        _x1 = _x0 + t2 * cos(a1);
        _y1 = _y0 + t2 * sin(a1);
        _x2 = _x3 - t1 * cos(a2);
        _y2 = _y3 + t1 * sin(a2);
    } else {
        _x1 = _x0 + t1 * cos(a1);
        _y1 = _y0 - t1 * sin(a1);
        _x2 = _x3 - t2 * cos(a2);
        _y2 = _y3 - t2 * sin(a2);
    }
}

void
Curve::computeDefaultCurve()
{
    // A flat symmetrical curve.
    //
    // The height h is incremented (to a maximum of 3.5 lines) as
    // D increases, add more curvature.
    //
    // The proportion d/D increases as D increases to increase the
    // start and end points curvature.
    
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    
    // Determine height, in tenths
    auto h = 0.0;
    if (_distance < 25.0) {
        h = 6.0;
    } else if (_distance < 50.0f) {
        h = 10.0;
    } else if (_distance < 100.0f) {
        h = 10.0f + (_distance - 50.0f) * 0.12f;
    } else if (_distance < 300.0f) {
        h = 18.0f + (_distance - 100.0f) * 0.045f;
    } else if (_distance < 600.0f) {
        h = 26.0f + (_distance - 300.0f) * 0.016666666f;
    } else {
        h = 32.0;
    }
    
    // Determine distance
    auto d = _distance;
    if (d < 55.0) {
        d = 0.5f * _distance;
    } else if (_distance < 200.0) {
        d = 0.6 * _distance;
    } else {
        d = 0.7 * _distance;
    }
    
    // Final height
    _thickness = tenthsToStaff(Thickness);
    _height = tenthsToStaff(h);
    
    // Determine tangent length
    const auto d1 = tenthsToStaff((_distance - d) / 2.0);
    
    // Peak point when this Bezier is horizontal
    _xm = _x0 + _pixelDistance / 2.0;
    _ym = _y0 + 0.75 * (isBelow ? _height : -_height);
    
    // Shifts from start/end points for control points
    // as y axis is reversed, sinb has the sign changed
    const auto dx1 = d1 * _cosb + _height * _sinb;
    const auto dx2 = d1 * _cosb - _height * _sinb;
    const auto dy1 = _height * _cosb - d1 * _sinb;
    const auto dy2 = _height * _cosb + d1 * _sinb;
    
    // Control points
    _x1 = _x0 + (isBelow ? dx2 : dx1);
    _y1 = _y0 + (isBelow ? dy2 : -dy1);
    _x2 = _x3 - (isBelow ? dx1 : dx2);
    _y2 = _y3 + (isBelow ? dy1 : -dy2);
}

void
Curve::computeMultiplePointsCurve()
{
    computeDefaultCurve();
    
    // If the curve has insufficient height, try to improve it.
    const auto distance = checkReferencePointLimits();
    if (distance > 0.0) {
        improveLowCurve(distance);
    }
}

double
Curve::checkReferencePointLimits()
{
    // Check that all reference points are above/below the slur.
    // Return 0.0 if the slur fits perfectly. Otherwise, return the required
    // distance to shift the slur so that it overpasses all reference points.
    
    // Finding the distance between a reference point and the Bezier curve requires many
    // computations: solving a cubic equation to find t, then use t to find the point on
    // the Bezier and, finally, compute the distance between the two points.
    // To save time, the slur will be approximated by a circular arc. This is
    // safe as the arc is always below the Bezier (when slur up) or above it (slur down).
    // And the distance from the arc to the ref point is a cheap computation.
    computeApproximateArc();
    
    // Check if distance from the reference points to the circle center
    // is greater than the radius.
    const auto r2 = _radius * _radius;
    
    auto maxDistance = 0.0;
    Point peak{0.0, 0.0};
    
    for (size_t i = 1; i < _dataPoints.size() - 1; ++i) {
        const auto x = _dataPoints[i].x;
        const auto y = _dataPoints[i].y;
        const auto dx2 = (_xc - x) * (_xc - x);
        const auto dist2 = dx2 + (_yc - y) * (_yc - y);
        if (dist2 > r2) {
            const auto dy = abs(_yc - y) - sqrt(r2 - dx2);
            if (dy > maxDistance) {
                peak.x = x;
                peak.y = y;
                maxDistance = dy;
            }
        }
    }
    
    return maxDistance;
}

void
Curve::computeApproximateArc()
{
    // For a symmetrical horizontal Bezier:
    _xc = _xm;
    
    const auto length = _x0 - _xc;
    const auto slope = _y0 - _ym;
    _yc = (length * length + (_y0 + _ym) * slope) / (2.0 * slope);
    
    _radius = abs(_ym - _yc);
    
    // If the Bezier is not horizontal, rotate circle center around P0
    // new circle center (sinb reversed, as y axis is reversed)
    const auto dx = _xc - _x0;
    const auto dy = abs(_yc - _y0);
    
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    if (isBelow) {
        _xc = _x0 + dx * _cosb + dy * _sinb;;
        _yc = _y0 - dy * _cosb + dx * _sinb;
    } else {
        _xc = _x0 + dx * _cosb - dy * _sinb;
        _yc = _y0 + dy * _cosb + dx * _sinb;
    }
}

void
Curve::improveLowCurve(double inDistance)
{
    // The slur does not overpass the reference points.
    // inDistance is the required distance to overpass all points.
    
    // If the distance is less than 3 lines, shift the slur
    if (inDistance < tenthsToStaff(30.0)) {
        shiftBezierUpDown(inDistance);
    } else {
        // Too much distance to solve by shifting the slur.
        // Let's try an asymmetrical Bezier with more curvature.
        _height += inDistance;
        methodAsymmetricCurve(_height);
        // TODO: Now distance to reference points from this new slur should be
        // measured and if not overpassing all points, shift the slur.
        // For now, apply a fix shift.
        shiftBezierUpDown(tenthsToStaff(10.0));
    }
}

void
Curve::shiftBezierUpDown(double inDistance)
{
    const auto isBelow = determinePlacement() == PlacementType::BELOW_ITEM;
    if (!isBelow) {
        inDistance = -inDistance;
    }
    
    _y0 += inDistance;
    _y1 += inDistance;
    _y2 += inDistance;
    _y3 += inDistance;
}

const Note&
Curve::getRelevantNoteForChord(const Note& inNote)
{
    if (!inNote.getIsChordMember()) {
        return inNote;
    }
    
    const auto chord = inNote.getChord();
    if (chord->getNotes().empty()) {
        return inNote;
    }
    
    switch (mPlacement)
    {
    case PlacementType::ABOVE_ITEM:
        return chord->getTopNote();
        
    case PlacementType::BELOW_ITEM:
        return chord->getBottomNote();
        
    default:
        {
            const auto noteID = inNote.getItemID();
            
            const auto& topNote = chord->getTopNote();
            if (noteID == topNote.getItemID()) {
                return topNote;
            }

            const auto& botomNote = chord->getBottomNote();
            if (noteID == botomNote.getItemID()) {
                return botomNote;
            }
        }
    }
    
    return inNote;
}

Rect
Curve::getBoundingBox() const
{
    const auto top = std::min({_y0, _y1, _y2, _y3});
    const auto left = std::min({_x0, _x1, _x2, _x3});
    const auto bottom = std::max({_y0, _y1, _y2, _y3});
    const auto right = std::max({_x0, _x1, _x2, _x3});
    return {top, left, bottom, right};
}

// MARK: - Dragging

void
Curve::drag(const CurrentState& inState)
{
    LOG_F(WARNING, "TODO: Add drag support to curve");
}

// MARK: - Placement

PlacementType
Curve::determinePlacement() const
{
    if (mPlacement != PlacementType::AUTOMATIC) {
        return mPlacement;
    }

    auto handleChord = false;

    const auto beginChord = mBeginTemporal.getIsChordMember();
    const auto endChord = mEndTemporal.getIsChordMember();
        
    if (beginChord || endChord) {
        const auto chord = beginChord ? mBeginTemporal.getChord()
                                      : mEndTemporal.getChord();
        handleChord = !chord->getNotes().empty();
    }
        
    if (handleChord) {
        
        const auto chord = beginChord ? mBeginTemporal.getChord()
                                      : mEndTemporal.getChord();
        KOMP_ASSERT(chord);        
        const auto chordID = chord->getTopNote().getItemID();
        const auto noteID = beginChord ? mBeginTemporal.getItemID() 
                                       : mEndTemporal.getItemID();
        if (noteID == chordID) {
            return PlacementType::ABOVE_ITEM;
        } else {
            return PlacementType::BELOW_ITEM;
        }
        
    } else {
        const auto beginUp = mBeginTemporal.getStemPolicy().getIsStemUp();
        const auto endUp = mEndTemporal.getStemPolicy().getIsStemUp();
        
        const auto bothUp = beginUp && endUp;
        const auto bothDown = !beginUp && !endUp;
        
        if (bothUp) {
            return PlacementType::BELOW_ITEM;
        } else if (bothDown) {
            return PlacementType::ABOVE_ITEM;
        } else {
            return PlacementType::ABOVE_ITEM;
        }
    }
}

// MARK: - Accessors

void
Curve::setX(double inValue)
{
    Spatial::setX(inValue);
    update();
}

void
Curve::setEndTime(int inValue)
{
    mEndTime = inValue;
    update();
}

/// \brief Clear references to ourself with any associated temporal objects
void
Curve::reset()
{
    if (mBeginTemporal.getAttachments().hasOutgoingCurve()) {
        mBeginTemporal.getAttachments().removeOutgoingCurve(*this);
        mBeginTemporal.unregisterObserver(*this);
    }
    
    if (mEndTemporal.getAttachments().hasIncomingCurve()) {
        mEndTemporal.getAttachments().removeIncomingCurve(*this);
        mEndTemporal.unregisterObserver(*this);
    }
    
    const auto members = getMembers();
    for (Temporal& member : members) {
        member.unregisterObserver(*this);
    }
}

void
Curve::copyState(const Curve& other)
{
    Spatial::operator=(other);
    mEndTime = other.mEndTime;
    mBeginTemporal = other.mBeginTemporal;
    mEndTemporal = other.mEndTemporal;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
