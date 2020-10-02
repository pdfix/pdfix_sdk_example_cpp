#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

// SaveImage processes each element recursively. If the element is an image, it saves it to save_path.
void SaveImage(PdeElement* element,
               const std::wstring& save_path,
               PdfImageParams& img_params,
               PdfPage* page,
               PdfPageView* page_view,
               int& image_index);

// Extracts all images from the document and saves them to save_path.
void ExtractImages(
    const std::wstring& open_path,                // source PDF document
    const std::wstring& save_path,                // directory where to extract images
    int render_width,                             // with of the rendered page in pixels (image )
    PdfImageParams& img_params                    // image parameters
    );
