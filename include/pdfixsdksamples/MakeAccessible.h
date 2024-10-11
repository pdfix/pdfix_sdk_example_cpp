#pragma once

#include <optional>
#include <string>

void MakeAccessible(
    const std::wstring& open_path,     // source PDF document
    const std::wstring& save_path,     // output PDF/UA document
    const std::wstring& command_path,  // configuration file
    const std::wstring& language =
        L"",  // document language to set and overide the command configuration
    const std::wstring& title = L""  // document title to set and overide the command configuration
);
