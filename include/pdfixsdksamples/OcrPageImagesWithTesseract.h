#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Pdfix.h"

using namespace PDFixSDK;

void parse_page_element(PdeElement* elem, std::vector<PdfRect>& image_bbox_arr);
void OcrPageImagesWithTesseract(
    const std::wstring& open_path,                  // source PDF document
    const std::wstring& save_path,                  // searchable PDF document
    const std::wstring& data_path,                  // path to OCR data
    const std::wstring& language,                   // default OCR language
    const float zoom,                               // zoom to control page rendering quality
    const PdfRotate rotate                          // page rotation to be applied
    );
