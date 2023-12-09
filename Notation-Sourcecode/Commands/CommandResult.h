// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include "Forward.h"

namespace komp
{
    /// \brief  Encapsulate results from a command->execute() call.
    ///
    /// \discussion Can implicitly convert itself to a bool so that you
    /// can easily check if the call to Command->execute() was successful,
    /// but also carries additional information.
    ///
	class CommandResult
	{
	public:
		CommandResult();
        CommandResult(bool inIsSuccess);
        CommandResult(std::vector<ItemID> inInsertedItems);
        operator bool() const;
        bool operator !() const;

    public:
    	bool getIsSuccess() const;

        /// caution, many commands may not be implementing this. if you need
        /// to receive the inserted item(s), you may need to go to the specific
        /// command that you are using and implement it such that it returns
        /// the inserted itemID's
        const std::vector<ItemID>& getInsertedItems() const;
        	
    private:
    	bool mIsSuccess;
        std::vector<ItemID> mInsertedItems;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
