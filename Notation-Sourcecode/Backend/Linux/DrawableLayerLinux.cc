// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLayerLinux.h"

// Komp
#include "Throw.h"

namespace komp
{

DrawableLayerLinux::DrawableLayerLinux()
{
    mLayer = new QGraphicsPathItem();
}

DrawableLayerLinux::DrawableLayerLinux(const Region& inPath)
{
    const auto path = const_cast<Region&>(inPath).getPath();
    mLayer = new QGraphicsPathItem(path);
}

DrawableLayerLinux::DrawableLayerLinux(QGraphicsItem* inLayer)
{
    mLayer = clone(inLayer);
}

DrawableLayerLinux::~DrawableLayerLinux()
{
    delete mLayer;
}

DrawableLayerLinux::DrawableLayerLinux(const DrawableLayerLinux& inCopy)
{
    mLayer = clone(inCopy.getLayer());
}

DrawableLayerLinux&
DrawableLayerLinux::operator=(const DrawableLayerLinux& inOther)
{
    mLayer = clone(inOther.getLayer());
    return *this;
}

void
DrawableLayerLinux::removeFromParent()
{
    mLayer->setParentItem(nullptr);
    
    const auto scene = mLayer->scene();
    if (scene) {
        scene->removeItem(mLayer);
    }
}

void
DrawableLayerLinux::setLineWidth(double inValue)
{
    auto setValue = [inValue](QGraphicsItem* inItem) {
        if (const auto shapeItem = dynamic_cast<QAbstractGraphicsShapeItem*>(inItem)) {
            auto pen = shapeItem->pen();
            pen.setWidthF(inValue);
            shapeItem->setPen(pen);
        } else if (const auto lineItem = dynamic_cast<QGraphicsLineItem*>(inItem)) {
            auto pen = lineItem->pen();
            pen.setWidthF(inValue);
            lineItem->setPen(pen);
        }
    };
    
    const auto layer = static_cast<QGraphicsItem*>(mLayer);
    if (layer->type() == QGraphicsEllipseItem::Type ||
        layer->type() == QGraphicsLineItem::Type ||
        layer->type() == QGraphicsPathItem::Type ||
        layer->type() == QGraphicsRectItem::Type) {
        setValue(layer);
    }
}

void
DrawableLayerLinux::setMiterLimit(double inValue)
{
    auto setValue = [inValue](QGraphicsItem* inItem) {
        if (const auto shapeItem = dynamic_cast<QAbstractGraphicsShapeItem*>(inItem)) {
            auto pen = shapeItem->pen();
            pen.setMiterLimit(inValue);
            shapeItem->setPen(pen);
        } else if (const auto lineItem = dynamic_cast<QGraphicsLineItem*>(inItem)) {
            auto pen = lineItem->pen();
            pen.setMiterLimit(inValue);
            lineItem->setPen(pen);
        }
    };
    
    const auto layer = static_cast<QGraphicsItem*>(mLayer);
    if (layer->type() == QGraphicsEllipseItem::Type ||
        layer->type() == QGraphicsLineItem::Type ||
        layer->type() == QGraphicsPathItem::Type ||
        layer->type() == QGraphicsRectItem::Type) {
        setValue(layer);
    }
}

void
DrawableLayerLinux::setFillColor(const DrawableColor& inValue)
{
    auto setValue = [inValue](QGraphicsItem* inItem) {
        if (const auto shapeItem = dynamic_cast<QAbstractGraphicsShapeItem*>(inItem)) {
            shapeItem->setBrush(QBrush(inValue));
        }
    };
    
    const auto layer = static_cast<QGraphicsItem*>(mLayer);
    if (layer->type() == QGraphicsEllipseItem::Type ||
        layer->type() == QGraphicsPathItem::Type ||
        layer->type() == QGraphicsRectItem::Type) {
        setValue(layer);
    }
}

void
DrawableLayerLinux::disableFillColor()
{
    auto setValue = [](QGraphicsItem* inItem) {
        if (const auto shapeItem = dynamic_cast<QAbstractGraphicsShapeItem*>(inItem)) {
            shapeItem->setBrush(Qt::NoBrush);
        }
    };
    
    const auto layer = static_cast<QGraphicsItem*>(mLayer);
    if (layer->type() == QGraphicsEllipseItem::Type ||
        layer->type() == QGraphicsPathItem::Type ||
        layer->type() == QGraphicsRectItem::Type) {
        setValue(layer);
    }
}

void
DrawableLayerLinux::setStrokeColor(const DrawableColor& inValue)
{
    auto setValue = [inValue](QGraphicsItem* inItem) {
        if (const auto shapeItem = dynamic_cast<QAbstractGraphicsShapeItem*>(inItem)) {
            auto pen = shapeItem->pen();
            pen.setColor(inValue);
            shapeItem->setPen(pen);
        } else if (const auto lineItem = dynamic_cast<QGraphicsLineItem*>(inItem)) {
            auto pen = lineItem->pen();
            pen.setColor(inValue);
            lineItem->setPen(pen);
        }
    };
    
    const auto layer = static_cast<QGraphicsItem*>(mLayer);
    if (layer->type() == QGraphicsEllipseItem::Type ||
        layer->type() == QGraphicsLineItem::Type ||
        layer->type() == QGraphicsPathItem::Type ||
        layer->type() == QGraphicsRectItem::Type) {
        setValue(layer);
    }
}

void
DrawableLayerLinux::disableStrokeColor()
{
    auto setValue = [](QGraphicsItem* inItem) {
        if (const auto shapeItem = dynamic_cast<QAbstractGraphicsShapeItem*>(inItem)) {
            shapeItem->setPen(Qt::NoPen);
        }
    };
    
    const auto layer = static_cast<QGraphicsItem*>(mLayer);
    if (layer->type() == QGraphicsEllipseItem::Type ||
        layer->type() == QGraphicsLineItem::Type ||
        layer->type() == QGraphicsPathItem::Type ||
        layer->type() == QGraphicsRectItem::Type) {
        setValue(layer);
    }
}

void
DrawableLayerLinux::moveTo(const Point& inPosition)
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    if (item->type() == QGraphicsTextItem::Type) {
        const auto textItem = dynamic_cast<QGraphicsTextItem*>(item);
        const auto bounds = textItem->boundingRect();
        item->setPos(inPosition.x - (bounds.width() / 2.0),
                     inPosition.y - (bounds.height() / 2.0));
    } else {
        item->setPos(inPosition.x, inPosition.y);
    }
}

