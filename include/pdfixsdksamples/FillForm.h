#pragma once

// system
#include <string>

// Adds a new text annotation.
void FillForm(
    const std::wstring& open_file,                // source PDF document
    const std::wstring& save_file,                // directory where to save PDF docuemnt
    const std::wstring& json_file,                // json with field values
    bool flatten                                  // flatten for fields
    );
