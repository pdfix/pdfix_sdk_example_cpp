////////////////////////////////////////////////////////////////////////////////////////////////////
// RegexSearch.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RegexSearch.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Finds all occurences of the regex_pattern at the first page.
void RegexSearch(
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& regex_pattern              // regex pattern you want to search
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

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();

    PdePageMap* page_map = page->AcquirePageMap();
    if (!page_map)
      throw PdfixException();
    if (!page_map->CreateElements(nullptr, nullptr))
      throw PdfixException();

  PsRegex* regex = pdfix->CreateRegex();
  regex->SetPattern(regex_pattern.c_str());

  PdeElement* element = page_map->GetElement();
  PdeContainer* container = static_cast<PdeContainer*>(element);
  int count = container->GetNumChildren();
  for (int i = 0; i < count; i++) {
    PdeElement* elem = container->GetChild(i);
    if (elem->GetType() == kPdeText) {
      PdeText* text_elem = static_cast<PdeText*>(elem);
      auto text = text_elem->GetText();

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