// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ScoreManager.h"

// Notation
#include "Score.h"

namespace komp
{

ScoreManager&
ScoreManager::getInstance()
{
    static ScoreManager theInstance;
    return theInstance;
}

ScoreManager::ScoreManager()noexcept
{
}

void
ScoreManager::add(Score* inScore)
{
    _scores.push_back(inScore);
}

void
ScoreManager::remove(Score* inScore)
{
    const auto iter = std::find(_scores.begin(),
                                _scores.end(),
                                inScore);
    if (iter != _scores.end()) {
        _scores.erase(iter);
    }
}

const std::vector<Score*>&
ScoreManager::getScores() const
{
    return _scores;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
