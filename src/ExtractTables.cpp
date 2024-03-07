////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractTables.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractTables.h"

#include <fstream>
#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"
#include "pdfixsdksamples/Utils.h"

using namespace PDFixSDK;

// Example how to extract tables from a PDF document and save them to csv format.
// GetText processes each element recursively. If the element is a text, saves it to the output
// stream.
void GetText(PdeText* element, std::ofstream& ofs, bool eof) {
  PdeText* text_elem = static_cast<PdeText*>(element);
  std::wstring text = text_elem->GetText();

  std::string str = ToUtf8(text);
  ofs << str;
  if (eof)
    ofs << std::endl;
}

// SaveTable processes each element recursively.
// If the element is a table, it saves it to save_path as csv.
void SaveTable(PdeElement* element, std::wstring save_path, int& table_index) {
  auto pdfix = PdfixEngine::Get();

  PdfElementType elem_type = element->GetType();
  if (elem_type == kPdeTable) {
    PdeTable* table = static_cast<PdeTable*>(element);

    auto path = save_path + L"/ExtractTables_" + std::to_wstring(table_index++) + L".csv";
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
              GetText((PdeText*)child, ofs, false);
            }
            if (i < count - 1) {
              ofs << " ";
            }
          }
          ofs << "\"";
        }

        if (col < col_count)
          ofs << ",";
      }
      if (row < row_count)
        ofs << std::endl;
    }

    ofs.close();
  } else {
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
void ExtractTables(const std::wstring& open_path,  // source PDF document
                   const std::wstring& save_path   // directory where to extract images
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  int table_index = 1;

  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    PdfPage* page = doc->AcquirePage(i);
    if (!page)
      throw PdfixException();
    PdePageMap* page_map = page->AcquirePageMap();
    if (!page_map)
      throw PdfixException();
    if (!page_map->CreateElements())
      throw PdfixException();

    auto element = page_map->GetElement();
    if (!element)
      throw PdfixException();

    SaveTable(element, save_path, table_index);

    page->Release();
  }
  std::cout << std::endl << table_index - 1 << " tables found" << std::endl;

  doc->Close();
}
