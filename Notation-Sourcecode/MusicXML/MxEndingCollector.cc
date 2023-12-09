// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// self
#include "MxEndingCollector.h"

// Local
#include "Constants.h"
#include "Ending.h"
#include "Forward.h"
#include "Measure.h"
#include "MxConverter.h"
#include "Score.h"
#include "Throw.h"

namespace komp
{
    MxEndingCollector::MxEndingCollector()
    {
    }

    void
    MxEndingCollector::addEnding(Measure& inMeasure,
                                 Track& inTrack,
                                 mx::api::EndingType inType,
                                 int inNumber)
    {
        mEndingSpecs.emplace_back(MxEndingSpec(inMeasure,
                                               inTrack,
                                               inType,
                                               inNumber));
    }

    void
    MxEndingCollector::insertEndings(ItemRegistry& inRegistry) const
    {
        // Parse the array of ending elements into grouped elements
        EndingParams params;
        
        komp::Measure* measure = nullptr;
        auto previousEnding = -1;
        
        for (const auto& spec : mEndingSpecs) {
            
            const auto currentEnding = spec.number;
            if (previousEnding == -1) {
                
                params.measure = &spec.measure;
                params.track = &spec.track;
                komp::EndingElement element;
                element.measure = &spec.measure;
                element.type = komp::EndingType::BEGIN;
                element.status = komp::EndingStatus::DEFAULT;
                element.ending = currentEnding;
                params.elements.emplace_back(element);
                
                previousEnding = currentEnding;
                
            } else {
            
                if (currentEnding < previousEnding) {
                                    
                    // Update the params for the new ending
                    params.measure = measure;
                    params.elements.clear();
                    
                    // Update the tracking values
                    measure = &spec.measure;
                    previousEnding = currentEnding;
                    
                    // Add the starting element
                    komp::EndingElement element;
                    element.measure = &spec.measure;
                    element.type = komp::EndingType::BEGIN;
                    element.status = komp::EndingStatus::DEFAULT;
                    params.elements.emplace_back(element);

                } else {
                    
                    // Add the next element.  It must not be another start.
                    if (spec.type == mx::api::EndingType::none
                     || spec.type == mx::api::EndingType::start) {
                        LOG_F(WARNING, "Malformed ending");
                    } else {
                        komp::EndingElement element;
                        element.measure = &spec.measure;
                        element.ending = currentEnding;
                        element.type = komp::EndingType::END;
                        if (spec.type == mx::api::EndingType::discontinue) {
                            element.status = komp::EndingStatus::HIDDEN;
                        } else {
                            element.status = komp::EndingStatus::DEFAULT;
                        }
                        params.elements.emplace_back(element);
                        
                        insertEnding(inRegistry, params);

                        previousEnding = -1;
                        params.elements.clear();
                    }
                }
            }
        }
    }
    
    void
    MxEndingCollector::insertEnding(ItemRegistry& inRegistry,
                                    const komp::EndingParams& inParams) const
    {
        if (!inParams.measure) {
            LOG_F(ERROR,  "measure is null" );
            return;
        }

        const MusicLocation loc(*inParams.measure, *inParams.track);
        auto& ending = inRegistry.createItem<Ending>(loc, inParams);
        const auto& measure = inParams.measure;
        measure->insertItem(ending);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
