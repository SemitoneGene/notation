// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>
#include <vector>

namespace komp
{
class Score;

class ScoreManager
{
public:
    static ScoreManager& getInstance();
    
    void add(Score* inScore);
    void remove(Score* inScore);
    
    const std::vector<Score*>& getScores() const;

private:
    ScoreManager() noexcept;
    
    std::vector<Score*> _scores;
};

}


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
