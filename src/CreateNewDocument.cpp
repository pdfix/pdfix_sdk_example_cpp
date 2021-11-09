////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateNewDocument.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CreateNewDocument.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

  // Creates new document
void CreateNewDocument(
  const std::wstring& save_file                 // directory where to save PDF docuemnt
) {
  auto pdfix = PdfixEngine::Get();

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
}