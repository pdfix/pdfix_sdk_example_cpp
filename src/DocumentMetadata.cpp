////////////////////////////////////////////////////////////////////////////////////////////////////
// DocumentMetadata.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/DocumentMetadata.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace DocumentMetadata {
  void Run(
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& save_path,                       // output PDF doucment
    const std::wstring& xml_path                         // metadata file path
  ) {
    auto pdfix = PdfixEngine::Get();

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();

    // save document metadata into a file
    auto* metadata = doc->GetMetadata();
    if (!metadata)
      throw PdfixException();

    auto size = metadata->GetSize();
    std::string buffer;
    buffer.resize(size);
    if (!metadata->Read(0, (uint8_t*)buffer.c_str(), size)) {
      throw PdfixException();
    }

    // save metadata to file
    PsFileStream* stream = pdfix->CreateFileStream(xml_path.c_str(), kPsTruncate);
    if (!stream->Write(0, (uint8_t*)buffer.c_str(), size)) {
      throw PdfixException();
    }
    stream->Destroy();

    // modify document title
    std::wstring title = doc->GetInfo(L"Title");
    if (!doc->SetInfo(L"Title", L"My next presentation"))
      throw PdfixException();

    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();

    doc->Close();
  }
}