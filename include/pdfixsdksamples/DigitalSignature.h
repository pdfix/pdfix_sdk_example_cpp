#pragma once

#include <string>

void DigitalSignature(
    const std::wstring& open_path,               // source PDF document
    const std::wstring& save_path,               // signed PDF document
    const std::wstring& pfx_path,                // pfx file
    const std::wstring& pfx_password             // pfx password
    );
