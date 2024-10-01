#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ConvertToJson {
void Run(const std::wstring& open_path,    // source PDF document
         const std::wstring& password,     // open password
         std::ostream& output,             // output JSON file
         const std::wstring& config_path,  // configuration file
         PdfJsonParams& json_params,       // conversion parameters
         const bool preflight              // preflight document template before processing
);
}
