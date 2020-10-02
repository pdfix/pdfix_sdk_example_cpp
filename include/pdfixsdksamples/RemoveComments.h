#pragma once

#include <string>

// Removes from first text annot with it's popup and all replies
void RemoveComments(
    const std::wstring& open_path,                 // source PDF document
    const std::wstring& save_path                  // output PDF document
    );
