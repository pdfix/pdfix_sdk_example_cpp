////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractTables.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ExtractTables_cpp
*/
/*!
\page ExtractTables_cpp Pdf To Table Sample
Example how to extract images from a PDF document.
\snippet /ExtractTables.hpp ExtractTables_cpp
*/

#pragma once

//! [ExtractTables_cpp]
#include <string>
#include <iostream>
#include <fstream>
#include "Pdfix.h"

// Example how to extract tables from a PDF document and save them to csv format.
// GetText processes each element recursively. If the element is a text, saves it to the output stream.
void GetText(PdeElement* element, std::ofstream& ofs, bool eof) {
  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeText) {
    PdeText* text_elem = static_cast<PdeText*>(element);
    std::wstring text;
    text.resize(text_elem->GetText(nullptr, 0));
    text_elem->GetText((wchar_t*)text.c_str(), text.size());
    std::string str = ToUtf8(text);
    ofs << str;
    if (eof)
      ofs << std::endl;
  }
  else {
    int count = element->GetNumChildren();
    if (count == 0)
      return;
    for (int i = 0; i < count; i++) {
      PdeElement* child = element->GetChild(i);
      if (child)
        GetText(child, ofs, eof);
    }
  }
}

// SaveTable processes each element recursively. 
// If the element is a table, it saves it to save_path as csv.
void SaveTable(PdeElement* element, std::wstring save_path, int& table_index) {
  Pdfix* pdfix = GetPdfix();

  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeTable) {
    PdeTable* table = static_cast<PdeTable*>(element);

    auto path = save_path + L"table_" + std::to_wstring(table_index++) + L".csv";
    std::ofstream ofs;
    ofs.open(ToUtf8(path));

    int row_count = table->GetNumRows();
    int col_count = table->GetNumCols();

    for (int row = 0; row < row_count; row++) {
      for (int col = 0; col < col_count; col++) {
        PdeCell* cell = (PdeCell*)table->GetCell(row, col);
        if (!cell)
          continue;

        int row_span = cell->GetRowSpan();
        int col_span = cell->GetColSpan();

        int count = cell->GetNumChildren();
        if ((row_span != 0) && (col_span != 0) && (count > 0)) {
          ofs << "\"";
          for (int i = 0; i < count; i++) {
            PdeElement* child = cell->GetChild(i);
            if (child && (child->GetType() == kPdeText)) {
              GetText(child, ofs, false);
            }
            if (i < count) {
              ofs << " ";
            }
          }
          ofs << "\"";
        }

        if (col < col_count)
          ofs << std::endl;
      }
      if (row < row_count)
        ofs << std::endl;
    }

    ofs.close();
  }
  else {
    int count = element->GetNumChildren();
    if (count == 0)
      return;
    for (int i = 0; i < count; i++) {
      PdeElement* child = element->GetChild(i);
      if (child)
        SaveTable(child, save_path, table_index);
    }
  }
}

// Extracts all tables from the document and saves them to CSV format. 
void ExtractTables(
  std::wstring email,                     // authorization email   
  std::wstring license_key,               // authorization license key
  std::wstring open_path,                 // source PDF document
  std::wstring save_path                  // directory where to extract images
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

  int table_index = 1;

  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    std::cout << std::endl;
    std::cout << "Processing pages..." << i + 1 << "/" << num_pages;

    PdfPage* page = doc->AcquirePage(i);
    if (!page)
      throw std::runtime_error(pdfix->GetError());
    PdePageMap* page_map = page->AcquirePageMap(nullptr, nullptr);
    if (!page_map)
      throw std::runtime_error(pdfix->GetError());

    auto element = page_map->GetElement();
    if (!element)
      throw std::runtime_error(pdfix->GetError());

    SaveTable(element, save_path, table_index);

    doc->ReleasePage(page);
  }
  std::cout << std::endl << table_index - 1 << " tables found" << std::endl;

  doc->Close();
  pdfix->Destroy();
}
//! [ExtractTables_cpp]