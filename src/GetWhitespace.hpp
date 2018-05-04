////////////////////////////////////////////////////////////////////////////////////////////////////
// GetWhitespace.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage GetWhitespace_cpp
*/
/*!
\page GetWhitespace_cpp Get Whitespace Sample
Example how to get a white space with the expected width and height.
It can be used when you search for the best place where to put a watermark.
\snippet /GetWhitespace.hpp GetWhitespace_cpp
*/

#pragma once

//! [GetWhitespace_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // GetWhitespace example
void GetWhitespace(
  const std::wstring& email,                     // authorization email   
  const std::wstring& license_key,               // authorization license key
  const std::wstring& open_path                  // source PDF document
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

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw std::runtime_error(pdfix->GetError());
  PdePageMap* page_map = page->AcquirePageMap(nullptr, nullptr);
  if (!page_map)
    throw std::runtime_error(pdfix->GetError());

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

  doc->ReleasePage(0);
  doc->Close();
  pdfix->Destroy();
}
//! [GetWhitespace_cpp]