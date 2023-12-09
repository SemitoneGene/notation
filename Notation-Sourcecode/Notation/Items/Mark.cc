// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Mark.h"

// System
#include <unordered_map>

// Local
#include "DrawableContext.h"
#include "DrawableString.h"
#include "DrawableText.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "MarkImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Temporal.h"

namespace komp
{

static const auto SymbolSnap = 2.0;

// MARK: - MarkRange

struct MarkRange
{
    MarkRange(const std::set<MarkType>& inMarks,
              const std::vector<MarkType>& inRanges)
    : marks{inMarks}
    , ranges{inRanges}
    {
    }
    
    bool contains(MarkType inMark) const
    {
        return marks.find(inMark) != marks.end();
    }
    
    int
    getIndex(MarkType inMark) const {
        return static_cast<int>(std::distance(std::begin(ranges),
                                              std::find(std::begin(ranges),
                                                        std::end(ranges),
                                                        inMark)));
    }

    int
    lastIndex() const {
        return static_cast<int>(marks.size() - 1);
    }

    MarkType
    at(size_t index) const {
        return ranges.at(index);
    }
    
    const std::set<MarkType>& marks;
    const std::vector<MarkType>& ranges;

};

// MARK: - Bows

static const std::vector<MarkType> sBowRanges =
{
    MarkType::STRINGS_BOW_DOWN,
    MarkType::STRINGS_BOW_UP
};

static const std::set<MarkType> sBows(std::cbegin(sBowRanges),
                                      std::cend(sBowRanges));
static const auto sBowRange = MarkRange(sBows, sBowRanges);

// MARK: - Dynamics

static const std::vector<MarkType> sDynamicRanges =
{
    MarkType::DYNAMIC_NIENTE,
    MarkType::DYNAMIC_PPPPPP,
    MarkType::DYNAMIC_PPPPP,
    MarkType::DYNAMIC_PPPP,
    MarkType::DYNAMIC_PPP,
    MarkType::DYNAMIC_PP,
    MarkType::DYNAMIC_P,
    MarkType::DYNAMIC_MP,
    MarkType::DYNAMIC_MF,
    MarkType::DYNAMIC_F,
    MarkType::DYNAMIC_FF,
    MarkType::DYNAMIC_FFF,
    MarkType::DYNAMIC_FFFF,
    MarkType::DYNAMIC_FFFFF,
    MarkType::DYNAMIC_FFFFFF,
    MarkType::DYNAMIC_SF,
    MarkType::DYNAMIC_SFP,
    MarkType::DYNAMIC_SFPP,
    MarkType::DYNAMIC_SFZ,
    MarkType::DYNAMIC_SFFZ,
    MarkType::DYNAMIC_FP,
    MarkType::DYNAMIC_FZ,
    MarkType::DYNAMIC_RF,
    MarkType::DYNAMIC_RFZ
};

static const std::set<MarkType> sDynamics(std::cbegin(sDynamicRanges),
                                          std::cend(sDynamicRanges));

static const auto sDynamicRange = MarkRange(sDynamics, sDynamicRanges);

// MARK: - Mordents

static const std::vector<MarkType> sMordentRanges =
{
    MarkType::MORDENT,
    MarkType::MORDENT_INVERTED
};

static const std::set<MarkType> sMordents(std::cbegin(sMordentRanges),
                                          std::cend(sMordentRanges));
static const auto sMordentRange = MarkRange(sMordents, sMordentRanges);


// MARK: - Tremolos

static const std::vector<MarkType> sTremoloRanges =
{
    MarkType::TREMOLO_FIVE,
    MarkType::TREMOLO_FOUR,
    MarkType::TREMOLO_THREE,
    MarkType::TREMOLO_TWO,
    MarkType::TREMOLO_ONE
};

static const std::set<MarkType> sTremolos(std::cbegin(sTremoloRanges),
                                          std::cend(sTremoloRanges));
static const auto sTremoloRange = MarkRange(sTremolos, sTremoloRanges);

// MARK: - Turns

static const std::vector<MarkType> sTurnRanges =
{
    MarkType::TURN,
    MarkType::TURN_INVERTED
    //MarkType::TURN_SLASH
};

static const std::set<MarkType> sTurns(std::cbegin(sTurnRanges),
                                       std::cend(sTurnRanges));

struct TurnRange
{
    static bool contains(MarkType inMark)
    {
        return sTurns.find(inMark) != sTurns.end();
    }
    
