////////////////////////////////////////////////////////////////////////////////////////////////////
// CreatePage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CreatePage.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Creates new document
void CreatePage(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // directory where to save PDF docuemnt
  int afterPageNumber                           // index of page after page is created
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

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(),L"");
  if (!doc)
    throw PdfixException();

  PdfRect *mBox = new PdfRect();
  mBox->left = 0;
  mBox->bottom = 0;
  mBox->right = 595;
  mBox->top = 842;
  PdfPage* page = doc->CreatePage(afterPageNumber, mBox);
  page->Release();
  doc->Save(save_file.c_str(), kSaveFull);
  doc->Close();

  pdfix->Destroy();
}