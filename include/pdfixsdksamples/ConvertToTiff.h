#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ConvertToHTiff {
    void Run(
        const std::wstring& open_path,      // source PDF document
        const std::wstring& password,       // open password
        const std::wstring& save_path,      // output TIFF file
        PdfTiffParams& tiff_params         // conversion parameters
    );
}