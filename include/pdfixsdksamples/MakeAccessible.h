#pragma once

#include <string>
#include <optional>

void MakeAccessible(
    const std::wstring& open_path,           // source PDF document
    const std::wstring& save_path,           // output PDF/UA document
    const std::wstring& command_path         // configuration file
    );
