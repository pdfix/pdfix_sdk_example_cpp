#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void OcrWithTesseract(
    const std::wstring& open_path,                  // source PDF document
    const std::wstring& save_path,                  // searchable PDF document
    const std::wstring& data_path,                  // path to OCR data
    const std::wstring& language,                   // default OCR language
    const float zoom,                               // page zoom level for rendering to control image processing quality
    const PdfRotate rotate                          // page rotation
    );
