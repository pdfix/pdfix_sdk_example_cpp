////////////////////////////////////////////////////////////////////////////////////////////////////
// AddTags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage AddTags_cpp
*/
/*!
\page AddTags_cpp Add Tags Sample
Example how to automatically add tags into the document.
\snippet /AddTags.hpp AddTags_cpp
*/

#pragma once

//! [AddTags_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

void AddTags(
  const std::wstring& email,            // authorization email
  const std::wstring& license_key,      // authorization license key
  const std::wstring& open_path,        // source PDF document
  const std::wstring& save_path,        // output PDF document
  const std::wstring& config_path       // configuration file
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  // customize auto-tagging 
  if (!config_path.empty()) {
    PdfDocTemplate* doc_tmpl = doc->GetDocTemplate();
    if (!doc_tmpl)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
    if (stm) {
      if (!doc_tmpl->LoadFromStream(stm, kDataFormatJson))
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
      stm->Destroy();
    }
  }

  if (!doc->AddTags(nullptr, nullptr))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  doc->Close();
  pdfix->Destroy();
}
//! [AddTags_cpp]
