////////////////////////////////////////////////////////////////////////////////////////////////////
// BookmarksToJson.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage BookmarksToJson_cpp
*/
/*!
\page BookmarksToJson_cpp Save Bookmarks to JSON Sample
Example how to extract all documents bookmarks into json.
\snippet /GetBookmarks.hpp GetBookmarks_cpp
*/

#pragma once

//! [BookmarksToJson_cpp]
// system
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "Pdfix.h"

using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);

void ProcessAction(PdfAction* action, PdfDoc* doc, ptree& json) {
  
  switch (action->GetSubtype()) {
    case kActionURI: {
      std::wstring uri;
      uri.resize(action->GetURI(nullptr, 0));
      action->GetURI((wchar_t*)uri.c_str(), (int)uri.size());
      json.put("type", "URI");
      json.put("uri", ToUtf8(uri));
      break;
    }
    case kActionGoTo:
      json.put("type", "GoTo");
      json.put("page_num", action->GetDestPageNum(doc) + 1);
      break;
    default:;
  }
}

// ProcessBookmark gets the title of the bookmark if it's not a root.
void ProcessBookmark(PdfBookmark* bmk, PdfDoc* doc, ptree& json) {
  // skip bookmark root (no parent)
  if (bmk->GetParent()) {

    // bookmark title
    std::wstring title;
    title.resize(bmk->GetTitle(nullptr, 0));
    bmk->GetTitle((wchar_t*)title.c_str(), (int)title.size());

    json.put("title", ToUtf8(title));

    // action
    PdfAction* action = bmk->GetAction();
    if (action) {
      ptree action_json;
      ProcessAction(action, doc, action_json);
      json.add_child("action", action_json);
    }
  }

  // kids
  int num = bmk->GetNumChildren();
  if (num > 0) {
    ptree kids;
    for (int i = 0; i < num; i++) {
      ptree bmk_json;
      PdfBookmark* child = bmk->GetChild(i);
      ProcessBookmark(child, doc, bmk_json);

      kids.push_back(std::make_pair("", bmk_json));
    }
    json.add_child("kids", kids);
  }
}

// Extract all documents bookmars into json.
void BookmarksToJson(
  const std::wstring& email,                           // authorization email   
  const std::wstring& license_key,                     // authorization license key
  const std::wstring& open_path,                       // source PDF document
  const std::wstring& save_path                        // path to output json
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::to_string(GetPdfix()->GetErrorType());
  
  ptree bookmark_root;

  PdfBookmark* parent = doc->GetBookmarkRoot();
  if (parent) {
    ProcessBookmark(parent, doc, bookmark_root);
  }

  ptree output_json;
  output_json.add_child("bookmark_root", bookmark_root);

  std::ofstream ofs(ToUtf8(save_path));
  if (!ofs.is_open())
    throw std::runtime_error("Failed to open output file (" + std::to_string(errno) + ")");
  write_json(ofs, output_json);

  doc->Close();
  pdfix->Destroy();
}
//! [BookmarksToJson_cpp]
