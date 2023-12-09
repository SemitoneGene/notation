// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLayerFactoryLinux.h"

// Qt
#include <QtWidgets/QtWidgets>

// Local
#include "DrawableLayer.h"
#include "FontManager.h"

namespace komp
{
DrawableLayerFactoryLinux::DrawableLayerFactoryLinux()
{
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryLinux::createCurve(const DrawableContext& inContext,
                                       const Region& inRegion) const
{
    QPainterPath* path = inRegion;
    auto item = new QGraphicsPathItem(*path);
    return std::make_unique<DrawableLayer>(inContext, item);

}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryLinux::createEllipse(const DrawableContext& inContext,
                                         double inWidth,
                                         double inHeight) const
{
    auto item = new QGraphicsEllipseItem(0, 0, inWidth, inHeight);
    return std::make_unique<DrawableLayer>(inContext, item);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryLinux::createLine(const DrawableContext& inContext,
                                      const Point& inStart,
                                      const Point& inEnd) const
{
    auto item = new QGraphicsLineItem(inStart.x,
                                      inStart.y,
                                      inEnd.x,
                                      inEnd.y);
    QPen pen;
    pen.setCapStyle(Qt::PenCapStyle::FlatCap);
    pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    pen.setMiterLimit(10.0);
    item->setPen(pen);
    return std::make_unique<DrawableLayer>(inContext, item);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryLinux::createRectangle(const DrawableContext& inContext,
                                           const Rect& inBounds) const
{
    auto item = new QGraphicsRectItem(inBounds);
    return std::make_unique<DrawableLayer>(inContext, item);
}

std::unique_ptr<DrawableLayer>
DrawableLayerFactoryLinux::createText(const DrawableContext& inContext,
                                      std::string_view inFontName,
                                      double inFontSize,
                                      const char* inText,
                                      const Point& inPosition,
                                      const DrawableColor& inColor) const
{
    auto& fontManager = komp::FontManager::instance();
    auto font = fontManager.getFont(inFontName, inFontSize);
    font.setPointSizeF(inFontSize);
    auto item = new QGraphicsTextItem(inText);
    item->setPos(inPosition);
    item->setFont(font);
    item->setDefaultTextColor(inColor);
    return std::make_unique<DrawableLayer>(inContext, item);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
