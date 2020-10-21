////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeAccessible.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/MakeAccessible.h"

#include <string>
#include <iostream>
#include <memory>
#include <optional>
#include "Pdfix.h"
#include "OcrTesseract.h"

using namespace PDFixSDK;

void MakeAccessible(
  const std::wstring& open_path,           // source PDF document
  const std::wstring& save_path,           // output PDF/UA document
  std::pair<bool, std::wstring> language,  // document reading language
  std::pair<bool, std::wstring> title,     // document title
  const std::wstring& config_path,         // configuration file
  const bool preflight                     // preflight document template before processing
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

  // convert to PDF/UA
  PdfAccessibleParams params;
  params.embed_fonts = 1;
  params.subset_fonts = 1;
  //params.accept_tags = 1;
 
  if (!doc->MakeAccessible(&params, nullptr, nullptr))
    throw PdfixException();

  // set document language
  if (language.first)
    doc->SetLang(language.second.c_str());

  // set documnt title
  if (title.first)
    doc->SetInfo(L"Title", title.second.c_str());

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}