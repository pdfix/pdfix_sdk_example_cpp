////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractText.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ExtractText_cpp
*/
/*!
\page ExtractText_cpp Pdf To Text Sample
Example how to extract tables from a PDF document and save them to csv format.
\snippet /ExtractText.hpp ExtractText_cpp
*/

#pragma once

//! [ExtractText_cpp]
#include <string>
#include <iostream>
#include <sstream>
#include "Pdfix.h"

extern std::string ToUtf8(const std::wstring& wstr);

// GetText processes each element recursively. If the element is a text, saves it to the output stream.
void GetText(PdeElement* element, std::stringstream& ss, bool eof) {
  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeText) {
    PdeText* text_elem = static_cast<PdeText*>(element);
    std::wstring text;
#define BY_WORDS 0
#if BY_WORDS
    std::vector<int> codes;
    auto num_words = text_elem->GetNumWords();
    for (auto i = 0; i < num_words; i++) {
      auto word = text_elem->GetWord(i);
      auto num_chars = word->GetNumChars();
      for (int j = 0; j < num_chars; j++) {
        std::wstring text;
        word->GetCharText(j, text);
        if (text.empty()) {
          codes.push_back(word->GetCharCode(j));
          text.push_back(codes.back());
        }
      }
      text += L" ";
    }
#else
    text.resize(text_elem->GetText(nullptr, 0));
    text_elem->GetText((wchar_t*)text.c_str(), (int)text.size());
#endif
    
    std::string str = ToUtf8(text);
    ss << str;
    if (eof)
      ss << std::endl;
  }
  else {
    // process children
    int count = element->GetNumChildren();
    if (count == 0)
      return;
    for (int i = 0; i < count; i++) {
      PdeElement* child = element->GetChild(i);
      if (child)
        GetText(child, ss, eof);
    }
  }
}

// Extracts texts from the document and saves them to TXT format. 
void ExtractText(
  const std::wstring& email,          // authorization email
  const std::wstring& license_key,    // authorization license key
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path,      // output TXT file
  const std::wstring& config_path     // configuration file
) {
  std::cout << "ExtractText " << std::endl;

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

  std::stringstream ss;

  ss << "<!-- PDFix SDK " << pdfix->GetVersionMajor() << "." << pdfix->GetVersionMinor()
    << "." << pdfix->GetVersionPatch() 
    << " conversion PDF to TXT. License: http://pdfix.net/terms -->" << std::endl;

  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    std::cout << std::endl;
    std::cout << "Processing pages..." << i + 1 << "/" << num_pages;

    ss << std::endl << "Page: " << i << std::endl;

    PdfPage* page = doc->AcquirePage(i);
    if (!page)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    PdePageMap* page_map = page->AcquirePageMap(nullptr, nullptr);
    if (!page_map)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    PdeElement* container = page_map->GetElement();
    if (!container)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    GetText(container, ss, true);

    page->Release();
  }
  std::cout << std::endl;

  // write text to file stream
  PsFileStream* stream = pdfix->CreateFileStream(save_path.c_str(), kPsWrite);
  if (!stream)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  auto str = ss.str();
  std::vector<unsigned char> data(begin(str), end(str));
  stream->Write(stream->GetPos(), &data[0], (int)data.size());
  stream->Destroy();

  // destroy variables
  doc->Close();
  pdfix->Destroy();
}

//! [ExtractText_cpp]
