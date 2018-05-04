////////////////////////////////////////////////////////////////////////////////////////////////////
// SetFieldFlags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage SetFieldFlags_cpp
*/
/*!
\page SetFieldFlags_cpp Set Field Flags Sample
Sets form fields flags as readonly.
\snippet /SetFieldFlags.hpp SetFieldFlags_cpp
*/

#pragma once

//! [SetFieldFlags_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

// Example how to change form fields flags.
void SetFieldFlags(
  const std::wstring& email,                     // authorization email   
  const std::wstring& license_key,               // authorization license key
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF doucment
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  for (int i = 0; i < doc->GetNumFormFields(); i++) {
    std::wstring name, value;
    PdfFormField* field = doc->GetFormField(i);
    if (!field)
      throw std::runtime_error(pdfix->GetError());

    auto flags = field->GetFlags();
    flags |= kFieldFlagReadOnly;
    if (!field->SetFlags(flags))
      throw std::runtime_error(pdfix->GetError());
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw std::runtime_error(pdfix->GetError());
  doc->Close();
  pdfix->Destroy();
}
//! [SetFieldFlags_cpp]