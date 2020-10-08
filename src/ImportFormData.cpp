////////////////////////////////////////////////////////////////////////////////////////////////////
// ImportFormData.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ImportFormData_cpp
*/
/*!
\page ImportFormData_cpp Set Form Field Value Sample
Example how to import form data from json.
\snippet /ImportFormData.hpp ImportFormData_cpp
*/

#include "pdfixsdksamples/ImportFormData.h"

//! [ImportFormData_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"
// JSON parser
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);
extern std::wstring FromUtf8(const std::string& str);

namespace ImportFormData {
  void Run(
    const std::wstring& open_path,                 // source PDF document
    const std::wstring& save_path,                 // output PDF document
    const std::wstring& json_path,                 // json file to import
    bool flatten                                   // flatten annotations
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

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
      PdfFlattenAnnotsParams params;
      if (!doc->FlattenAnnots(&params))
        throw PdfixException();
    }

    doc->Save(save_path.c_str(), kSaveFull);
    doc->Close();
    pdfix->Destroy();
  }
}
//! [ImportFormData_cpp]
