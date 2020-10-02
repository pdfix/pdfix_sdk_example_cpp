#pragma once

#include <string>

void AddTags(
    const std::wstring& open_path,        // source PDF document
    const std::wstring& save_path,        // output PDF document
    const std::wstring& config_path,      // configuration file
    const bool preflight                  // preflight document template before processing
    );
