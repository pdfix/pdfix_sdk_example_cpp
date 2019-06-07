////////////////////////////////////////////////////////////////////////////////////////////////////
// RegexSearch.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage RegexSearch_cpp
*/
/*!
\page RegexSearch_cpp Regex Search Sample
Example how to search the regex pattern at the first page of the document.
\snippet /RegexSearch.hpp RegexSearch_cpp
*/

#pragma once

//! [RegexSearch_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // Finds all occurences of the regex_pattern at the first page.
void RegexSearch(
  const std::wstring& email,                     // authorization email   
  const std::wstring& license_key,               // authorization license key
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& regex_pattern              // regex pattern you want to search
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdePageMap* page_map = page->AcquirePageMap(nullptr, nullptr);
  if (!page_map)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PsRegex* regex = pdfix->CreateRegex();
  regex->SetPattern(regex_pattern.c_str());

  PdeElement* element = page_map->GetElement();
  PdeContainer* container = static_cast<PdeContainer*>(element);
  int count = container->GetNumChildren();
  for (int i = 0; i < count; i++) {
    PdeElement* elem = container->GetChild(i);
    if (elem->GetType() == kPdeText) {
      PdeText* text_elem = static_cast<PdeText*>(elem);
      std::wstring text;
      text.resize(text_elem->GetText(nullptr, 0));
      text_elem->GetText((wchar_t*)text.c_str(), (int)text.size());

      int start_pos = 0;
      while (start_pos < (int)text.length()) {
        if (regex->Search(text.c_str(), start_pos)) {
          int pos = regex->GetPosition();
          int len = regex->GetLength();
          std::wstring match = text.substr(start_pos + pos, len);
          std::wcout << match << std::endl;
          start_pos += pos + 1;
        }
        else
          start_pos = (int)text.length();
      }
    }
  }

  regex->Destroy();

  page->Release();
  doc->Close();
  pdfix->Destroy();
}
//! [RegexSearch_cpp]