void
DrawableLayerLinux::moveBy(const Point& inAmount)
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    item->moveBy(inAmount.x, inAmount.y);
}

void
DrawableLayerLinux::setZOrder(int inOrder)
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    item->setZValue(inOrder);
}

void
DrawableLayerLinux::setScale(double inScale)
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    item->setScale(inScale);
}

void
DrawableLayerLinux::setRotation(double inDegrees)
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    item->setRotation(inDegrees);
}

Region
DrawableLayerLinux::getPath() const
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    const auto shape = item->shape();
    return Region{QPainterPath(shape)};
}

Rect
DrawableLayerLinux::getBounds() const
{
    auto item = static_cast<QGraphicsItem*>(mLayer);
    return item->boundingRect();
}

QGraphicsItem*
DrawableLayerLinux::getLayer() const
{
    return mLayer;
}

QGraphicsItem*
DrawableLayerLinux::clone(QGraphicsItem* inItem) {
    
    if (!inItem) {
        return nullptr;
    }

    QGraphicsItem* newItem = nullptr;

    switch (inItem->type())
    {
    case QGraphicsEllipseItem::Type:
        {
            const auto original = static_cast<QGraphicsEllipseItem*>(inItem);
            newItem = new QGraphicsEllipseItem(original->rect());
            const auto actual = static_cast<QGraphicsEllipseItem*>(newItem);
            actual->setBrush(original->brush());
            actual->setPen(original->pen());
            break;
        }

    case QGraphicsLineItem::Type:
        {
            const auto original = static_cast<QGraphicsLineItem*>(inItem);
            newItem = new QGraphicsLineItem(original->line());
            const auto actual = static_cast<QGraphicsLineItem*>(newItem);
            actual->setPen(original->pen());
            break;
        }

    case QGraphicsPathItem::Type:
        {
            const auto original = static_cast<QGraphicsPathItem*>(inItem);
            newItem = new QGraphicsPathItem(original->path());
            const auto actual = static_cast<QGraphicsPathItem*>(newItem);
            actual->setBrush(original->brush());
            actual->setPen(original->pen());
            break;
        }
        
    case QGraphicsRectItem::Type:
        {
            const auto original = static_cast<QGraphicsRectItem*>(inItem);
            newItem = new QGraphicsRectItem(original->rect());
            const auto actual = static_cast<QGraphicsRectItem*>(newItem);
            actual->setBrush(original->brush());
            actual->setPen(original->pen());
            break;
        }
        
    case QGraphicsTextItem::Type:
        {
            const auto original = static_cast<QGraphicsTextItem*>(inItem);
            newItem = new QGraphicsTextItem(original->toPlainText());
            const auto actual = static_cast<QGraphicsTextItem*>(newItem);
            actual->setFont(original->font());
            actual->setDefaultTextColor(original->defaultTextColor());
            break;
        }
        
    default:
        KOMP_THROW("Unsupported QGraphicsItem");
    }

    newItem->setPos(inItem->pos());
    newItem->setRotation(inItem->rotation());
    newItem->setScale(inItem->scale());
    newItem->setTransform(inItem->transform());
    newItem->setZValue(inItem->zValue());    

    return newItem;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

