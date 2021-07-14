#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace RenderPage {

    void Run(
        const std::wstring& open_path,              // source PDF document
        const std::wstring& password,               // open password
        const std::wstring& img_path,               // output image
        PdfImageParams img_params,                  // output image params
        int page_num,                               // page number
        float zoom,                                 // page zoom
        PdfRotate rotate,                           // page rotation
        PdfDevRect clip_rect                        // clip region
        );
}