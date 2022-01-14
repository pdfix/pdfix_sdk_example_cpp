////////////////////////////////////////////////////////////////////////////////////////////////////
// PreflightDocTemplate.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/PreflightDocTemplate.h"

#include <string>
#include <cstring>
#include <iostream>
#include <memory>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace PreflightDocTemplate {

  void Run(
    const std::wstring& open_path,        // source PDF document
    std::ostream &output,                 // output stream for generated config
    PsDataFormat format                   // output format
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

    auto save_config = [&](auto flags) {
      // save JSON to stream
      auto stm = pdfix->CreateMemStream();
      if (!stm)
        throw PdfixException();
      if (!doc_template->SaveToStream(stm, format, flags))
        throw PdfixException();

      auto sz = stm->GetSize();

      struct free_delete { void operator()(void* x) { free(x); } };
      std::shared_ptr<uint8_t> buffer((uint8_t *)malloc(sizeof(uint8_t) * (sz + 1)), free_delete());
      memset(buffer.get(), 0, sz + 1);

      if (!stm->Read(0, buffer.get(), sz))
        throw PdfixException();
      output << buffer.get();
      stm->Destroy();
    };

    // save default config
    output << "Default config: " << std::endl;
    save_config(kSaveUncompressed | kSaveIncludeComments);

    // add reference pages for preflight
    for (auto i = 0; i < doc->GetNumPages(); i++) {
      if (!doc_template->AddPage(i, nullptr, nullptr))
        throw PdfixException();
    }
    // update document preflight
    if (!doc_template->Update(nullptr, nullptr))
      throw PdfixException();

    output << "Preflight config: " << std::endl;
    save_config(kSaveUncompressed);

    doc->Close();
    pdfix->Destroy();
  }
}