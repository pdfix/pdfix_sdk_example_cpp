////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractHighlightedText.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractHighlightedText.h"

#include <iostream>
#include <sstream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

namespace ExtractHighlightedText {

// HasHighlight rerturns true if there is an highlight annotation over the char_rect
bool HasHighlight(PdfPage* page, PdfRect& char_rect) {
  // deflate char rect to minimal
  char_rect.left += (char_rect.right - char_rect.left) / 2.f;
  char_rect.right = char_rect.left + 1.f;
  char_rect.bottom += (char_rect.top - char_rect.bottom) / 2.f;
  char_rect.top = char_rect.bottom + 1.f;

  // get annotations over the bbox
  int num_annots = page->GetNumAnnotsAtRect(&char_rect);
  for (int i = 0; i < num_annots; i++) {
    PdfAnnot* annot = page->GetAnnotAtRect(&char_rect, i);
    if (annot) {
      PdfAnnotSubtype subtype = annot->GetSubtype();
      if (subtype == kAnnotHighlight)
        return true;
    }
  }
  return false;
}

// GetHighlightedText processes each element recursively.
// If the element is a highlighted text, saves it to the output stream.
void GetHighlightedText(PdfPage* page, PdeElement* element, std::ostream& output) {
  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeText) {
    PdeText* text_elem = static_cast<PdeText*>(element);

    std::string text;  // collected highlighted text from the line
    auto flush_text = [&]() {
      if (text.length()) {
        output << text << std::endl;
        text.clear();
      }
    };

    int num_lines = text_elem->GetNumTextLines();
    for (int l = 0; l < num_lines; l++) {
      bool line_highlighted = false;
      PdeTextLine* line = text_elem->GetTextLine(l);
      if (!line)
        return;

      int num_words = line->GetNumWords();
      for (int w = 0; w < num_words; w++) {
        PdeWord* word = line->GetWord(w);
        if (!word)
          return;
        // iterate through each character
        int length = word->GetNumChars(false);
        for (int i = 0; i < length; i++) {
          PdfRect char_bbox;
          word->GetCharBBox(i, &char_bbox);

          // add text only if there is a highlight over it
          if (HasHighlight(page, char_bbox)) {
            line_highlighted = true;
            std::wstring char_str = word->GetCharText(i);
            text += ToUtf8(char_str);
          } else
            flush_text();
        }
        // add whitespace between words
        if (text.size() > 0 && text.back() != ' ')
          text += " ";
      }
      flush_text();
    }
  } else {
    // process children
    int count = element->GetNumChildren();
    if (count == 0)
      return;
    for (int i = 0; i < count; i++) {
      PdeElement* child = element->GetChild(i);
      if (child)
        GetHighlightedText(page, child, output);
    }
  }
}

// Extracts texts from the document and saves them to TXT format.
void Run(const std::wstring& open_path,   // source PDF document
         std::ostream& output,            // output stream
         const std::wstring& config_path  // configuration file
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    output << "## Page: " << i + 1 << std::endl;
    PdfPage* page = doc->AcquirePage(i);
    if (!page)
      throw std::runtime_error(pdfix->GetError());
    PdePageMap* page_map = page->AcquirePageMap();
    if (!page_map)
      throw PdfixException();
    if (!page_map->CreateElements())
      throw PdfixException();

    PdeElement* container = page_map->GetElement();
    if (!container)
      throw PdfixException();

    GetHighlightedText(page, container, output);

    page->Release();
  }
  output << std::endl;

  // destroy variables
  doc->Close();
}
}  // namespace ExtractHighlightedText
