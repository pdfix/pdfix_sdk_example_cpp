////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadStructTree.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);

void ProcessStructElement(PdsStructTree* struct_tree, PdsStructElement* struct_elem,
  ptree& json) {

  if (!struct_elem)
    throw PdfixException();

  auto type_str = struct_elem->GetType(true);
  json.put("type", ToUtf8(type_str));

  auto obj_id = struct_elem->GetElementObject()->GetId();
  json.put("object-id", obj_id);
 
  auto title_str = struct_elem->GetTitle();
  if ( title_str.length())
    json.put("title", ToUtf8(title_str));

  auto actual_text_str = struct_elem->GetActualText();
  if ( actual_text_str.length())
    json.put("actual_text", ToUtf8(actual_text_str));

  auto alt_str = struct_elem->GetAlt();
  if ( alt_str.length())
    json.put("alt", ToUtf8(alt_str));

  auto page_num = struct_elem->GetPageNumber();
  if ( page_num != -1 )
    json.put("page_num", page_num + 1);
  
  ptree kids;

  int num_kids = struct_elem->GetNumChildren();
  for (int i = 0; i < num_kids; i++) {
    auto kid_obj = struct_elem->GetChildObject(i);
    // based on structure element you can obtain different data
    switch (struct_elem->GetChildType(i)) {
    case kPdsStructChildElement: {
      auto kid_struct_elem = struct_tree->GetStructElementFromObject(kid_obj);
      if (kid_struct_elem == nullptr)
        throw PdfixException();
      ptree kid_json;
      ProcessStructElement(struct_tree, kid_struct_elem, kid_json);
      kids.push_back(std::make_pair("", kid_json));
    } break;
    case kPdsStructChildObject: 
      break;
    case kPdsStructChildStreamContent: {
      auto kid_page_num = struct_elem->GetChildPageNumber(i);
      ptree kid_json;
      if ( kid_page_num != -1 )
        kid_json.put("page_num", kid_page_num + 1);
      auto mcid = struct_elem->GetChildMcid(i);
      kid_json.put("mcid", mcid);
      kids.push_back(std::make_pair("", kid_json));
      } break;
    case kPdsStructChildPageContent: {
      auto mcid = struct_elem->GetChildMcid(i);
      ptree kid_json;
      kid_json.put("mcid", mcid);
      kids.push_back(std::make_pair("", kid_json));
      } break;
    }
  }
  json.add_child("kids", kids);
}

void TagsReadStructTree(
  const std::wstring& open_path,        // source PDF document
  std::ostream& output                  // output stream
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

  ptree struct_tree_root_json;
  ptree kids;
  // read document structure tree
  PdsStructTree* struct_tree = doc->GetStructTree();
  if (!struct_tree)
    std::cout << "No Tags available" << std::endl;
  else {
    int num_kids = struct_tree->GetNumChildren();
    for (auto i = 0; i < num_kids; i++) {
      PdsObject* kid_object = struct_tree->GetChildObject(i);
      ptree kid_json;
      PdsStructElement* struct_elem = struct_tree->GetStructElementFromObject(kid_object);
      ProcessStructElement(struct_tree, struct_elem, kid_json);
      kids.push_back(std::make_pair("", kid_json));
    }
  }
  struct_tree_root_json.add_child("kids", kids);
  
  ptree json;
  json.add_child("struct-tree", struct_tree_root_json);
  write_json(output, json, false);

  doc->Close();
  pdfix->Destroy();
}