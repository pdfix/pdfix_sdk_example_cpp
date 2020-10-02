////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveComments.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage RemoveComments_cpp
*/
/*!
\page RemoveComments_cpp Remove Comments Sample
Example how to remove from a first text annot with it's popup and all replies,
\snippet /RemoveComments.hpp RemoveComments_cpp
*/

#include "pdfixsdksamples/RemoveComments.h"

//! [RemoveComments_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Removes from first text annot with it's popup and all replies
void RemoveComments(
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF document
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
  pdfix->Destroy();
}
//! [RemoveComments_cpp]
