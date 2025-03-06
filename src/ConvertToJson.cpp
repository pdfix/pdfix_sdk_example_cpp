////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHJson.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertToJson.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace ConvertToJson {

void Run(const std::wstring& open_path,    // source PDF document
         const std::wstring& password,     // open password
         std::ostream& output,             // output JSON file
         const std::wstring& config_path,  // configuration file
         PdfJsonParams& json_params,       // conversion parameters
         const bool preflight              // preflight document template before processing
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
  if (!doc)
    throw PdfixException();

  auto json_conv = doc->CreateJsonConversion();
  if (!json_conv)
    throw PdfixException();

  // initializ edocument template
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

  if (!json_conv->SetParams(&json_params))
    throw PdfixException();

  auto memStm = pdfix->CreateMemStream();
  if (!json_conv->SaveToStream(memStm))
    throw PdfixException();
  json_conv->Destroy();

  auto sz = memStm->GetSize();
  uint8_t data[4096];
  size_t remaining = sz;
  size_t offset = 0;

  while (remaining > 0) {
    size_t chunkSize = (remaining > sizeof(data)) ? sizeof(data) : remaining;
    if (!memStm->Read(offset, data, chunkSize))
      throw PdfixException();

    output.write(reinterpret_cast<const char*>(data), chunkSize);
    offset += chunkSize;
    remaining -= chunkSize;
  }

  doc->Close();
}
}  // namespace ConvertToJson
