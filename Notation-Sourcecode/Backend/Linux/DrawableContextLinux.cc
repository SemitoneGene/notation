// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableContextLinux.h"

namespace komp
{
DrawableContextLinux::DrawableContextLinux()
{
    mRoot = std::make_unique<QGraphicsScene>();
}

DrawableContextLinux::~DrawableContextLinux()
{
    const auto items = mRoot->items();
    for (auto item : items) {
        mRoot->removeItem(item);
    }
}

void
DrawableContextLinux::addLayer(DrawableLayer* inLayer) const
{
    const auto layer = inLayer->getLayer();
    if (layer && layer->scene() != mRoot.get()) {
        QGraphicsItem* layer = inLayer->getLayer();
        mRoot->addItem(layer);
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
