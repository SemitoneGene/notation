// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace kern
{
    inline std::vector<std::string> getBigrams( std::string_view inString )
    {
        std::vector<std::string> bigrams;
        auto char1 = inString.cbegin();
        auto char2 = char1 + 1;
        const auto e = inString.cend();
        for( ; char1 != e && char2 != e; ++char1, ++char2 )
        {
            std::string bigram;
            std::copy( char1, char2 + 1, std::back_inserter(bigram) );
            bigrams.emplace_back( std::move( bigram ) );
        }

        return bigrams;
    }


    inline double getDistance( std::string_view inLeft, std::string_view inRight, bool isCaseSensitive = false )
    {
        auto left = std::string(inLeft);
        auto right = std::string(inRight);

        if( !isCaseSensitive )
        {
            std::transform(left.begin(), left.end(), left.begin(), ::toupper);
            std::transform(right.begin(), right.end(), right.begin(), ::toupper);
        }

        // remove anything that is not a letter
        std::stringstream lstr;

        for (const auto& c : left) {
            if(std::isalpha(c)) {
                lstr << c;
            }
        }

        std::stringstream rstr;

        for (const auto& c : right) {
            if(std::isalpha(c)) {
                rstr << c;
            }
        }

        auto lbigrams = getBigrams( left );
        auto rbigrams = getBigrams( right );
        std::sort( std::begin(lbigrams), std::end(lbigrams) );
        std::sort( std::begin(rbigrams), std::end(rbigrams) );

        std::vector<std::string> intersection;
        std::set_intersection( std::begin( lbigrams ), std::end( lbigrams ), std::begin( rbigrams ), std::end( rbigrams ), std::inserter(intersection, intersection.begin() ) );

        double numerator = static_cast<double>( intersection.size() ) * 2.0;
        double denominator = static_cast<double>( lbigrams.size() ) + static_cast<double>( rbigrams.size() );

        if( denominator == 0.0 )
        {
            return 0;
        }
        
        const double closeness = numerator / denominator;
        const double distance = 1 - closeness;
        return distance;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
