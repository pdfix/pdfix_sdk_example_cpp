#pragma once

#include <string>

// Move page in document.
void MovePage(
    const std::wstring& open_file,                // source PDF document
    const std::wstring& save_file,                // file where to save PDF docuemnt
    int to,
    int from
    );
