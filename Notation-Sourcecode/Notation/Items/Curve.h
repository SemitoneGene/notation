// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

// Local
#include "CurveParams.h"
#include "Spatial.h"


// Forward declarations
namespace komp
{
class StaffSystem;
class Temporal;
}

namespace komp
{

class KOMP_EXPORT Curve : public Spatial
{
public:
    Curve(const Curve& inCurve);
    
    virtual ~Curve() = default;
    Curve& operator=(const Curve& other);
    
    virtual const ItemTraits& getItemTraits() const override { throw std::runtime_error{"not implemented"}; }
    virtual ItemUPtr getMemento() const override;
    virtual void restoreMemento(const Item& inItem) override;
    virtual void observedItemChanged(const Item& observedItem,
                                     const ItemChanges& inChanges) override;
    
    virtual Rect getFrame() const override;
    virtual Region getRegion() const override;
    virtual Point getAnchor() const override { return POINT_ZERO; }
    
    virtual void inserted() override;
    virtual void removed() override;
    
    ///@{ Draggable /////////////////////////////////////////////
    virtual void drag(const CurrentState& inState) override;
    ///@} Draggable /////////////////////////////////////////////
    
    virtual void setX(double inValue) override;
    
    int getEndTime() const { return mEndTime; }
    void setEndTime(int inValue);
    virtual void update() override;
    
    void reset();
    
    void getPoints(Point& outStart,
                   Point& outControlOne,
                   Point& outControlTwo,
                   Point& outEnd) const;
    
    Temporal& getBeginTemporal() const { return mBeginTemporal; }
    Temporal& getEndTemporal() const { return mEndTemporal; }
    
protected:
    friend class ItemRegistry;
    
    Curve(const MusicLocation& inLocation, const CurveParams& inParams);
    virtual void configure();
    virtual void createDrawable() override final;
    
    Temporal& mBeginTemporal;
    Temporal& mEndTemporal;
    
private:
    Curve();
    
    void copyState(const Curve& other);
    
    void computePoints();
    void computePoint(bool inBegin,
                      double& ioX,
                      double& ioY);
    void computeEndPoint();
    void applyRulesForStartEndPoints();
    void findContourReferencePoints();
    void computeBaseline();
    void computeControlPoints();
    void methodForTwoPoints();
    void methodAsymmetricCurve(double inHeight);
    void computeDefaultCurve();
    void computeMultiplePointsCurve();
    double checkReferencePointLimits();
    void computeApproximateArc();
    void improveLowCurve(double inDistance);
    void shiftBezierUpDown(double inDistance);
    const Note& getRelevantNoteForChord(const Note& inNote);
    
    Rect getBoundingBox() const;

    komp::TemporalRefVec getMembers() const;
    
    PlacementType determinePlacement() const;
        
    int mEndTime{0};
    PlacementType mPlacement{PlacementType::AUTOMATIC};
    
    // Intermediate reference points
    std::vector<Point> _dataPoints;
        
    double _thickness{0.0};
    
    // Curve baseline data, for intermediate computations
    double _pixelDistance{0.0}; // Length of curve baseline in pixels
    double _distance{0.0};      // Length of curve baseline in tenths
    double _height{0.0};        // Height of curve
    double _cosb{1.0};          // Cos(baseline angle)
    double _sinb{1.0};          // Sin(baseline angle))
    
    // Curve control points during intermediate computations
    // Start point
    mutable double _x0{0.0};
    mutable double _y0{0.0};
    
    // First control point, near start point
    mutable double _x1{0.0};
    mutable double _y1{0.0};
    
    // Second control point, near end point
    mutable double _x2{0.0};
    mutable double _y2{0.0};
    
    // End point
    mutable double _x3{0.0};
    mutable double _y3{0.0};
    
    // Curve middle point and approximate arc for intermediate computations
    // Bezier middle point (t = 0.5)
    double _xm{0.0};
    double _ym{0.0};
    
    // Center of circle
    double _xc{0.0};
    double _yc{0.0};
    
    double _radius = {0.0};
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
