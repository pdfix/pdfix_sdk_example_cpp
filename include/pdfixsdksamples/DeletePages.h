#pragma once

#include <string>

// Delete pages from document.
void DeletePages(
    const std::wstring& open_file,                // source PDF document
    const std::wstring& save_file,                // file where to save PDF docuemnt
    int from,
    int to
    );
