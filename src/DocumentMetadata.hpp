////////////////////////////////////////////////////////////////////////////////////////////////////
// DocumentMetadata.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage DocumentMetadata_cpp
*/
/*!
\page DocumentMetadata_cpp Get/Set Document Metadata Sample
Example how to get or set document metadata.
\snippet /DocumentMetadata.hpp DocumentMetadata_cpp
*/

#pragma once

//! [DocumentMetadata_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

// Gets/sets document metadata.
void DocumentMetadata(
  const std::wstring& email,                           // authorization email   
  const std::wstring& license_key,                     // authorization license key
  const std::wstring& open_path,                       // source PDF document
  const std::wstring& save_path,                       // output PDF doucment
  const std::wstring& xml_path                         // metadata file path
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  std::wstring title;
  title.resize(doc->GetInfo(L"Title", nullptr, 0));
  doc->GetInfo(L"Title", (wchar_t*)title.c_str(), title.size());
  doc->SetInfo(L"Title", L"My next presentation");

  PsMetadata* metadata = doc->GetMetadata();
  if (!metadata)
    throw std::runtime_error(pdfix->GetError());

  PsFileStream* stream = pdfix->CreateFileStream(xml_path.c_str(), kPsTruncate);
  if (!metadata->SaveToStream(stream))
    throw std::runtime_error(pdfix->GetError());
  stream->Destroy();

  // do something with XML metadata ...

  stream = pdfix->CreateFileStream(xml_path.c_str(), kPsReadOnly);
  if (!metadata->LoadFromStream(stream))
    throw std::runtime_error(pdfix->GetError());
  stream->Destroy();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw std::runtime_error(pdfix->GetError());

  doc->Close();
  pdfix->Destroy();
}

//! [DocumentMetadata_cpp]