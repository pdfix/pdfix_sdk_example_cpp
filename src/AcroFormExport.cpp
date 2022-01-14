////////////////////////////////////////////////////////////////////////////////////////////////////
// AcroFormExport.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AcroFormExport.h"
#include "pdfixsdksamples/Utils.h"

//system
#include <string>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//project
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

namespace AcroFormExport {
  void ProcessWidget(PdfDoc* doc, PdsDictionary* widget_obj, ptree& json) {
    auto page_obj = widget_obj->GetDictionary(L"P");
    auto page_num = doc->GetPageNumFromObject(page_obj);
    json.put("page_num", page_num + 1);
  }
  
  void ProcessFormField(PdfDoc* doc, PdfFormField* field, ptree& json, bool widgets) {
    json.put("name", ToUtf8(field->GetFullName()));
    json.put("value", ToUtf8(field->GetValue()));
    json.put("default_value", ToUtf8(field->GetDefaultValue()));
    json.put("tooltip", ToUtf8(field->GetTooltip()));
    auto field_type = field->GetType();
    switch (field_type) {
      case kFieldButton: json.put("type", "button"); break;
      case kFieldRadio: json.put("type", "radio"); break;
      case kFieldCheck: json.put("type", "check"); break;
      case kFieldText: json.put("type", "text"); break;
      case kFieldCombo: json.put("type", "combo"); break;
      case kFieldList: json.put("type", "list"); break;
      case kFieldSignature: json.put("type", "signature"); break;
      default:
        break;
    }
    json.put("flags", field->GetFlags());
    
    // options - list box and combo box
    ptree options;
    for (int i = 0; i < field->GetNumOptions(); i++) {
      ptree option;
      option.put("caption", ToUtf8(field->GetOptionCaption(i)));
      option.put("value", ToUtf8(field->GetOptionCaption(i)));
      options.push_back(std::make_pair("", option));
    }
    if (options.size() != 0)
      json.add_child("options", options);
      
    // export values
    if (field_type == kFieldRadio || field_type == kFieldCheck) {
      ptree export_values;
      for (int i = 0; i < field->GetNumExportValues(); i++) {
        export_values.put("", ToUtf8(field->GetExportValue(i)));
      }
      json.add_child("exports", export_values);
    }
    if (field_type == kFieldText || field_type == kFieldCombo) {
      json.put("max_length", field->GetMaxLength());
      json.put("multiline", (field->GetFlags() & kFieldFlagMultiline) != 0);
    }
    json.put("required", (field->GetFlags() & kFieldFlagRequired) != 0);
    json.put("read_only", (field->GetFlags() & kFieldFlagReadOnly) != 0);
    
    // kids
    if (widgets) {
      auto field_obj = field->GetObject();
      if (field_obj) {
        ptree kids_json;
        auto kids = field_obj->GetArray(L"Kids");
        if (kids) {
          for (int i = 0; i < kids->GetNumObjects(); i++) {
            auto kid_obj = kids->GetDictionary(i);
            ptree kid_json;
            ProcessWidget(doc, kid_obj, kid_json);
            kids_json.push_back(std::make_pair("", kid_json));
          }
        }
        else {
          // field is widget annotation
          ptree kid_json;
          ProcessWidget(doc, field_obj, kid_json);
          kids_json.push_back(std::make_pair("", kid_json));
        }
        json.add_child("kids", kids_json);
      }
    }
  }

  void Run(
    const std::wstring& open_path,         // source PDF document
    std::ostream& output,                  // output JSON document
    bool widgets                           // include widget annots
  ) {
    Pdfix* pdfix = PdfixEngine::Get();

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();
      
    ptree acroform_root;

    for (int i = 0; i < doc->GetNumFormFields(); i++) {
      PdfFormField* field = doc->GetFormField(i);
      if (!field)
        continue;
      ptree field_json;
      ProcessFormField(doc, field, field_json, widgets);
      acroform_root.push_back(std::make_pair("", field_json));
    }
    
    ptree output_json;
    output_json.add_child("acroform", acroform_root);

    write_json(output, output_json, true);
    
    doc->Close();
  }
}