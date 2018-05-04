////////////////////////////////////////////////////////////////////////////////////////////////////
// AddComment.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage AddComment_cpp
*/
/*!
\page AddComment_cpp Add Comment Sample
Example how to add a text annotation with a reply.
\snippet /AddComment.hpp AddComment_cpp
*/

#pragma once

//! [AddComment_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // Adds a new text annotation.
void AddComment(
  const std::wstring& email,                    // authorization email   
  const std::wstring& license_key,              // authorization license key
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file                 // directory where to save PDF docuemnt
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
    pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
    pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
    throw std::runtime_error("Incompatible version");

  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  PdfPage* page = doc->AcquirePage(0);
  PdfRect crop_box;
  page->GetCropBox(&crop_box);

  // place annotation to the middle of the page
  PdfRect annot_rect;
  annot_rect.left = (crop_box.right + crop_box.left) / 2. - 10;
  annot_rect.bottom = (crop_box.top + crop_box.bottom) / 2. - 10;
  annot_rect.right = (crop_box.right + crop_box.left) / 2. + 10;
  annot_rect.top = (crop_box.top + crop_box.bottom) / 2. + 10;

  PdfTextAnnot* annot = page->AddTextAnnot(-1, &annot_rect);
  annot->SetAuthor(L"Peter Brown");
  annot->SetContents(L"This is my comment.");
  annot->AddReply(L"Mark Fish", L"This is some reply.");

  doc->ReleasePage(page);
  doc->Save(save_file.c_str(), kSaveFull);
  doc->Close();

  pdfix->Destroy();
}
//! [AddComment_cpp]