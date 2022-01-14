////////////////////////////////////////////////////////////////////////////////////////////////////
// AddTags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AddTags.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void AddTags(
  const std::wstring& open_path,        // source PDF document
  const std::wstring& save_path,        // output PDF document
  const std::wstring& config_path,      // configuration file
  const bool preflight                  // preflight document template before processing
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

  auto doc_template = doc->GetTemplate();
  if (!doc_template)
    throw PdfixException();

  if (!config_path.empty()) {
    PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
    if (stm) {
      if (!doc_template->LoadFromStream(stm, kDataFormatJson))
        throw PdfixException();
      stm->Destroy();
    }
  }

  if (preflight) {
    // add reference pages for preflight
    for (auto i = 0; i < doc->GetNumPages(); i++) {
      if (!doc_template->AddPage(i, nullptr, nullptr))
        throw PdfixException();
    }
      
    // run document preflight
    if (!doc_template->Update(nullptr, nullptr))
      throw PdfixException();
  }

  // remove old marked content
  if (!doc->RemoveTags(nullptr, nullptr))
    throw PdfixException();

  // add tags to the document
  if (!doc->AddTags(nullptr, nullptr))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull | kSaveCompressedStructureOnly))
    throw PdfixException();
  doc->Close();
  pdfix->Destroy();
}