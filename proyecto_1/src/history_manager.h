#pragma once
#include <vector>
#include <memory>
#include "editor_memento.h"

class HistoryManager {
    
    std::vector<std::shared_ptr<EditorMemento>> undoStack;
    std::vector<std::shared_ptr<EditorMemento>> redoStack;

public:
    // Pushes a new state onto the undo stack and clears the redo timeline
    void save(std::shared_ptr<EditorMemento> memento) {
        if (!memento) return;
        undoStack.push_back(std::move(memento));
        redoStack.clear();
    }

    // Takes the current state (to save in redo) and returns the previous state
    std::shared_ptr<EditorMemento> undo(std::shared_ptr<EditorMemento> currentState) {
        if (undoStack.empty()) {
            return nullptr; // Nothing to undo
        }

        // Save current state to Redo stack
        if (currentState) {
            redoStack.push_back(std::move(currentState));
        }

        // Pop and return the last state from Undo stack
        auto previousState = undoStack.back();
        undoStack.pop_back();
        return previousState;
    }

    // Takes the current state (to save in undo) and returns the next state
    std::shared_ptr<EditorMemento> redo(std::shared_ptr<EditorMemento> currentState) {
        if (redoStack.empty()) {
            return nullptr; // Nothing to redo
        }

        // Save current state to Undo stack
        if (currentState) {
            undoStack.push_back(std::move(currentState));
        }

        // Pop and return the next state from Redo stack
        auto nextState = redoStack.back();
        redoStack.pop_back();
        return nextState;
    }

    void clear() {
        undoStack.clear();
        redoStack.clear();
    }
};