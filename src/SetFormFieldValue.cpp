////////////////////////////////////////////////////////////////////////////////////////////////////
// SetFormFieldValue.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/SetFormFieldValue.h"

#include <string>
#include <iostream>
#include <algorithm>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void SetFormFieldValue(
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF document
) {
  auto pdfix = PdfixEngine::Get();

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
}