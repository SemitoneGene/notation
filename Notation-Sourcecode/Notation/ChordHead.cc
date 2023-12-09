// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordHead.h"

// Local
#include "Temporal.h"
#include "Throw.h"

namespace komp
{
    ChordHead::ChordHead(int inSlot, ItemID inItemID)
    : slot{inSlot}
    , side{ChordSide::Unknown}
    , mItemID{inItemID}
    , hasAccidental{false}
    , accidentalFrame{}
    , accidentalOffset{}
    , noteheadFrame{}
    , noteDrawPoint{}
    , chordDrawPoint{}
    , noteheadFrames{}
    {

    }


    ItemID
    ChordHead::getItemID() const
    {
        return mItemID;
    }


    // For testing
    void
    ChordHead::calculateHeadSidesNotPtr(ChordHeads& outChordHeads,
                                  StemDirection inStemDirection)
    {
        sortHeads(outChordHeads);

        ChordHeadPtrs ptrs;

        for (ChordHead& chordHead : outChordHeads) {
            ptrs.emplace_back(&chordHead);
        }

        calculateHeadSides(ptrs, inStemDirection);
    }


    void
    ChordHead::calculateHeadSides(ChordHeadPtrs& outChordHeads,
                                  StemDirection inStemDirection)
    {
        if (outChordHeads.empty()) {
            return;
        }

        if (inStemDirection == StemDirection::NONE) {
            inStemDirection = calculateStemDirection(outChordHeads);
        }

        const bool isUp = (inStemDirection == StemDirection::UP);

        bool isFirst = true;
        int previousSlot = isUp ? 9999 : -9999;
        ChordSide previousSide = ChordSide::Unknown;
        const auto preferredSide = isUp ? ChordSide::Left : ChordSide::Right;

        auto loopBody = [&](ChordHeadPtr& currentHead) {

            if (isFirst) {
                isFirst = false;
                currentHead->side = preferredSide;
            } else {
                const int slotChange = std::abs(previousSlot - currentHead->slot);
                if (slotChange < 2) {
                    currentHead->side = oppositeSide(previousSide);
                } else {
                    currentHead->side = preferredSide;
                }
            }

            previousSlot = currentHead->slot;
            previousSide = currentHead->side;
        };

        if (isUp) {
            std::for_each(std::begin(outChordHeads),
                          std::end(outChordHeads),
                          loopBody);
        } else {
            std::for_each(std::rbegin(outChordHeads),
                          std::rend(outChordHeads),
                          loopBody);
        }
    }


    void
    ChordHead::calculateAccidentalPositionsNotPtr(ChordHeads& outChordHeads)
    {
        sortHeads(outChordHeads);

        ChordHeadPtrs ptrs;

        for (ChordHead& chordHead : outChordHeads) {
            ptrs.emplace_back(&chordHead);
        }

        calculateAccidentalPositions(ptrs);
    }


