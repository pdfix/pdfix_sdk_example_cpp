////////////////////////////////////////////////////////////////////////////////////////////////////
// ExportFormFieldValues.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ExportFormFieldValues_cpp
*/
/*!
\page ExportFormFieldValues_cpp Export Form Field Values Sample
Example how to export form fields values into a txt file.
\snippet /ExportFormFieldValues.hpp ExportFormFieldValues_cpp
*/

#pragma once

//! [ExportFormFieldValues_cpp]
#include <string>
#include <iostream>
#include <fstream>
#include "Pdfix.h"

extern std::string ToUtf8(const std::wstring& wstr);

void ExportFormFieldValues(
  const std::wstring& email,                   // authorization email   
  const std::wstring& license_key,             // authorization license key
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path                // output PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  std::string path = ToUtf8(save_path);
  std::ofstream ofs;
  ofs.open(path);

  for (int i = 0; i < doc->GetNumFormFields(); i++) {
    std::wstring name, value;
    PdfFormField* field = doc->GetFormField(i);
    if (!field)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    name.resize(field->GetFullName(nullptr, 0));
    field->GetFullName((wchar_t*)name.c_str(), (int)name.size());

    value.resize(field->GetValue(nullptr, 0));
    field->GetValue((wchar_t*)value.c_str(), (int)value.size());

    ofs << ToUtf8(name) << ": " << ToUtf8(value) << std::endl;
  }
  ofs.close();
  doc->Close();

  pdfix->Destroy();
}

//! [ExportFormFieldValues_cpp]
