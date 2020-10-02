#pragma once

#include <string>
#include "Pdfix.h"
#include "TaggedPdf.h"

using namespace PDFixSDK;

void ConvertTaggedPdf(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& save_path,      // output HTML file
    const std::wstring& config_path,    // configuration file
    PdfTaggedParams& params             // conversion parameters
    );
