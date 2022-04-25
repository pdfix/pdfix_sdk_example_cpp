////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveTags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RemoveTags.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void RemoveTags(
  const std::wstring& open_path,        // source PDF document
  const std::wstring& save_path        // output PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
  
  PdfTagsParams params;
  if (!doc->RemoveTags(&params, nullptr, nullptr))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
}