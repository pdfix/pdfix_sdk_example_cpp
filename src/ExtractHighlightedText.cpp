////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractHighlightedText.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ExtractHighlightedText_cpp
*/
/*!
\page ExtractHighlightedText_cpp Pdf To Text Sample
Example how to extract tables from a PDF document and save them to csv format.
\snippet /ExtractHighlightedText.hpp ExtractHighlightedText_cpp
*/

#include "pdfixsdksamples/ExtractHighlightedText.h"

//! [ExtractHighlightedText_cpp]
#include <string>
#include <iostream>
#include <sstream>
#include "Pdfix.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

// HasHighlight rerturns true if there is an highlight annotation over the char_rect
bool HasHighlight(PdfPage* page, PdfRect& char_rect) {
  // deflate char rect to minimal
  char_rect.left += (char_rect.right - char_rect.left) / 2.;
  char_rect.right = char_rect.left + 1;
  char_rect.bottom += (char_rect.top - char_rect.bottom) / 2.;
  char_rect.top = char_rect.bottom + 1;

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
void GetHighlightedText(PdfPage* page, PdeElement* element, std::stringstream& ss) {
  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeText) {
    PdeText* text_elem = static_cast<PdeText*>(element);
    std::string text;

    int num_lines = text_elem->GetNumTextLines();
    for (int l = 0; l < num_lines; l++) {
      PdeTextLine* line = text_elem->GetTextLine(l);
      if (!line)
        return;
      // if line is bullet or newline, write a new line
      if ((line->GetFlags() & kTextLineFlagNewLine) != 0 && l > 0) {
        // write text into the output stream
        if (text.size() > 0) {
          ss << text;
          ss << std::endl;
          text = "";
        }
      }

      int num_words = line->GetNumWords();
      for (int w = 0; w < num_words; w++) {
        PdeWord* word = line->GetWord(w);
        if (!word)
          return;
        // iterate through each character
        int length = word->GetNumChars();
        for (int i = 0; i < length; i++) {
          PdfRect char_bbox;
          word->GetCharBBox(i, &char_bbox);

          // add text only if there is a highlight over it
          if (HasHighlight(page, char_bbox)) {
            std::wstring char_str = word->GetCharText(i);
            text += ToUtf8(char_str);
          }
        }
        // add whitespace between words
        if (text.size() > 0)
          text += " ";
      }
      // add whitespace between lines
      if ((l != num_lines - 1) && (text.size() > 0))
        text += " ";
    }

    // write text into the output stream
    if (text.size() > 0) {
      ss << text;
      ss << std::endl;
      text = "";
    }
  }
  else {
    // process children
    int count = element->GetNumChildren();
    if (count == 0)
      return;
    for (int i = 0; i < count; i++) {
      PdeElement* child = element->GetChild(i);
      if (child)
        GetHighlightedText(page, child, ss);
    }
  }
}

// Extracts texts from the document and saves them to TXT format. 
void ExtractHighlightedText(
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path,      // output TXT file
  const std::wstring& config_path     // configuration file
) {
  std::cout << "ExtractHighlightedText " << std::endl;

  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  std::stringstream ss;

  ss << "<!-- PDFix SDK " << pdfix->GetVersionMajor() << "." << pdfix->GetVersionMinor()
    << "." << pdfix->GetVersionPatch() 
    << " conversion PDF to TXT. License: http://pdfix.net/terms -->" << std::endl;

  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    std::cout << std::endl;
    std::cout << "Processing pages..." << i + 1 << "/" << num_pages;

    ss << std::endl << "Page: " << i + 1 << std::endl;

    PdfPage* page = doc->AcquirePage(i);
    if (!page)
      throw std::runtime_error(pdfix->GetError());
    PdePageMap* page_map = page->AcquirePageMap(nullptr, nullptr);
    if (!page_map)
      throw std::runtime_error(pdfix->GetError());

    PdeElement* container = page_map->GetElement();
    if (!container)
      throw std::runtime_error(pdfix->GetError());

    GetHighlightedText(page, container, ss);

    page->Release();
  }
  std::cout << std::endl;

  // write text to file stream
  PsFileStream* stream = pdfix->CreateFileStream(save_path.c_str(), kPsWrite);
  if (!stream)
    throw std::runtime_error(pdfix->GetError());

  auto str = ss.str();
  std::vector<unsigned char> data(begin(str), end(str));
  stream->Write(stream->GetPos(), &data[0], (int)data.size());

  stream->Destroy();

  // destroy variables
  doc->Close();
  pdfix->Destroy();
}

//! [ExtractHighlightedText_cpp]
