// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "UndoManager.h"

// Local
#include "Command.h"
#include "Score.h"
#include "Selection.h"
#include "Throw.h"

namespace komp
{

template <typename T>
void log_helper(T t) {
    std::cout << t << ' ';
}

template <typename T, typename... Args>
void log_helper(T t, Args... args) {
    std::cout << t << ' ';
    log_helper(args...);
}

template <typename... Args>
void command_log(Args... args) {
    std::cout << "command: ";
    log_helper(args...);
    std::cout << std::endl;
}

// MARK: - UndoManager

UndoManager::UndoManager(Score& inScore)
: mScore{inScore}
{
}

bool
UndoManager::canUndo() const
{
    return !mUndoStack.empty();
}

bool
UndoManager::canRedo() const
{
    return !mRedoStack.empty();
}

void
UndoManager::undo()
{
    KOMP_LOG_COMMAND
    if (mUndoStack.empty()) {
        LOG_F(WARNING, "Undo called on empty stack");
        return;
    }
    
    auto command = std::move(mUndoStack.front());
    KOMP_ASSERT(command);
    //command_log("Undo", std::string(command->getName()));
    command->undo();
    
    mUndoStack.pop_front();
    mRedoStack.push_front(std::move(command));
    
    mScore.setDirty(true);
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().undone(*this);
    }
}

void
UndoManager::redo()
{
    KOMP_LOG_COMMAND
    if (mRedoStack.empty()) {
        LOG_F(WARNING, "Redo called on empty stack");
        return;
    }
    
    auto command = std::move(mRedoStack.front());
    KOMP_ASSERT(command);
    //command_log("Redo", std::string(command->getName()));
    command->redo();
    
    mRedoStack.pop_front();
    mUndoStack.push_front(std::move(command));
    
    mScore.setDirty(true);
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().redone(*this);
    }

}

const std::string
UndoManager::undoTitle() const
{
    std::string title;
    if (!mUndoStack.empty()) {
        const auto& command = mUndoStack.front();
        title = command->getName();
    }
    return title;
}

const std::string
UndoManager::redoTitle() const
{
    std::string title;
    if (!mRedoStack.empty()) {
        const auto& command = mRedoStack.front();
        title = command->getName();
    }
    return title;
}

void
UndoManager::addCommand(CommandIntfUPtr inCommand)
{
    mUndoStack.push_front(std::move(inCommand));
    
    // Clear the redo stack once a new command is explicity added
    mRedoStack.clear();
    
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        observer.get().added(*this);
    }
}

// Removes all commands that reference this item
void
UndoManager::purgeItem(const Item& inItem)
{
    auto iter = mUndoStack.begin();
    while (iter != mUndoStack.end()) {
        const auto& command = *iter;
        if (command->containsItem(inItem)) {
            iter = mUndoStack.erase(iter);
        } else {
            ++iter;
        }
    }
    
    iter = mRedoStack.begin();
    while (iter != mRedoStack.end()) {
        const auto& command = *iter;
        if (command->containsItem(inItem)) {
            iter = mRedoStack.erase(iter);
        } else {
            ++iter;
        }
    }
}

void
UndoManager::reset()
{
    mUndoStack.clear();
    mRedoStack.clear();
    mObservers.clear();
}

// MARK: - Observation

void
UndoManager::registerObserver(UndoManagerObserver& inObserver)
{
    auto predicate = [&](UndoManagerObserverRef& observer) {
        return &observer.get() == &inObserver;
    };
    
    auto iter = std::find_if(mObservers.begin(),
                             mObservers.end(),
                             predicate);
    
    if (iter == mObservers.end()) {
        mObservers.emplace_back(inObserver);
    }
}

void
UndoManager::unregisterObserver(UndoManagerObserver& inObserver)
{
    for (auto it = mObservers.cbegin(); it != mObservers.cend(); ++it) {
        auto observer = *it;
        if (&observer.get() == &inObserver) {
            it = mObservers.erase(it);
            --it;
            continue;
        }
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
