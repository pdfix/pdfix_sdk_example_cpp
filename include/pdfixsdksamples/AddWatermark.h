#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

// Adds watermark from img_path and saves the document to save_path.
void AddWatermark(
    const std::wstring& open_path,                    // source PDF document
    const std::wstring& save_path,                    // path to save PDF docuemnt
    const std::wstring& img_path,                     // watermark to apply
    int start_page,                                   // start page number
    int end_page,                                     // end page number
    int order_top,                                    // number specifying where in the page z-order the watermark should be added
    bool percentage_vals,                             // boolean specifying the units of horizValue and vertValue
    PdfAlignment h_align,                             // the horizontal alignment to be used when adding the watermark to a page
    PdfAlignment v_align,                             // the vertical alignment to be used when adding the watermark to a page
    float h_value,                                    // the horizontal offset value to be used when adding the watermark on a page
    float v_value,                                    // the vertical offset value to be used when adding the watermark on a page
    float scale,                                      // the scale factor to be used when adding the watermark, with 1.0 meaning 100%
    float rotation,                                   // the counter-clockwise rotation, in degrees, to be used when adding the watermark
    float opacity                                     // the opacity to be used when adding the watermark
    );