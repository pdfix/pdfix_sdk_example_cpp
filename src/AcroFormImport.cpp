////////////////////////////////////////////////////////////////////////////////////////////////////
// AcroFormImport.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AcroFormImport.h"

//system
#include <string>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//project
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);
extern std::wstring FromUtf8(const std::string& wstr);

namespace AcroFormImport {
  
  void Run(
    const std::wstring& open_path,         // source PDF document
    const std::wstring& save_path,         // destination PDF document
    const std::wstring& json_path          // path to JSON to import
  ) {
    Pdfix* pdfix = PdfixEngine::Get();

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();
      
    ptree acroform_root;
    read_json(ToUtf8(json_path), acroform_root);
    
    auto node = acroform_root.get_child_optional("acroform");
    if (node) {
      // enumerate childs
      for (auto& item : node.get()) {
        auto name = item.second.get_optional<std::string>("name");
        auto value = item.second.get_optional<std::string>("value");
        auto read_only = item.second.get_optional<bool>("read_only");
        auto required = item.second.get_optional<bool>("required");
        auto multiline = item.second.get_optional<bool>("multiline");
        
        if (!name)
          continue;
        PdfFormField* field = doc->GetFormFieldByName(FromUtf8(name.get()).c_str());
        if (!field)
          continue;
          
        // update value
        if (value) {
          auto old_value = field->GetValue();
          if (old_value != FromUtf8(value.get()))
            field->SetValue(FromUtf8(value.get()).c_str());
        }
        
        // field flags
        auto field_flags = field->GetFlags();
        auto check_field_flags_flags = [&](auto mask, auto set) {   // update new flags lambda
          if ( ( field_flags & mask ) != (set ? mask : 0) ) {
            if ( set ) field_flags |= mask;
            else field_flags &= ~mask;
          }
        };
        if (read_only) check_field_flags_flags(kFieldFlagReadOnly, read_only.get());
        if (required) check_field_flags_flags(kFieldFlagRequired, required.get());
        if (multiline) check_field_flags_flags(kFieldFlagMultiline, multiline.get());
        if (field_flags != field->GetFlags())
          field->SetFlags(field_flags);
      }
    }

    // save PDF
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();
    
    doc->Close();
  }
}