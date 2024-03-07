////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplyRedaction.hpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ApplyRedaction.h"

// #define GENERATE_REDAC_APP

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void ApplyRedaction(const std::wstring& open_path,  // source PDF document
                    const std::wstring& save_path   // output PDF doucment
) {
  auto pdfix = PdfixEngine::Get();

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

  if (!doc->ApplyRedaction())
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
