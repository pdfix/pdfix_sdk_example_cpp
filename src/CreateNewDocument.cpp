////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateNewDocument.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CreateNewDocument.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Creates new document
void CreateNewDocument(
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

  PdfDoc* doc = pdfix->CreateDoc();
  if (!doc)
    throw PdfixException();

  PdfRect *mBox = new PdfRect();
  mBox->left = 0;
  mBox->right = 595;
  mBox->bottom = 0;
  mBox->top = 842;
  auto page = doc->CreatePage(-1, mBox);
  page->Release();
  if (!doc->Save(save_file.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
  pdfix->Destroy();
}