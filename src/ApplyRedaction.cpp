////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplyRedaction.hpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ApplyRedaction.h"

// #define GENERATE_REDAC_APP

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void ApplyRedaction(
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path                // output PDF doucment
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

#ifdef GENERATE_REDAC_APP
  for (int i = 0; i < doc->GetNumPages(); i++) {
    auto page = doc->AcquirePage(i);
    for (int j = 0; j < page->GetNumAnnots(); j++) {
      auto annot = page->GetAnnot(j);
      if (annot->GetSubtype() == kAnnotRedact) {
        annot->NotifyDidChange(L"", 0);
      }
    }
  }
#endif

  if (!doc->ApplyRedaction(nullptr, nullptr))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}