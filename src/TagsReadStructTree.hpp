////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadStructTree.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage GetTags_cpp
*/
/*!
\page GetTags_cpp Get Tags Sample
Example how to automatically get tags from the document.
\snippet /GetTags.hpp GetTags_cpp
*/

#pragma once

//! [GetTags_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

extern std::string ToUtf8(const std::wstring& wstr);

void ProcessStructElement(PdsStructTree* struct_tree, PdsStructElement* struct_elem,
  std::string indent, std::ostream& ss) {
  indent += " ";
  Pdfix* pdfix = GetPdfix();
  if (!struct_elem)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  std::wstring type_str;
  type_str.resize(struct_elem->GetType(true, nullptr, 0));
  struct_elem->GetType(true, (wchar_t*)type_str.c_str(), (int)type_str.size());
  ss << indent << ToUtf8(type_str) << std::endl;

  std::wstring tile_str;
  tile_str.resize(struct_elem->GetTitle(nullptr, 0));
  struct_elem->GetTitle((wchar_t*)tile_str.c_str(), (int)tile_str.size());
  ss << indent << ToUtf8(tile_str) << std::endl;

  std::wstring actual_text_str;
  actual_text_str.resize(struct_elem->GetActualText(nullptr, 0));
  struct_elem->GetActualText((wchar_t*)actual_text_str.c_str(), (int)actual_text_str.size());
  ss << indent << ToUtf8(actual_text_str) << std::endl;

  std::wstring alt_str;
  alt_str.resize(struct_elem->GetAlt(nullptr, 0));
  struct_elem->GetAlt((wchar_t*)alt_str.c_str(), (int)alt_str.size());
  ss << indent << ToUtf8(alt_str) << std::endl;

  auto page_num = struct_elem->GetPageNumber();
  ss << indent << "Page number: " << page_num << std::endl;

  int num_kids = struct_elem->GetNumKids();
  for (int i = 0; i < num_kids; i++) {
    auto kid_obj = struct_elem->GetKidObject(i);
    // based on structure element you can obtain different data
    switch (struct_elem->GetKidType(i)) {
    case kPdsStructKidElement: {
      auto kid_struct_elem = struct_tree->AcquireStructElement(kid_obj);
      if (kid_struct_elem == nullptr)
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
      ProcessStructElement(struct_tree, kid_struct_elem, indent, ss);
      kid_struct_elem->Release();
    } break;
    case kPdsStructKidObject: 
      break;
    case kPdsStructKidStreamContent: {
      auto kid_page_num = struct_elem->GetKidPageNumber(i);
      ss << indent << "Kid Page number: " << kid_page_num << std::endl;
      auto mcid = struct_elem->GetKidMcid(i);
      ss << indent << "MCID: " << mcid << std::endl;
      } break;
    case kPdsStructKidPageContent: {
      auto mcid = struct_elem->GetKidMcid(i);
      ss << indent << "MCID: " << mcid << std::endl;
      } break;
    }
  }
}

void TagsReadStructTree(
  const std::wstring& email,            // authorization email
  const std::wstring& license_key,      // authorization license key
  const std::wstring& open_path,        // source PDF document
  const std::wstring& save_path,        // output TXT document
  const std::wstring& config_path       // configuration file
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  std::ofstream ofs;
  ofs.open(ToUtf8(save_path));

  // read document structure tree
  PdsStructTree* struct_tree = doc->GetStructTree();
  if (!struct_tree)
    std::cout << "No Tags available" << std::endl;
  else {
    int num_kids = struct_tree->GetNumKids();
    for (auto i = 0; i < num_kids; i++) {
      PdsObject* kid_object = struct_tree->GetKidObject(i);
      PdsStructElement* struct_elem = struct_tree->AcquireStructElement(kid_object);
      ProcessStructElement(struct_tree, struct_elem, "", ofs);
      struct_elem->Release();
    }
  }

  ofs.close();

  doc->Close();
  pdfix->Destroy();
}
//! [GetTags_cpp]
