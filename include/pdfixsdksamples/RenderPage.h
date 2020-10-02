#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

void RenderPage(
    const std::wstring& open_path,              // source PDF document
    const std::wstring& img_path,               // output image
    PdfImageParams img_params,                  // output image params
    int page_num,                               // page number
    double zoom,                                // page zoom
    PdfRotate rotate,                           // page rotation
    PdfDevRect clip_rect                        // clip region
    );
