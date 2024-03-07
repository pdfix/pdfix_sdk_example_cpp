////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractText.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractText.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

namespace ExtractText {
auto doc_deleter = [](PdfDoc* doc) {
  if (doc)
    doc->Close();
};
auto page_deleter = [](PdfPage* page) {
  if (page)
    page->Release();
};
auto page_map_deleter = [](PdePageMap* page_map) {
  if (page_map)
    page_map->Release();
};

// GetText processes each element recursively. If the element is a text, saves it to the output
// stream.
void GetText(PdeElement* element, std::stringstream& ss) {
  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeText) {
    PdeText* text_elem = static_cast<PdeText*>(element);
    std::wstring text;
    text = text_elem->GetText();

    std::string str = ToUtf8(text);
    ss << str << std::endl;
  } else {
    // process children
    int count = element->GetNumChildren();
    if (count == 0)
      return;
    for (int i = 0; i < count; i++) {
      PdeElement* child = element->GetChild(i);
      if (child)
        GetText(child, ss);
    }
  }
}

void GetPageText(PdfPage* page, std::stringstream& ss) {
  std::unique_ptr<PdePageMap, decltype(page_map_deleter)> page_map(page->AcquirePageMap(),
                                                                   page_map_deleter);
  if (!page_map)
    throw PdfixException();
  if (!page_map->CreateElements())
    throw PdfixException();

  PdeElement* container = page_map->GetElement();
  if (!container)
    throw PdfixException();

  GetText(container, ss);
}

// Extracts texts from the document and saves them to TXT format.
void Run(const std::wstring& open_path,    // source PDF document
         std::ostream& output,             // output stream
         const std::wstring& config_path,  // configuration file
         const int page_number) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  std::stringstream ss;

  auto num_pages = doc->GetNumPages();
  auto from_page = page_number == -1 ? 0 : page_number;
  auto to_page = page_number == -1 ? num_pages - 1 : page_number;

  for (auto i = from_page; i <= to_page; i++) {
    std::unique_ptr<PdfPage, decltype(page_deleter)> page(doc->AcquirePage(i), page_deleter);
    if (!page)
      throw PdfixException();
    GetPageText(page.get(), ss);
  }

  // write text to stream
  output << ss.str();

  // destroy variables
  doc->Close();
}
}  // namespace ExtractText
