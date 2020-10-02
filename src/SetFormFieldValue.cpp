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

#include "pdfixsdksamples/SetFormFieldValue.h"

//! [SetFormFieldValue_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void SetFormFieldValue(
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
  PdfFormField* field = doc->GetFormFieldByName(L"Text1");
  if (field) {
    auto value = field->GetValue();
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
