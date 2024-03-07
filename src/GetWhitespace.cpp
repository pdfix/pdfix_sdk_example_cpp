////////////////////////////////////////////////////////////////////////////////////////////////////
// GetWhitespace.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/GetWhitespace.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace GetWhitespace {
void Run(const std::wstring& open_path  // source PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();
  PdePageMap* page_map = page->AcquirePageMap();
  if (!page_map)
    throw PdfixException();
  if (!page_map->CreateElements())
    throw PdfixException();

  PdfRect bbox;
  PdfWhitespaceParams whitespace_params;
  // set watermark width in user space coordinates
  whitespace_params.width = 100;
  // set watermark height in user space coordinates
  whitespace_params.height = 50;
  if (page_map->GetWhitespace(&whitespace_params, 0, &bbox)) {
    // use the bbox to place watermark into it - AddWatermark example
    // ...
  }

  page->Release();
  doc->Close();
}
}  // namespace GetWhitespace
