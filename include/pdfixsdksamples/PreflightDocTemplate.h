#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace PreflightDocTemplate {
    void Run(
        const std::wstring& open_path,        // source PDF document
        std::ostream &output,                 // output stream for generated config
        PsDataFormat format                   // output format
        );
}
