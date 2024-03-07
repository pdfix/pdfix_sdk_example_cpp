////////////////////////////////////////////////////////////////////////////////////////////////////
// AddTags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AddTags.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void AddTags(const std::wstring& open_path,    // source PDF document
             const std::wstring& save_path,    // output PDF document
             const std::wstring& config_path,  // configuration file
             const bool preflight              // preflight document template before processing
) {
  auto pdfix = PdfixEngine::Get();

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
      if (!doc_template->AddPage(i))
        throw PdfixException();
    }

    // run document preflight
    if (!doc_template->Update())
      throw PdfixException();
  }

  // remove old marked content
  if (!doc->RemoveTags())
    throw PdfixException();

  // add tags to the document
  PdfTagsParams params;
  if (!doc->AddTags(&params))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull | kSaveCompressedStructureOnly))
    throw PdfixException();
  doc->Close();
}
