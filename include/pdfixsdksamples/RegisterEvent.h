#pragma once

#include <string>

// DocDidOpenCallback gets title when the document is opened.
void DocDidOpenCallback(void* data);
// DocWillCallback notifies you when the event happens.
void DocWillCallback(void* data);
// Registers different kinds of events.
void RegisterEvent(
    const std::wstring& open_path                  // source PDF document
    );
