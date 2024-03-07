////////////////////////////////////////////////////////////////////////////////////////////////////
// ImportFormData.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ImportFormData.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"
// JSON parser
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);
extern std::wstring FromUtf8(const std::string& str);

namespace ImportFormData {
void Run(const std::wstring& open_path,  // source PDF document
         const std::wstring& password,   // open password
         const std::wstring& save_path,  // output PDF document
         const std::wstring& json_path,  // json file to import
         bool flatten                    // flatten annotations
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  ptree node;
  // load json
  read_json(ToUtf8(json_path), node);
  for (auto& item : node) {
    std::wstring name = FromUtf8(item.first);
    std::wstring value = FromUtf8(item.second.get_value<std::string>());
    PdfFormField* field = doc->GetFormFieldByName(name.c_str());
    if (field) {
      if (!field->SetValue(value.c_str()))
        throw PdfixException();
    }
  }

  if (flatten) {
    for (auto i = 0; i < doc->GetNumPages(); i++) {
      auto page = doc->AcquirePage(i);
      if (!page)
        throw PdfixException();
      for (auto j = page->GetNumAnnots() - 1; j >= 0; j--) {
        auto annot = page->GetAnnot(i);
        // flatten all but link annotations which to not have appearance
        if (annot && annot->GetSubtype() == kAnnotWidget)
          if (!page->FlattenAnnot(annot))
            throw PdfixException();
      }
      page->Release();
    }
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
}  // namespace ImportFormData
