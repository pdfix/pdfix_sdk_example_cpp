////////////////////////////////////////////////////////////////////////////////////////////////////
// MovePage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/MovePage.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Move page in document.
void MovePage(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // file where to save PDF docuemnt
  int to,
  int from
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

  if(!doc->MovePage(to,from))
    throw PdfixException();
  if(!doc->Save(save_file.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
  pdfix->Destroy();
}