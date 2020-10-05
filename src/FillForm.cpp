////////////////////////////////////////////////////////////////////////////////////////////////////
// FillForm.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage FillForm_cpp
*/
/*!
\page FillForm_cpp Add Comment Sample
Example how to add a text annotation with a reply.
\snippet /FillForm.hpp FillForm_cpp
*/

#include "pdfixsdksamples/FillForm.h"

//! [FillForm_cpp]
// system
#include <string>
#include <iostream>
// other libraries
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);
extern std::wstring FromUtf8(const std::string& wstr);

  // Adds a new text annotation.
void FillForm(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // directory where to save PDF docuemnt
  const std::wstring& json_file,                // json with field values
  bool flatten                                  // flatten for fields
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

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
  
  PdfFlattenAnnotsParams params;
  if (flatten) {
    if (!doc->FlattenAnnots(&params))
      throw PdfixException();
  }

  if (!doc->Save(save_file.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();

  pdfix->Destroy();
}
//! [FillForm_cpp]