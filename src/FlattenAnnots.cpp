////////////////////////////////////////////////////////////////////////////////////////////////////
// FlattenAnnots.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/FlattenAnnots.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void FlattenAnnots(const std::wstring& open_path,  // source PDF document
                   const std::wstring& save_path   // output PDF doucment
) {
  auto pdfix = PdfixEngine::Get();

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

      // flatten all but link annotations which do not have appearance
      if (annot && annot->GetSubtype() != kAnnotLink) {
        page->FlattenAnnot(annot);
      }
    }
    page->Release();
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
