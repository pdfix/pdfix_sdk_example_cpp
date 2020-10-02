////////////////////////////////////////////////////////////////////////////////////////////////////
// NamedDestsToJson.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage NamedDestsToJson_cpp
*/
/*!
\page NamedDestsToJson_cpp Save Named Destinations to JSON Sample
Example how to extract all named destinations in documnent into json.
\snippet /NamedDestsToJson.hpp NamedDestsToJson_cpp
*/

#include "pdfixsdksamples/NamedDestsToJson.h"

//! [NamedDestsToJson_cpp]
// system
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);

namespace NamedDestsToJson {
  bool ProcessViewDestination(PdfViewDestination* view_dest, PdfDoc* doc, ptree& json) {
    if (view_dest && (view_dest = view_dest->Resolve(doc)) && view_dest) {
      auto page_num = view_dest->GetPageNum(doc);
      if (page_num != -1) {
        // process only valid named destinations pointing to existing pages
        json.put("page_num", page_num + 1);
        return true;
      }
    }
    return false;
  }

  void ProcessNamedDest(PdsObject* name, PdsObject* value, PdfDoc* doc, ptree& json) {
    ptree dest_json;
    auto text = static_cast<PdsString*>(name)->GetText();
    auto view_dest = doc->GetViewDestinationFromObject(value);
    if (view_dest && ProcessViewDestination(view_dest, doc, dest_json)) {
      // named dest may contain dots so define space as a delimiter
      json.add_child(ptree::path_type(ToUtf8(text), ' '), dest_json);
    }
  }

  void ProcessNameTreeObject(PdsObject* obj, PdfDoc* doc, ptree& json) {
    if (!obj) return;
    
    if (obj->GetObjectType() == kPdsDictionary) {
      auto names = static_cast<PdsDictionary*>(obj)->GetArray(L"Names");
      if (names) {
        for (int i = 0; i < names->GetNumObjects(); i+=2) {
          ProcessNamedDest(names->Get(i), names->Get(i+1), doc, json);
        }
        return;
      }
      auto kids = static_cast<PdsDictionary*>(obj)->GetArray(L"Kids");
      if (kids) {
        for (int i = 0; i < kids->GetNumObjects(); i++) {
          ProcessNameTreeObject(kids->GetDictionary(i), doc, json);
        }
      }
    }
  }

  // Extract all documents bookmars into json.
  void Run(
    const std::wstring& open_path,                       // source PDF document
    std::ostream& output                                 // output stream
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();

    ptree dests_root;

    PdfNameTree* name_tree = doc->GetNameTree(L"Dests", false);
    if (name_tree) {
      auto name_tree_obj = name_tree->GetObject();
      ProcessNameTreeObject(name_tree_obj, doc, dests_root);
    }

    ptree output_json;
    output_json.add_child("dests", dests_root);

    write_json(output, output_json, true);

    doc->Close();
    pdfix->Destroy();
  }
}
//! [NamedDestsToJson_cpp]
