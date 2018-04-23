////////////////////////////////////////////////////////////////////////////////////////////////////
// GetBookmarks.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage GetBookmarks_cpp
*/
/*!
\page GetBookmarks_cpp Get Bookmarks Sample
Example how to read all documents bookmarks.
\snippet /GetBookmarks.hpp GetBookmarks_cpp
*/

#pragma once

//! [GetBookmarks_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

// ProcessBookmark gets the title of the bookmark if it's not a root.
void ProcessBookmark(PdfBookmark* bmk, std::wstring indent) {
  if (bmk->GetParent()) {
    std::wstring title;
    title.resize(bmk->GetTitle(nullptr, 0));
    bmk->GetTitle((wchar_t*)title.c_str(), title.size());
    std::wcout << indent + title << std::endl;
  }
  indent += L"  ";
  int num = bmk->GetNumChildren();
  if (num > 0) {
    for (int i = 0; i < num; i++) {
      PdfBookmark* child = bmk->GetChild(i);
      ProcessBookmark(child, indent);
    }
  }
}

// Iterates all documents bookmars.
void GetBookmarks(
  std::wstring email,                           // authorization email   
  std::wstring license_key,                     // authorization license key
  std::wstring open_path                        // source PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw pdfix->GetError();

  PdfBookmark* parent = doc->GetBookmarkRoot();
  if (!parent)
    throw std::runtime_error("Document has no bookmarks!");
  ProcessBookmark(parent, L"");

  doc->Close();
  pdfix->Destroy();
}
//! [GetBookmarks_cpp]