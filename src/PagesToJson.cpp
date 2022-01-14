////////////////////////////////////////////////////////////////////////////////////////////////////
// PagesToJson.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/PagesToJson.h"

// system
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "Pdfix.h"
#include "pdfixsdksamples/ExtractText.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace PagesToJson {
  void ProcessPage(PdfPage* page, ptree& json, int flags) {
    json.put("number", page->GetNumber() + 1);
    
    if (flags & kFlagExportGeometry) {
      auto crop = page->GetCropBox();
      json.put("width", crop.right - crop.left);
      json.put("height", crop.top - crop.bottom);
      json.put("rotate", page->GetRotate());
    }
    
    if (flags & kFlagExportText) {
      std::stringstream ss;
      ExtractText::GetPageText(page, ss);
      json.put("text", ss.str());
    }
  }

  // Extract all documents bookmars into json.
  void Run(
    const std::wstring& open_path,                      // source PDF document
    const std::wstring& password,                       // open document password
    std::ostream& output,                               // output stream
    int export_flags,                                   // export flags
    int page_num                                        // page number to process
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
    if (!doc)
      throw PdfixException();
      
    int from_page = page_num == -1 ? 0 : page_num;
    int to_page = page_num == -1 ? doc->GetNumPages() - 1 : page_num;
    
    ptree pages_root;
    
    for (int i = from_page; i <= to_page; i++) {
      std::unique_ptr<PdfPage, decltype(page_deleter)> page(doc->AcquirePage(i), page_deleter);
      if (!page)
        throw PdfixException();

      ptree page_json;
      ProcessPage(page.get(), page_json, export_flags);
      pages_root.push_back(std::make_pair("", page_json));
    }

    ptree output_json;
    output_json.add_child("pages", pages_root);

    write_json(output, output_json, false);

    doc->Close();
    pdfix->Destroy();
  }
}