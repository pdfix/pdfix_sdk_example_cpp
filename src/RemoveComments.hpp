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
Example how to remove from a first highlight annot with it's popup and all replies,
\snippet /RemoveComments.hpp RemoveComments_cpp
*/

#pragma once

//! [RemoveComments_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // Removes from first highlight annot with it's popup and all replies
void RemoveComments(
  const std::wstring& email,                     // authorization email   
  const std::wstring& license_key,               // authorization license key
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());
  PdfPage* page = doc->AcquirePage(0);

  for (auto i = 0; i < page->GetNumAnnots(); i++) {
    PdfAnnot* annot = page->GetAnnot(i);
    if (annot->GetSubtype() == kAnnotHighlight) {
      page->RemoveAnnot(i, kRemoveAnnotPopup | kRemoveAnnotReply);
      break;
    }
  }
  doc->ReleasePage(page);
  doc->Save(save_path.c_str(), kSaveFull);
  doc->Close();
  pdfix->Destroy();
}
//! [RemoveComments_cpp]