    static size_t
    getIndex(MarkType inMark) {
        return std::distance(std::begin(sTurnRanges),
                             std::find(std::begin(sTurnRanges),
                                       std::end(sTurnRanges),
                                       inMark));
    }
};

static const auto sTurnRange = MarkRange(sTurns, sTurnRanges);


// MARK: - Mark

Mark::Mark(const MusicLocation& inLocation,
           MarkType inMarkType,
           PlacementType inPlacementType,
           const RelativePoint& inRelativePoint)
: komp::AttachedItem(inLocation, inRelativePoint)
, mMarkData(FontTheme::instance().MarkInfo.at(inMarkType))
{
    if (inPlacementType != PlacementType::AUTOMATIC) {
        mMarkData.setPlacementType(inPlacementType);
    }
}


Mark::Mark(const Mark& other)
: komp::AttachedItem(other)
, mMarkData(other.mMarkData)
{
    
}


Mark&
Mark::operator=(const Mark& other)
{
    copyState(other);
    return *this;
}


Mark::Mark(Mark&& other) = default;


Mark&
Mark::operator=(Mark&& other) = default;


const ItemTraits&
Mark::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Mark);
}

void
Mark::copyState(const Mark& other)
{
    AttachedItem::operator=(other);
    mMarkData = other.mMarkData;
}

ItemUPtr
Mark::getMemento() const
{
    return std::make_unique<Mark>(*this);
}

void
Mark::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Mark* downcastedP = dynamic_cast<const Mark*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

void
Mark::update()
{
    calculateAboveBelow();
}

void
Mark::observedItemChanged(const Item& observedItem,
                          const ItemChanges& inChanges)
{
    update();
}

Rect
Mark::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    return drawable->getFrame(getDrawPoint());
}

Region
Mark::getRegion() const
{
    return ItemImpl::getRegion<DrawableString>(*this);
}

void
Mark::createDrawable()
{
    if (getIsExpression()) {
        ItemImpl::createDrawable<DrawableString>(*this);
    } else {
        ItemImpl::createDrawable<DrawableText>(*this);
    }
}

void
Mark::inserted()
{
    AttachedItem::inserted();
    
    auto& item = getRelativePoint().getItem();
    item.registerObserver(*this);
    
    setDrawableData();
    
    notifyObservers({ItemChangeType::Inserted});
}

void
Mark::removed()
{
    auto& item = getRelativePoint().getItem();
    item.unregisterObserver(*this);
    AttachedItem::removed();
    notifyObservers({ItemChangeType::Removed});
}

double
Mark::getX() const
{
    return AttachedItem::getX();
}


void
Mark::setX(double inValue)
{
    AttachedItem::setX(inValue);
}


double
Mark::getScoreX() const
{
    return AttachedItem::getScoreX();
}


double
Mark::getScoreY() const
{
    return AttachedItem::getScoreY();
}


int
Mark::getSlot() const
{
    return AttachedItem::getSlot();
}


void
Mark::setSlot(int inValue)
{
    AttachedItem::setSlot(inValue);
}


void
Mark::beginDrag(const CurrentState& inState)
{
    mDragStartMark = mMarkData.getMarkType();
}


