#pragma once

#include <string>
#include "Pdfix.h"
#include "PdfToHtml.h"

using namespace PDFixSDK;

void ConvertToHtml(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& save_path,      // output HTML file
    const std::wstring& config_path,    // configuration file
    PdfHtmlParams& html_params,         // conversion parameters
    const bool preflight                // preflight document template before processing
    );
