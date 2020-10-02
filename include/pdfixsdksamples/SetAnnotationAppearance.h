#pragma once

#include <string>

namespace SetAnnotationAppearance {

// Adds watermark from img_path and saves the document to save_path.
void Run(
    const std::wstring& open_path,                    // source PDF document
    const std::wstring& save_path,                    // path to save PDF docuemnt
    const std::wstring& img_path                      // image to apply
    );
  }
