////////////////////////////////////////////////////////////////////////////////////////////////////
// DocumentMetadata.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/DocumentMetadata.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace DocumentMetadata {
  void Run(
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& save_path,                       // output PDF doucment
    const std::wstring& xml_path                         // metadata file path
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();

    // save document metadata into a file
    PsMetadata* metadata = doc->GetMetadata();
    if (!metadata)
      throw PdfixException();

    // save metadata to file
    PsFileStream* stream = pdfix->CreateFileStream(xml_path.c_str(), kPsTruncate);
    if (!metadata->SaveToStream(stream))
      throw PdfixException();
    stream->Destroy();

    // modify document title
    std::wstring title = doc->GetInfo(L"Title");
    if (!doc->SetInfo(L"Title", L"My next presentation"))
      throw PdfixException();

    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();

    doc->Close();
    pdfix->Destroy();
  }
}