////////////////////////////////////////////////////////////////////////////////////////////////////
// SetFieldFlags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/SetFieldFlags.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void SetFieldFlags(
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path                  // output PDF doucment
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  for (int i = 0; i < doc->GetNumFormFields(); i++) {
    std::wstring name, value;
    PdfFormField* field = doc->GetFormField(i);
    if (!field)
      throw PdfixException();

    auto flags = field->GetFlags();
    flags |= kFieldFlagReadOnly;
    if (!field->SetFlags(flags))
      throw PdfixException();
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
}