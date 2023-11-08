#pragma once

// system
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <string>
// project
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace PagesToJson {
static auto doc_deleter = [](PdfDoc* doc) {
  if (doc)
    doc->Close();
};
static auto page_deleter = [](PdfPage* page) {
  if (page)
    page->Release();
};

const int kFlagExportGeometry = 0x01;
const int kFlagExportText = 0x02;

void ProcessPage(PdfPage* page, ptree& json, int flags);

// Extract all documents bookmars into json.
void Run(const std::wstring& open_path,  // source PDF document
         const std::wstring& password,   // open password
         std::ostream& output,           // output stream
         int export_flags,               // export flags
         int page_num                    // page number to process
);
}  // namespace PagesToJson
