#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

void RenderPages(
    const std::wstring& open_path,              // source PDF document
    const std::wstring& img_path,               // output image
    PdfImageParams img_params,                  // output image params
    int page_from,                              // page from
    int page_to,                                // page to
    double zoom,                                // page zoom
    PdfRotate rotate,                           // page rotation
    PdfDevRect clip_rect,                       // clip region
    size_t thread_count                         // max number of threads
    );
