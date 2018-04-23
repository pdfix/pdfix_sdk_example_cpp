////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeAccessible.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage MakeAccessible_cpp
*/
/*!
\page MakeAccessible_cpp Make PDF Accessible Sample
Example how to make PDF Accessible.
\snippet /MakeAccessible.hpp MakeAccessible_cpp
*/

#pragma once

//! [MakeAccessible_cpp]
#include <string>
#include <iostream>
#include <memory>
#include "Pdfix.h"
#include "OcrWithTesseract.hpp"

// Test elements recursively if there is any text. 
bool HasText(PdeElement* element) {
  Pdfix* pdfix = GetPdfix();
  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeText)
    return true;  
  int count = element->GetNumChildren();
  for (int i = 0; i < count; i++) {
    PdeElement* child = element->GetChild(i);
    if (child && HasText(child))
      return true;
  }
  return false;
}

// Test if PDF document is searchable.
bool IsSearchable(PdfDoc* doc) {
  Pdfix* pdfix = GetPdfix();
  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    auto page_deleter = [=](PdfPage* page) { doc->ReleasePage(page); };
    std::unique_ptr<PdfPage, decltype(page_deleter)> page(doc->AcquirePage(i), page_deleter);
    if (!page)
      throw std::runtime_error(pdfix->GetError());
    PdePageMap* page_map = page->AcquirePageMap(nullptr, nullptr);
    if (!page_map)
      throw std::runtime_error(pdfix->GetError());
    auto element = page_map->GetElement();
    if (!element)
      throw std::runtime_error(pdfix->GetError());
    if (HasText(element))
      return true;
  }
  return false;
}

// Make PDF accessible (PDF/UA) 
void MakeAccessible(
  std::wstring email,               // authorization email   
  std::wstring license_key,         // authorization license key
  std::wstring open_path,           // source PDF document
  std::wstring save_path,           // output PDF/UA document
  std::wstring ocr_data_path,       // path to OCR data or empty
  std::string ocr_language,         // OCR language
  std::wstring language             // document reading language
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
  
  // at first check, if document is searchable
  // if not, it needs to be converted to searchable PDF with OCR engine
  if (!ocr_data_path.empty() && !IsSearchable(doc)) {
    OcrTesseractParams ocr_params;
    ocr_params.zoom = 2;
    ocr_params.rotate = kRotate0;
    OcrWithTesseract(email, license_key, open_path, save_path, ocr_data_path, ocr_language, ocr_params);
    // reopen document from save_path
    doc->Close();
    doc = pdfix->OpenDoc(save_path.c_str(), L"");
  }

  // set reading language
  if (language.empty())
    language = L"en-US";
  doc->SetLang(language.c_str());

  // convert to PDF/UA
  PdfAccessibleParams params;
  doc->MakeAccessible(&params, nullptr, nullptr);

  doc->Close();
  pdfix->Destroy();
}
//! [MakeAccessible_cpp]