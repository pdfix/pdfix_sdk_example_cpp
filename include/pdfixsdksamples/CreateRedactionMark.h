#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

// Creates redaction mark and saves it to the new document
void CreateRedactionMark(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // file path where to save PDF docuemnt
  int page_num,                                 // index of page where to create redaction mark
  PdfRect& redaction_rect                       // redaction mark rectangle
);
