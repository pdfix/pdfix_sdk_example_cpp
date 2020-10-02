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

#include "pdfixsdksamples/ExportFormFieldValues.h"

//! [ExportFormFieldValues_cpp]
#include <string>
#include <iostream>
#include <fstream>
#include "Pdfix.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

void ExportFormFieldValues(
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path                // output PDF document
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

  std::string path = ToUtf8(save_path);
  std::ofstream ofs;
  ofs.open(path);

  for (int i = 0; i < doc->GetNumFormFields(); i++) {
    PdfFormField* field = doc->GetFormField(i);
    if (!field)
      throw PdfixException();

    auto name = field->GetFullName();
    auto value = field->GetValue();
    ofs << ToUtf8(name) << ": " << ToUtf8(value) << std::endl;
  }
  ofs.close();
  doc->Close();

  pdfix->Destroy();
}

//! [ExportFormFieldValues_cpp]