void
Mark::drag(const CurrentState& inState)
{
    const auto& measure = getLocation().getMeasure();
    auto& score = measure.getScore();
    auto& selection = score.getSelection();
    auto& relativePoint = getRelativePoint();
    auto& item = relativePoint.getItem();
    if (selection->contains(item)) {
        return;
    }
    
    const auto& delta = dragDelta();
    if (delta.x == 0 && delta.y == 0) {
        return;
    }
    
    if (!mDragAxisLock) {
        if (abs(delta.x) > abs(delta.y)) {
            mDragAxisX = true;
        } else {
            mDragAxisY = true;
        }
        mDragAxisLock = true;
    }
    
    if (mDragAxisX) {
        
        if (handleMarkHorizontalDrag()) {
            relativePoint.addX(delta.x);
            ItemImpl::setNeedsDisplay(*this);
        }
        
    } else {
        
        if (handleMarkVerticalDrag()) {
            notifyObservers({ItemChangeType::TemporalType});
        } else {
            relativePoint.addYPixels(delta.y);
            calculateAboveBelow();
        }
        
        ItemImpl::setNeedsDisplay(*this);
    }
}

void
Mark::endDrag(const CurrentState& inState)
{
    mDragAxisLock = false;
    mDragAxisX = false;
    mDragAxisY = false;
    mDragStartMark = MarkType::NONE;
}

// MARK: - Drag Handling

bool
Mark::handleMarkHorizontalDrag()
{
    if (getIsTechnique()) {
        return false;
    }

    if (getIsFermata()) {
        return false;
    }
    
    return true;
}

bool
Mark::handleMarkVerticalDrag()
{
    if (handleMarkRangeVerticalDrag(sBowRange)) {
        return true;
    }

    if (handleMarkRangeVerticalDrag(sDynamicRange)) {
        return true;
    }

    if (handleMarkRangeVerticalDrag(sMordentRange)) {
        return true;
    }

    if (handleMarkRangeVerticalDrag(sTremoloRange)) {
        return true;
    }

    if (handleMarkRangeVerticalDrag(sTurnRange)) {
        return true;
    }
    
    if (getIsTechnique()) {
        return true;
    }
    
    return false;
}

bool
Mark::handleMarkRangeVerticalDrag(const MarkRange& inRange)
{
    const auto markType = getMarkType();
    if (!inRange.contains(markType)) {
        return false;
    }
        
    const auto dragDistance = dragStart().y - dragLast().y;
    const auto step = static_cast<int>(floor(dragDistance / SymbolSnap));
    const auto startIndex = inRange.getIndex(mDragStartMark);
    const auto newIndex = std::clamp(startIndex + step, 0, inRange.lastIndex());
    const auto newMark = inRange.at(newIndex);
    
    if (newMark != markType) {
        const auto& font = FontTheme::instance();
        const auto smufl = font.MarkInfo.at(newMark);
        mMarkData = MarkData(smufl);
        setDrawableData();
        const auto centerX = MarkImpl::getHorizonalCenter(*this);
        getRelativePoint().setScoreX(centerX);
    }
    
    return true;
}

// MARK: - Placement

static constexpr const auto Above = "Above";
static constexpr const auto Below = "Below";

void
Mark::calculateAboveBelow()
{
    if (getIsExpression() || getIsTechnique()) {
        return;
    }
    
    const auto smufl = mMarkData.getString();
    const auto size = static_cast<int>(smufl.size());
    if (smufl.size() < 6) {
        return;
    }
    
    const auto pos = size - 5;
    const auto name = smufl.substr(0, pos);
    const auto loc = smufl.substr(pos);    
    if (loc != Above && loc != Below) {
        return;
    }
    
    const auto isSmuflAbove = (loc == Above);
    
    auto& item = getRelativePoint().getItem();
    KOMP_ASSERT(item.is<Temporal>());
    auto& temporal = item.as<Temporal>();
    const auto frames = temporal.getFrames();
    const auto headFrame = frames.head;
    const auto myFrame = getFrame();
    const auto headCenterY = headFrame.getCenter().y;
    const auto myCenterY = myFrame.getCenter().y;
    const auto isMarkAbove = (myCenterY <= headCenterY);
    
    if (isSmuflAbove && isMarkAbove) {
        return;
    }
    
    if (!isSmuflAbove && !isMarkAbove) {
        return;
    }
    
    const auto nameStr = std::string(name);
    const auto newName = isMarkAbove ? nameStr + Above : nameStr + Below;
    const auto& font = FontTheme::instance();
    const auto& smuflMap = font.SmuflMarkMap;
    const auto iter = smuflMap.find(newName);
    
    if (iter != smuflMap.cend()) {
        const auto markType = iter->second;
        const auto& markMap = font.MarkInfo;
        const auto dataIter = markMap.find(markType);
        if (dataIter != markMap.cend()) {
            mMarkData = dataIter->second;
        }
    }
    setDrawableData();
}

