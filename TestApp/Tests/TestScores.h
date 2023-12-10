// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>

//Komp
#include "Forward.h"

namespace komp
{
    using ScoreFunctionPtr = komp::ScoreUPtrVec(*)();
    
    class TestScores
    {
    public:
        static komp::ScoreUPtrVec create(const std::string& name);
        
    private:
        using ScoreFunctionMap = std::map<const std::string, ScoreFunctionPtr>;
        std::map<const std::string, ScoreFunctionPtr> mMap;

    private:
        TestScores();
        static const TestScores& getInstance();
        static const ScoreFunctionMap& getMap();
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
