#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

// Example how to extract tables from a PDF document and save them to csv format.
// GetText processes each element recursively. If the element is a text, saves it to the output stream.
void GetText(PdeText* element, std::ofstream& ofs, bool eof);

// SaveTable processes each element recursively.
// If the element is a table, it saves it to save_path as csv.
void SaveTable(PdeElement* element, std::wstring save_path, int& table_index);

// Extracts all tables from the document and saves them to CSV format.
void ExtractTables(
    const std::wstring& open_path,                 // source PDF document
    const std::wstring& save_path                  // directory where to extract images
    );
