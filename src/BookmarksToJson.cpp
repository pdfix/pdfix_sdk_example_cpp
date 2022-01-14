////////////////////////////////////////////////////////////////////////////////////////////////////
// BookmarksToJson.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/BookmarksToJson.h"

// system
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::string ToUtf8(const std::wstring& wstr);

namespace BookmarksToJson {
  void ProcessAction(PdfAction* action, PdfDoc* doc, ptree& json) {
    
    switch (action->GetSubtype()) {
      case kActionURI: {
        std::wstring uri = action->GetDestFile();
        json.put("type", "URI");
        json.put("uri", ToUtf8(uri));
        break;
      }
      case kActionGoTo: {
        json.put("type", "GoTo");
        auto view_dest = action->GetViewDestination();
        if (view_dest != nullptr)
          json.put("page_num", view_dest->GetPageNum(doc) + 1);
        break;
      }
      default:;
    }
  }

  // ProcessBookmark gets the title of the bookmark if it's not a root.
  void ProcessBookmark(PdfBookmark* bmk, PdfDoc* doc, ptree& json) {
    // clear processed bookmarks
    if (bmk->GetParent() == nullptr)
      processed_bookmarks.clear();
   
    auto bmk_obj = bmk->GetObject();
    // skip processed bookmark
    if (processed_bookmarks.count(bmk_obj) > 0)
      return;
    processed_bookmarks.insert(bmk_obj);

    if (bmk->GetParent()) {
      // bookmark title
      json.put("title", ToUtf8(bmk->GetTitle()));

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
        auto child = bmk->GetChild(i);
        if (!child)
          throw PdfixException();
        ProcessBookmark(child, doc, bmk_json);
        kids.push_back(std::make_pair("", bmk_json));
      }
      json.add_child("kids", kids);
    }
  }

  // Extract all documents bookmars into json.
  void Run(
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& password,                        // open document password
    std::ostream& output                                 // output stream
  ) {
    auto pdfix = PdfixEngine::Get();

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
    if (!doc)
      throw PdfixException();

    ptree bookmark_root;

    PdfBookmark* parent = doc->GetBookmarkRoot();
    if (parent) {
      ProcessBookmark(parent, doc, bookmark_root);
    }

    ptree output_json;
    output_json.add_child("outlines", bookmark_root);

    write_json(output, output_json, false);

    doc->Close();
  }
}