////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveComments.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RemoveComments.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

  // Removes from first text annot with it's popup and all replies
void RemoveComments(
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
  PdfPage* page = doc->AcquirePage(0);

  for (auto i = 0; i < page->GetNumAnnots(); i++) {
    PdfAnnot* annot = page->GetAnnot(i);
    if (annot->GetSubtype() == kAnnotText) {
      page->RemoveAnnot(i, kRemoveAnnotPopup | kRemoveAnnotReply);
      break;
    }
  }
  page->Release();
  doc->Save(save_path.c_str(), kSaveFull);
  doc->Close();
}