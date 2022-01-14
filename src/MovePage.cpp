////////////////////////////////////////////////////////////////////////////////////////////////////
// MovePage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/MovePage.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

  // Move page in document.
void MovePage(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // file where to save PDF docuemnt
  int to,
  int from
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
  if (!doc)
    throw PdfixException();

  if(!doc->MovePage(to,from))
    throw PdfixException();
  if(!doc->Save(save_file.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
}