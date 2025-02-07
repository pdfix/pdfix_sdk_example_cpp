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

  if (!json_conv->SetParams(&json_params))
    throw PdfixException();

  auto memStm = pdfix->CreateMemStream();
  if (!json_conv->SaveToStream(memStm))
    throw PdfixException();
  json_conv->Destroy();

  auto sz = memStm->GetSize();
  std::vector<uint8_t> data;
  data.resize(sz);
  if (!memStm->Read(0, data.data(), sz))
    throw PdfixException();

  output << data.data();

  doc->Close();
}
}  // namespace ConvertToJson
