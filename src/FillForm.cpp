////////////////////////////////////////////////////////////////////////////////////////////////////
// FillForm.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/FillForm.h"

// system
#include <iostream>
#include <string>
// other libraries
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);
extern std::wstring FromUtf8(const std::string& wstr);

// Adds a new text annotation.
void FillForm(const std::wstring& open_file,  // source PDF document
              const std::wstring& save_file,  // directory where to save PDF docuemnt
              const std::wstring& json_file,  // json with field values
              bool flatten                    // flatten for fields
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
  if (!doc)
    throw PdfixException();

  ptree json;
  read_json(ToUtf8(json_file), json);

  for (auto& item : json) {
    std::wstring name = FromUtf8(item.first);
    std::wstring value = FromUtf8(item.second.get_value<std::string>());
    PdfFormField* form_field = doc->GetFormFieldByName(name.c_str());
    if (form_field)
      form_field->SetValue(value.c_str());
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

  if (!doc->Save(save_file.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
