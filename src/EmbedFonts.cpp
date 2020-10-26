////////////////////////////////////////////////////////////////////////////////////////////////////
// DocumentMetadata.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/EmbedFonts.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace EmbedFonts {
  void Run(
    const std::wstring& open_path,             // source PDF document
    const std::wstring& save_path              // output PDF doucment
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();

    if (!doc->EmbedFonts(true, nullptr, nullptr))
      throw PdfixException();

    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();

    doc->Close();
    pdfix->Destroy();
  }
}