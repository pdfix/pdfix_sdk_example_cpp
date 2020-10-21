#pragma once

#include <string>
#include <optional>

void MakeAccessible(
    const std::wstring& open_path,           // source PDF document
    const std::wstring& save_path,           // output PDF/UA document
    std::pair<bool, std::wstring> language,  // document reading language
    std::pair<bool, std::wstring> title,     // document title
    const std::wstring& config_path,         // configuration file
    const bool preflight                     // preflight document template before processing
    );
