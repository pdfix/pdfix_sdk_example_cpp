#pragma once

#include <string>

namespace ImportFormData {
void Run(
    const std::wstring& open_path,                 // source PDF document
    const std::wstring& password,                  // open password
    const std::wstring& save_path,                 // output PDF document
    const std::wstring& json_path,                 // json file to import
    bool flatten                                   // flatten annotations
    );
}
