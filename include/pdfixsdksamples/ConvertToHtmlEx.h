#pragma once

#include <string>
#include "Pdfix.h"
#include "PdfToHtml.h"

using namespace PDFixSDK;

void ConvertToHtmlEx(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& save_path,      // output HTML file
    const std::wstring& config_path,    // configuration
    PdfHtmlParams& html_params,         // conversion parameters
    const std::wstring& param1,         // param 1
    const std::wstring& param2          // param 2
    );