// MARK: - Playback

static constexpr const double DYNAMIC_MIX = 0.0;
static constexpr const double DYNAMIC_MAX = 100.0;

static const std::unordered_map<MarkType, double> sPlaybackLevels =
{
    { MarkType::DYNAMIC_PPPPPP, 17.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_PPPPP,  22.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_PPPP,   27.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_PPP,    32.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_PP,     37.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_P,      43.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_MP,     47.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_MF,     52.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_F,      57.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_FF,     62.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_FFF,    67.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_FFFF,   72.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_FFFFF,  82.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_FFFFFF, 87.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_SF,     57.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_SFZ,    57.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_SFFZ,   62.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_RFZ,    57.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_RF,     57.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_FZ,     57.5 / DYNAMIC_MIX },
    { MarkType::DYNAMIC_NIENTE, DYNAMIC_MAX }
};
    
double
Mark::getPlaybackVolume() const
{
    const auto type = mMarkData.getMarkType();
    const auto search = sPlaybackLevels.find(type);
    if (search != sPlaybackLevels.cend()) {
        return search->second;
    }
    
    return DYNAMIC_MAX;
}

double
Mark::getPlaybackDurationScale() const
{
    double result = 1.0;
    
    const auto markType = mMarkData.getMarkType();
    
    switch (markType)
    {
    case MarkType::TENUTO_ABOVE:
    case MarkType::TENUTO_BELOW:
        {
            result = 0.95;
            break;
        }
        
    case MarkType::STACCATO_ABOVE:
    case MarkType::STACCATO_BELOW:
        {
            result = 0.75;
            break;
        }
        
    default:
        break;
    }
    
    return result;
}

// MARK: - Getters

MarkType
Mark::getMarkType() const
{
    return mMarkData.getMarkType();
}


const MarkData&
Mark::getMarkData() const
{
    return mMarkData;
}


bool
Mark::getIsExpression() const
{
    return mMarkData.getMarkCategory() == MarkCategory::EXPRESSION;
}


bool
Mark::getIsFermata() const
{
    return mMarkData.getMarkCategory() == MarkCategory::FERMATA;
}


bool
Mark::getIsTechnique() const
{
    return mMarkData.getMarkCategory() == MarkCategory::TECHNIQUE;
}


void
Mark::setDrawableData()
{
    if (getIsExpression()) {
        const auto drawable = ItemImpl::getDrawable<DrawableString>(*this);
        drawable->setString(mMarkData.getString());
        const auto& font = Preferences::instance().ExpressionFont;
        drawable->setFontName(font.Font);
        drawable->setFontSize(font.Size);
    } else {
        const auto drawable = ItemImpl::getDrawable<DrawableText>(*this);
        drawable->setFontSize(getFontSize());
        drawable->setText(DrawableChars{mMarkData.getCharacter()});
    }
}


Rect
Mark::getTechniquesFrame(const Point& inPoint) const
{
    Rect result;
    if (getIsTechnique()) {
        const auto drawable = ItemImpl::getDrawable<DrawableText>(*this);
        result = drawable->getFrame(getDrawPoint());
    }
    return result;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

