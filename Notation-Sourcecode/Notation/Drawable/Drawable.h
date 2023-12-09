// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"
#include "DrawResult.h"
#include "DrawParams.h"

namespace komp
{    
    class Drawable
    {
    public:
        virtual ~Drawable() = default;
        
        /// Removes the drawable from the parent drawable context
        virtual void removeFromParent() const = 0;

        /// Restores the drawable to the previous parent
        virtual void restoreParent() const = 0;

        /// the offset, in pixels, will be added to the given
        /// draw point when drawing and calculating Frame, Region, etc
        /// this allows a composite of Drawables to have positions
        /// relative to one-another when receiving the same draw point
        virtual Point getOffset() const = 0;

        /// the offset, in pixels, will be added to the given
        /// draw point when drawing and calculating Frame, Region, etc
        /// this allows a composite of Drawables to have positions
        /// relative to one-another when receiving the same draw point
        virtual void setOffset(const Point& inPoint) = 0;
        
        /// Set the z-order of layer, which specifies the order layers
        /// will be drawn over one another.
        virtual void setZOrder(int inOrder) = 0;

        /// Returns the bounding box for this Drawable. pixels.
        virtual Rect getFrame(const Point& inPoint) const = 0;

        /// Returns the width in pixels
        virtual double getWidth() const = 0;

        /// Returns the height in pixels
        virtual double getHeight() const = 0;

        /// Returns the bounding box for this Drawable. pixels.
        virtual Region getRegion(const Point& inPoint) const = 0;
        
        /// Returns the visual anchor point of the Drawable. pixels.
        virtual Point getAnchor(const Point& inPoint) const = 0;
        
        // Set the scale factor
        virtual void setScale(double inScale) = 0;

        // Set the rotation
        virtual void setRotation(double inDegrees) = 0;

        /// Calculate the Drawable metrics
        virtual DrawResult getMetrics(const Point& inPoint) const = 0;
        
        /// Render the Drawable.
        virtual void draw(const DrawParams& inDrawParams) const = 0;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
