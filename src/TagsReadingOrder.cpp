////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadingOrder.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace TagsReadingOrder {

bool MoveParagraphToParent(PdsStructElement* struct_elem) {
  // search kid struct elements
  for (int i = 0; i < struct_elem->GetNumChildren(); i++) {
    if (struct_elem->GetChildType(i) == kPdsStructChildElement) {
      PdsObject* kid_obj = struct_elem->GetChildObject(i);
      PdsStructElement* kid_elem =
          struct_elem->GetStructTree()->GetStructElementFromObject(kid_obj);
      if (!kid_elem)
        throw PdfixException();

      auto type = kid_elem->GetType(true);
      if (type == L"P") {
        if (!struct_elem->MoveChild(i, struct_elem, struct_elem->GetNumChildren() - 1))
          throw PdfixException();
        return true;
      }
      return MoveParagraphToParent(kid_elem);
    }
  }
  return false;
}

bool MoveParagraphToParent(PdsStructTree* struct_tree) {
  for (int i = 0; i < struct_tree->GetNumChildren(); i++) {
    PdsObject* kid_obj = struct_tree->GetChildObject(i);
    PdsStructElement* kid_elem = struct_tree->GetStructElementFromObject(kid_obj);
    return MoveParagraphToParent(kid_elem);
  }
  return false;
}

void Run(const std::wstring& open_path,  // source PDF document
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

  // move paragraph to the back of it's parent
  if (!MoveParagraphToParent(struct_tree))
    throw std::runtime_error("No paragraph found.");

  // save document
  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}

}  // namespace TagsReadingOrder
