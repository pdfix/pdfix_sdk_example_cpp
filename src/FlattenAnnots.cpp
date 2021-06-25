////////////////////////////////////////////////////////////////////////////////////////////////////
// FlattenAnnots.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/FlattenAnnots.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void FlattenAnnots(
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

  for (auto i = 0; i < doc->GetNumPages(); i++) {
    auto page = doc->AcquirePage(i);
    if (!page)
      throw PdfixException();
    for (auto j = page->GetNumAnnots() - 1; j >= 0; j--) {
      auto annot = page->GetAnnot(j);
      
      // skip and remove hidden annotations
      auto flags = annot->GetFlags();
      if (flags & kAnnotFlagHidden) {
        page->RemoveAnnot(j, 0);
        continue;
      }

      // flatten all but link annotations which to not have appearance
      if (annot && annot->GetSubtype() != kAnnotLink) {
        page->FlattenAnnot(annot);
      }
    }
    page->SetContent();
    page->Release();
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}