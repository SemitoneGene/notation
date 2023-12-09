// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "CommandResult.h"

namespace komp
{
	CommandResult::CommandResult()
	: mIsSuccess{false}
    , mInsertedItems{}
    {

	}


    CommandResult::CommandResult(bool inIsSuccess)
    : mIsSuccess{inIsSuccess}
    , mInsertedItems{}
    {
        
    }


    CommandResult::CommandResult(std::vector<ItemID> inInsertedItems)
    : mIsSuccess{false}
    , mInsertedItems{std::move(inInsertedItems)}
    {
        mIsSuccess = mInsertedItems.size() > 0;
    }


    CommandResult::operator bool() const
    {
        return getIsSuccess();
    }


    bool
    CommandResult::operator!() const
    {
        return !getIsSuccess();
    }


	bool
	CommandResult::getIsSuccess() const
	{
        return mIsSuccess;
	}


    const std::vector<ItemID>&
    CommandResult::getInsertedItems() const
    {
        return mInsertedItems;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
