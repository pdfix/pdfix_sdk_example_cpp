////////////////////////////////////////////////////////////////////////////////////////////////////
// FlattenAnnots.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage FlattenAnnots_cpp
*/
/*!
\page FlattenAnnots_cpp Flatten Annots Sample
Example how to flatten annotations into the PDF content.
\snippet /FlattenAnnots.hpp FlattenAnnots_cpp
*/

#pragma once

//! [FlattenAnnots_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

void FlattenAnnots(
  const std::wstring& email,                   // authorization email   
  const std::wstring& license_key,             // authorization license key
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path,               // output PDF doucment
  PdfFlattenAnnotsParams& params               // flatten annotations parameters
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  if (!doc->FlattenAnnots(&params))
    throw std::runtime_error(pdfix->GetError());

  if (!doc->Save(save_path.c_str(), kSaveIncremental))
    throw std::runtime_error(pdfix->GetError());

  doc->Close();
  pdfix->Destroy();
}
//! [FlattenAnnots_cpp]