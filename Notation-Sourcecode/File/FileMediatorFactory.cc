// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FileMediatorFactory.h"

// Local
#include "Throw.h"
#include "MxFileMediator.h"

namespace komp
{
    FileMediatorIntfUPtr
    FileMediatorFactory::create(FileMediatorType fileMediatorType)
    {
		switch(fileMediatorType) {
            case FileMediatorType::MusicXML: return FileMediatorIntfUPtr{ new MxFileMediator{} };
            default: KOMP_THROW( "bad FileMediatorType value" );
		}
	}
    
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
