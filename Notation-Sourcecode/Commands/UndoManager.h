// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <stack>

// Local
#include "Forward.h"


// Forward declarations
namespace komp
{
class CommandIntf;
class Measure;
class Score;
class UndoManager;
}

namespace komp
{

class UndoManagerObserver
{
public:
    virtual ~UndoManagerObserver() = default;
    
    virtual void added(const UndoManager& inManager) = 0;
    virtual void undone(const UndoManager& inManager) = 0;
    virtual void redone(const UndoManager& inManager) = 0;
};


class UndoManager
{
public:
    UndoManager(Score& inScore);
    ~UndoManager() = default;
    
    bool canUndo() const;
    bool canRedo() const;
    
    void addCommand(std::unique_ptr<CommandIntf> inCommand);
    
    void undo();
    void redo();
    
    void purgeItem(const Item& inItem);
    
    const std::string undoTitle() const;
    const std::string redoTitle() const;
    
    void reset();
    
    void registerObserver(UndoManagerObserver& inObserver);
    void unregisterObserver(UndoManagerObserver& inObserver);

private:
    UndoManager() = delete;
    
    std::deque<std::unique_ptr<CommandIntf>> mUndoStack;
    std::deque<std::unique_ptr<CommandIntf>> mRedoStack;
    
    UndoManagerObserverRefVec mObservers;
    
    Score& mScore;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