    void
    ChordHead::calculateAccidentalPositions(ChordHeadPtrs& outChordHeads)
    {
        if (outChordHeads.empty()) {
            return;
        }

        // First copy the accidental-containing notes over to a new vector
        ChordHeadPtrs outHeads;

        for (auto it = outChordHeads.cbegin(); it != outChordHeads.cend(); ++it) {
            const auto chord = *it;
            const auto frame = chord->accidentalFrame;
            if (!frame.getIsEmpty() || chord->hasAccidental) {
                outHeads.push_back(*it);
            }
        }

        const auto numNotes = outHeads.size();
        if (numNotes == 0) {
            return;
        }

        ChordHeadPtrs finishedHeads;

        for (auto i = 0; i < numNotes; ++i) {
            const auto index = i / 2;
            const auto isReverse = (i == 0) || (i % 2 == 0);
            ChordHead* currentHead = nullptr;

            if (isReverse) {
                currentHead = *(outHeads.crbegin() + index);
            } else {
                currentHead = *(outHeads.cbegin() + index);
            }

            if (currentHead->accidentalFrame.getIsEmpty()) {
                continue;
            }

            std::vector<Rect> noteheadCollisionFrames;

            const auto noteheadCollisionFilter = [&](const Rect& inRect) {
                return inRect.intersects(currentHead->accidentalFrame);
            };

            std::copy_if(std::begin(currentHead->noteheadFrames), 
                         std::end(currentHead->noteheadFrames),
                         std::back_inserter(noteheadCollisionFrames),
                         noteheadCollisionFilter);

            if (!noteheadCollisionFrames.empty()) {
                // Sort the collision fames to find the leftmost
                const auto sortRectCompare = [&](Rect& inLeft, Rect& inRight) {
                    return inLeft.left < inRight.left;
                };

                const auto& leftestNotehead = noteheadCollisionFrames.front();
                double noteheadAvoidanceAmount = leftestNotehead.left - currentHead->accidentalFrame.right;
                // One more pixel to ensure whitespace
                noteheadAvoidanceAmount -= 1.0;
                moveAccidental(noteheadAvoidanceAmount, *currentHead);

                std::ranges::sort(noteheadCollisionFrames, sortRectCompare);
            }

            // Search previously encountered frames to see if mine will overlap with any of them
            auto didMoveLeft = [&]() {
                ChordHeadPtrs collisionFrames;

                const auto collisionFilter = [&](const ChordHeadPtr& inFilterPtr) {
                    return inFilterPtr->accidentalFrame.intersects(currentHead->accidentalFrame);
                };

                std::copy_if(std::begin(finishedHeads), 
                             std::end(finishedHeads),
                             std::back_inserter(collisionFrames),
                             collisionFilter);

                // Sort the collision heads to find the leftmost
                const auto sortCompare = [&](ChordHeadPtr& inLeft, ChordHeadPtr& inRight) {
                    return inLeft->accidentalFrame.left < inRight->accidentalFrame.left;
                };

                std::ranges::sort(collisionFrames, sortCompare);

                if (!collisionFrames.empty()) {
                    const auto& leftestAccidental = collisionFrames.front()->accidentalFrame;
                    double amount = leftestAccidental.left - currentHead->accidentalFrame.right;
                    // One more pixel to ensure whitespace
                    amount -= 1.0;
                    moveAccidental(amount, *currentHead);
                    return true;
                }
                return false;
            };

            while (didMoveLeft()) {}

            finishedHeads.emplace_back(currentHead);
            if (index >= (numNotes / 2)) {
                return;
            }
        }
    }


    void
    ChordHead::sortHeads(ChordHeads& outChordHeads)
    {
        auto compare = [&](ChordHead& left, ChordHead& right) {
            return left.slot < right.slot;
        };
        std::ranges::sort(outChordHeads, compare);
    }

    StemDirection
    ChordHead::calculateStemDirection(const ChordHeadPtrs& inChordHeads)
    {
        if (inChordHeads.size() == 0) {
            return StemDirection::UP;
        }

        double highest = inChordHeads.back()->slot;
        double lowest = inChordHeads.front()->slot;
        double split = (highest + lowest) / 2.0;

        auto result = StemDirection::UP;

        if (split < (6.0 - 0.0000001)) {
            result = StemDirection::UP;
        } else if (split > (6.0 + 0.0000001)) {
            result = StemDirection::DOWN;
        } else {
            int sum = 0;
            for (const auto& h : inChordHeads) {
                sum += h->slot;
            }
            auto avg = static_cast<double>(sum)/static_cast<double>(inChordHeads.size());
            if (avg <= 6.0) {
                result = StemDirection::UP;
            } else {
                result = StemDirection::DOWN;
            }
        }
        return result;
    }

    ChordSide
    ChordHead::oppositeSide(const ChordSide& inSide) {
        if (inSide == ChordSide::Right) {
            return ChordSide::Left;
        }
        return ChordSide::Right;
    }

    void
    ChordHead::moveAccidental(double inAmount, ChordHead& outChordHead)
    {
        outChordHead.accidentalFrame.left += inAmount;
        outChordHead.accidentalFrame.right += inAmount;
        outChordHead.accidentalOffset += inAmount;
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
