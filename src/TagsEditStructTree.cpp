////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsEditStructTree.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

// Find struct element by the name in the structure element
PdsStructElement* FindStructElement(PdsStructElement* struct_elem,
                                    const std::wstring& type,
                                    const std::wstring& title = L"") {
  // find object inside of the struct element with specified name
  auto struct_tree = struct_elem->GetStructTree();
  auto num_kids = struct_elem->GetNumChildren();
  for (int i = 0; i < num_kids; i++) {
    auto kid_type = struct_elem->GetChildType(i);
    if (kid_type != kPdsStructChildElement)
      continue;
    PdsObject* kid_obj = struct_elem->GetChildObject(i);
    if (!kid_obj)
      throw PdfixException();

    PdsStructElement* kid_elem = struct_tree->GetStructElementFromObject(kid_obj);

    // get struct element type and title
    std::wstring type_str(type), title_str(title);
    kid_elem->GetType(true, (wchar_t*)type_str.c_str(), (int)type_str.length());
    kid_elem->GetTitle((wchar_t*)title_str.c_str(), (int)title_str.length());

    // compare type and title
    if (type_str == type && (title.empty() || title_str == title))
      return kid_elem;

    // find in kid's elements
    if (auto found = FindStructElement(kid_elem, type, title))
      return found;
  }
  return nullptr;
}

// Find struct element by the name in the structure tree
PdsStructElement* FindStructElement(PdsStructTree* struct_tree,
                                    const std::wstring& name,
                                    const std::wstring& title = L"") {
  // find object inside of the struct tree
  for (int i = 0; i < struct_tree->GetNumChildren(); i++) {
    auto kid_obj = struct_tree->GetChildObject(i);
    if (!kid_obj)
      throw PdfixException();

    PdsStructElement* kid_elem = struct_tree->GetStructElementFromObject(kid_obj);
    if (!kid_elem)
      throw PdfixException();
    if (auto found = FindStructElement(kid_elem, name, title))
      return found;
  }
  return nullptr;
}

// Change the tag name of the first table row from <td> to <th>
void TableTagRowHeader(PdsStructElement* table) {
  auto struct_tree = table->GetStructTree();
  // get the first row td elements
  auto tr = FindStructElement(table, L"TR");
  for (int i = 0; i < tr->GetNumChildren(); i++) {
    if (tr->GetChildType(i) == kPdsStructChildElement) {
      PdsObject* td_obj = tr->GetChildObject(i);
      if (!td_obj)
        throw PdfixException();

      PdsStructElement* td = struct_tree->GetStructElementFromObject(td_obj);

      std::wstring type(L"TD");
      td->GetType(true, (wchar_t*)type.c_str(), (int)type.length());

      if (type == L"TD") {
        // rename to th
        td->SetType(L"TH");
      }
    }
  }
}

void FigureTagSetAttributes(PdsStructElement* figure) {
  // TODO: add some other attributes
  figure->SetAlt(L"This is new image alternate text");
}

void TagsEditStructTree(const std::wstring& open_path,  // source PDF document
                        const std::wstring& save_path   // output PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  // cleanup any previous structure tree
  if (!doc->RemoveTags())
    throw PdfixException();

  // autotag document first
  PdfTagsParams params;
  if (!doc->AddTags(&params))
    throw PdfixException();

  // get the struct tree
  auto struct_tree = doc->GetStructTree();
  if (!struct_tree)
    throw PdfixException();

  // rename tags
  auto table_elem = FindStructElement(struct_tree, L"Table");
  if (table_elem)
    TableTagRowHeader(table_elem);

  // set tag attributes
  auto image_elem = FindStructElement(struct_tree, L"Figure");
  if (image_elem)
    FigureTagSetAttributes(image_elem);

  // change reading order

  // make top and bottom element on page an artifact (header, footer)
  auto page2 = FindStructElement(struct_tree, L"NonStruct", L"Page 2");

  // re-tag only one page
  auto page3 = FindStructElement(struct_tree, L"NonStruct", L"Page 3");

  // create struct element
  // add annot, add page object

  // reconstruct parent tree
  if (!struct_tree->FixParentTree())
    throw PdfixException();

  // save document
  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
