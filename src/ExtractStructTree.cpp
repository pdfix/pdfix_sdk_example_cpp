////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractData.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
// project
#include "Pdfix.h"

namespace ExtractData {

  void ExtractStructElement(PdsStructElement* elem, ptree &node, const DataType& data_types) {
    node.put("type", EncodeText(elem->GetType(true)));
    node.put("title", EncodeText(elem->GetTitle()));
    node.put("lang", EncodeText(elem->GetLang()));
    node.put("id", EncodeText(elem->GetId()));
    node.put("alt", EncodeText(elem->GetAlt()));
    node.put("actual_text", EncodeText(elem->GetActualText()));

              // test
          elem->SetActualText(L"actual_text");
          elem->SetAlt(L"alt");
          elem->SetId(L"id");


    ptree kids_node;
    for (int i = 0; i < elem->GetNumKids(); i++) {
      ptree kid_node;
      switch (elem->GetKidType(i)) {
        case kPdsStructKidElement: {
          // structure element reference
          auto kid_obj = elem->GetKidObject(i);  
          ExtractStructObject(elem->GetStructTree(), kid_obj, kid_node, data_types);
          break;
        }
        case kPdsStructKidStreamContent:
        case kPdsStructKidObject: {
          // object reference
          auto kid_obj = elem->GetKidObject(i);
          kid_node.put("obj", kid_obj->GetId());
          break;
        }
        case kPdsStructKidPageContent: {
          // content element reference
          kid_node.put("mcid", elem->GetKidMcid(i));
          kid_node.put("page_num", elem->GetKidPageNumber(i));
          break;
        }
        default:; // unknown/invalid reference
      }
      kids_node.push_back(std::make_pair("", kid_node));
    }
    if (!kids_node.empty())
      node.put_child("kids", kids_node);
  }

  void ExtractStructObject(PdsStructTree* struct_tree, PdsObject* object, ptree &node, 
    const DataType& data_types) {
    auto struct_elem_deleter = [&](PdsStructElement *elem) { elem->Release(); };
    auto elem = std::unique_ptr<PdsStructElement, 
          decltype(struct_elem_deleter)>(struct_tree->AcquireStructElement(object), struct_elem_deleter);
    if (!elem)
      throw PdfixException();
    ExtractStructElement(elem.get(), node, data_types);
  }

  void ExtractStructTree(PdsStructTree* struct_tree, ptree &node, const DataType& data_types) {
    ptree kids_node;
    for (int i = 0; i < struct_tree->GetNumKids(); i++) {
      auto struct_elem_obj = struct_tree->GetKidObject(i);
      ptree kid_node;
      ExtractStructObject(struct_tree, struct_elem_obj, kid_node, data_types);
      kids_node.push_back(std::make_pair("", kid_node));
    }
    if (!kids_node.empty())
      node.put_child("kids", kids_node);
  }
}