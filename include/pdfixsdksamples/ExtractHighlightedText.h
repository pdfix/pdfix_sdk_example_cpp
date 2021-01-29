#pragma once

#include <string>
#include <sstream>
#include "Pdfix.h"

using namespace PDFixSDK;

// Extracts texts from the document and saves them to text format.
namespace ExtractHighlightedText {
    void Run(
        const std::wstring& open_path,      // source PDF document
        std::ostream& output,               // output stream
        const std::wstring& config_path     // configuration file
        );
}
