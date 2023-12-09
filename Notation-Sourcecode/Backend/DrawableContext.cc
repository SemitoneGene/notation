// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableContext.h"

// System
#include <cstdlib>

// Local
#include "DrawableLayer.h"
#include "Preferences.h"
#include "Throw.h"

#ifdef KOMP_BUILD_APPLE
#include "DrawableContextMacOS.h"
#elif KOMP_BUILD_LINUX
#include "DrawableContextLinux.h"
#endif

namespace komp
{
DrawableContext::DrawableContext()
{
#ifdef KOMP_BUILD_APPLE
    mDrawableContextImpl = std::make_unique<DrawableContextMacOS>();
#elif KOMP_BUILD_LINUX
    mDrawableContextImpl = std::make_unique<DrawableContextLinux>();
#endif
}

DrawableContext::~DrawableContext()
{
}

void
DrawableContext::addLayer(DrawableLayer* inLayer) const
{
    mDrawableContextImpl->addLayer(inLayer);
}

void*
DrawableContext::getLayer() const {
    return mDrawableContextImpl->getLayer();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
