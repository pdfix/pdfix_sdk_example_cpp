#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

// Adds watermark from img_path and saves the document to save_path.
void AddWatermark(
    const std::wstring& open_path,                    // source PDF document
    const std::wstring& save_path,                    // path to save PDF docuemnt
    const std::wstring& img_path,                     // watermark to apply
    PdfWatermarkParams& watermark_params              // watermark parameters
    );
