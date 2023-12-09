// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include <map>

namespace komp
{

    // in order to map between measure indexes in MusicXML and Komp
    // we need a list of multimeasure rests and their size.  For each
    // multi-measure rest we encounter in the MusicXML we need to
    // record the MusicXML measure index where the rest begain, and
    // the number of measures the rest contains.
    // for example, if a file contains 6 measures, the first 4 are in
    // a 4-measure rest and the last 2 are in a 2-measure rest, then
    // we should pass this information like so
    //
    // std::map<int, int> multiMeausreRestInfo =
    // {
    //     { 0, 4 },
    //     { 4, 2 )
    // };
    
    struct MxLocationParameters
    {
        int mxTicksPerQuarter;
        int kompTicksPerQuarter;
        int totalMxMeasures;
        std::map<int, int> multiMeasureRests;

        MxLocationParameters()
        : mxTicksPerQuarter{1}
        , kompTicksPerQuarter{1}
        , totalMxMeasures{1}
        , multiMeasureRests{}
        {
            
        }
    };
    
    class MxLocation
    {
    public:
        MxLocation(MxLocationParameters&& inParams);
        
        int partIndex;
        int handIndex;
        int mxMeasureIndex;
        int voiceIndex;
        int mxTickPosition;
        
        int getMxTicksPerQuarter() const;
        int getKompTicksPerQuarter() const;
        int getKompTickPosition() const;

        /// komp ticks per quarter divided by mx ticks per quarter.
        /// to convert from komp to mx, divide by this number
        /// to convert from mx to komp, multiply by this number
        long double getTickConversionRatio() const;
        
        /// for the current mxMeasureIndex, returns
        /// the corresponding Komp measure index.
        /// this becomes relevant when multi-measure
        /// rests occur in the data because MusicXML
        /// counts the hidden measures as 'real'
        /// measures, but these hidden measures do
        /// not exist in the Komp Score model.
        int getKompMeasureIndex() const;
        int getTotalMxMeasures() const;
        int getTotalKompMeasures() const;
        
        // the raw info about multimeasure rests.  The int pairs
        // represent <mxMeasureIndex, numMeasuresInMultiMeasureRest>
        const std::map<int, int>& multiMeasureRestInfo();
        
    private:
        MxLocationParameters mParams;
        std::map<int, int> mMeasureIndexMap;
        
    private:
        void createMeasureIndexMap();
    };
    
    bool operator==(const MxLocation& lhs, const MxLocation& rhs);
    bool operator!=(const MxLocation& lhs, const MxLocation& rhs);
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
