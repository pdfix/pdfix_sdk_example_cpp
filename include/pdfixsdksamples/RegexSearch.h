#pragma once

#include <string>

// Finds all occurences of the regex_pattern at the first page.
void RegexSearch(
    const std::wstring& open_path,                 // source PDF document
    const std::wstring& regex_pattern              // regex pattern you want to search
    );
