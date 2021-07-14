////////////////////////////////////////////////////////////////////////////////////////////////////
// AddComment.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AddComment.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Adds a new text annotation.
void AddComment(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file                 // directory where to save PDF docuemnt
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
    pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
    pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
    throw std::runtime_error("Incompatible version");

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
  if (!doc)
    throw PdfixException();

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();
  PdfRect crop_box;
  page->GetCropBox(&crop_box);

  // place annotation to the middle of the page
  PdfRect annot_rect;
  annot_rect.left = (crop_box.right + crop_box.left) / 2. - 10;
  annot_rect.bottom = (crop_box.top + crop_box.bottom) / 2. - 10;
  annot_rect.right = (crop_box.right + crop_box.left) / 2. + 10;
  annot_rect.top = (crop_box.top + crop_box.bottom) / 2. + 10;

  PdfTextAnnot* annot = (PdfTextAnnot*)page->CreateAnnot(kAnnotText, &annot_rect);
  page->AddAnnot(-1, annot);
  if (!annot)
    throw PdfixException();
  annot->SetAuthor(L"Peter Brown");
  annot->SetContents(L"This is my comment.");
  annot->AddReply(L"Mark Fish", L"This is some reply.");

  page->Release();
  doc->Save(save_file.c_str(), kSaveFull);
  doc->Close();

  pdfix->Destroy();
}