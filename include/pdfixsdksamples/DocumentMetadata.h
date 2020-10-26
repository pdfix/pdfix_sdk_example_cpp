#pragma once

#include <string>

namespace DocumentMetadata {
void Run(
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& save_path,                       // output PDF doucment
    const std::wstring& xml_path                         // metadata file path
    );
}
