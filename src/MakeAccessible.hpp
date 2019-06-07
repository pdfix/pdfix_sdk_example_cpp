////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeAccessible.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage MakeAccessible_cpp
*/
/*!
\page MakeAccessible_cpp Make PDF Accessible Sample
Example how to make PDF Accessible.
\snippet /MakeAccessible.hpp MakeAccessible_cpp
*/

#pragma once

//! [MakeAccessible_cpp]
#include <string>
#include <iostream>
#include <memory>
#include "Pdfix.h"
#include "OcrWithTesseract.hpp"

void MakeAccessible(
  const std::wstring& email,               // authorization email   
  const std::wstring& license_key,         // authorization license key
  const std::wstring& open_path,           // source PDF document
  const std::wstring& save_path,           // output PDF/UA document
  const std::wstring& language,            // document reading language
  const std::wstring& title,               // document title
  const std::wstring& config_path          // configuration file
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

  // set document language
  if (!language.empty())
    doc->SetLang(language.c_str());

  // set documnt title
  if (!title.empty())
    doc->SetInfo(L"Title", title.c_str());

  // convert to PDF/UA
  PdfAccessibleParams params;
  params.embed_fonts = 1;
  params.subset_fonts = 1;
  //params.accept_tags = 1;
 
  if (!doc->MakeAccessible(&params, nullptr, nullptr))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  doc->Close();
  pdfix->Destroy();
}
//! [MakeAccessible_cpp]
