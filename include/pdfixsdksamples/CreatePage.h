#pragma once

#include <string>

// Creates new document
void CreatePage(
    const std::wstring& open_file,                // source PDF document
    const std::wstring& save_file,                // directory where to save PDF docuemnt
    int afterPageNumber                           // index of page after page is created
);
