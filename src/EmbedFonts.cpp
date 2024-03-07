////////////////////////////////////////////////////////////////////////////////////////////////////
// DocumentMetadata.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/EmbedFonts.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace EmbedFonts {
void Run(const std::wstring& open_path,  // source PDF document
         const std::wstring& save_path   // output PDF doucment
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  if (!doc->EmbedFonts(true))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
}  // namespace EmbedFonts
