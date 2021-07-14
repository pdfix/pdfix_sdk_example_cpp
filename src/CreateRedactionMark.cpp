////////////////////////////////////////////////////////////////////////////////////////////////////
// CreatePage.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage CreatePage_cpp
*/
/*!
\page CreteRedactionMark_cpp Crete Redaction Mark Sample
Example how to create redaction mark in a document.
\snippet /CreteRedactionMark.hpp CreteRedactionMark_cpp
*/

#include "pdfixsdksamples/CreatePage.h"

//! [CreatePage_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;


static void CreteRedactionMark(PdfPage* page, PdfRect& redaction_rect) {
  // Create empty redact annotation and add it to the page
  auto redact_annot = page->CreateAnnot(PdfAnnotSubtype::kAnnotRedact, &redaction_rect);
  page->AddAnnot(-1, redact_annot);

  // Notify before editing
  redact_annot->NotifyWillChange(L"IC");

  auto redact_dict = redact_annot->GetObject();
  
  // Normal appearance

  // Outline color (red)
  auto outline_color = redact_dict->PutArray(L"OC");
  outline_color->PutNumber(0, 1.0);
  outline_color->PutNumber(1, 0.0);
  outline_color->PutNumber(2, 0.0);

  // Redaction overlay appearance

  // Inner color (black)
  auto inner_color = redact_dict->PutArray(L"IC");
  inner_color->PutNumber(0, 0.0);
  inner_color->PutNumber(1, 0.0);
  inner_color->PutNumber(2, 0.0);

  // Font style
  redact_dict->PutString(L"DA", L"1 1 0 RG 1 1 0 rg 0 Tc 0 Tw 100 Tz 0 TL 0 Ts 0 Tr /Helv 10 Tf");
  redact_dict->PutString(L"OverlayText", L"THIS WAS REDACTED ");
  redact_dict->PutNumber(L"Q", 0);

  // Notify after editing - this will regenerate redaction appearance from given settings
  redact_annot->NotifyDidChange(L"IC", 0);
}

// Creates redaction mark and saves it to the new document
void CreateRedactionMark(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // file path where to save PDF docuemnt
  int page_num,                                 // index of page where to create redaction mark
  PdfRect& redaction_rect                       // redaction mark rectangle
){
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

  auto page = doc->AcquirePage(page_num);
 
  CreteRedactionMark(page, redaction_rect);

  page->Release();
  doc->Save(save_file.c_str(), kSaveFull);
  doc->Close();

  pdfix->Destroy();
}
//! [CreatePage_cpp]
