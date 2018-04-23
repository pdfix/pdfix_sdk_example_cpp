////////////////////////////////////////////////////////////////////////////////////////////////////
// SetFormFieldValue.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage SetFormFieldValue_cpp
*/
/*!
\page SetFormFieldValue_cpp Set Form Field Value Sample
Example how to set a value into a form field.
\snippet /SetFormFieldValue.hpp SetFormFieldValue_cpp
*/

#pragma once

//! [SetFormFieldValue_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // Sets form field value.
void SetFormFieldValue(
  std::wstring email,                     // authorization email   
  std::wstring license_key,               // authorization license key
  std::wstring open_path,                 // source PDF document
  std::wstring save_path                  // output PDF document
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
  PdfFormField* field = doc->GetFormFieldByName(L"Text1");
  if (field) {
    std::wstring value;
    value.resize(field->GetValue(nullptr, 0));
    field->GetValue((wchar_t*)value.c_str(), value.size());
    if (value.length() == 0)
      value = L"New value";
    else
      std::reverse(std::begin(value), std::end(value));
    field->SetValue(value.c_str());
  }

  doc->Save(save_path.c_str(), kSaveFull);
  doc->Close();
  pdfix->Destroy();
}
//! [SetFormFieldValue_cpp]