#pragma once

#include <string>
#include <iostream>

#include "Pdfix.h"

using namespace PDFixSDK;

namespace ExtractText {
  void GetPageText(PdfPage* page, std::stringstream &ss);
  void Run(
      const std::wstring& open_path,      // source PDF document
      std::ostream& output,                // output stream
      const std::wstring& config_path,     // configuration file
      const int page_number
      );
}
