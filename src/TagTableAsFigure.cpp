////////////////////////////////////////////////////////////////////////////////////////////////////
// TagTableAsFigure.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <memory>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace TagTableAsFigure {

bool GetStructElementBBox(PdsStructElement* struct_elem, PdfRect& bbox) {
  bool result = false;
  for (int i = 0; i < struct_elem->GetNumChildren(); i++) {
    if (struct_elem->GetChildType(i) == kPdsStructChildPageContent) {
      // acquire page on which the element is present
      PdfDoc* doc = struct_elem->GetStructTree()->GetDoc();
      auto page_deleter = [](PdfPage* page) { page->Release(); };
      std::unique_ptr<PdfPage, decltype(page_deleter)> page(
          doc->AcquirePage(struct_elem->GetChildPageNumber(i)), page_deleter);

      // find text object with mcid on the page to get the text state
      int mcid = struct_elem->GetChildMcid(i);
      auto content = page->GetContent();
      for (int j = 0; j < content->GetNumObjects(); j++) {
        PdsPageObject* page_object = content->GetObject(j);

        // check if this text page object has the same mcid
        PdsContentMark* content_mark = page_object->GetContentMark();
        if (content_mark && content_mark->GetTagMcid() == mcid) {
          PdfRect elem_bbox = page_object->GetBBox();
          if ((bbox.left - bbox.right == 0) || (bbox.top - bbox.bottom == 0))
            bbox = elem_bbox;
          else {
            bbox.left = std::min(bbox.left, elem_bbox.left);
            bbox.right = std::max(bbox.right, elem_bbox.right);
            bbox.top = std::max(bbox.top, elem_bbox.top);
            bbox.bottom = std::min(bbox.bottom, elem_bbox.bottom);
          }
          result = true;
        }
      }
    } else if (struct_elem->GetChildType(i) == kPdsStructChildElement) {
      PdsObject* kid_obj = struct_elem->GetChildObject(i);
      PdsStructElement* kid_elem =
          struct_elem->GetStructTree()->GetStructElementFromObject(kid_obj);
      GetStructElementBBox(kid_elem, bbox);
    }
  }
  return result;
}

PdsStructElement* GetFirstTable(PdsStructElement* struct_elem) {
  // search kid struct elements
  for (int i = 0; i < struct_elem->GetNumChildren(); i++) {
    if (struct_elem->GetChildType(i) == kPdsStructChildElement) {
      PdsObject* kid_obj = struct_elem->GetChildObject(i);
      PdsStructElement* kid_elem =
          struct_elem->GetStructTree()->GetStructElementFromObject(kid_obj);
      if (!kid_elem)
        throw PdfixException();

      auto type = kid_elem->GetType(true);
      if (type == L"Table")
        return kid_elem;

      auto paragraph = GetFirstTable(kid_elem);
      if (paragraph)
        return paragraph;
    }
  }
  return nullptr;
}

PdsStructElement* GetFirstTable(PdsStructTree* struct_tree) {
  for (int i = 0; i < struct_tree->GetNumChildren(); i++) {
    PdsObject* kid_obj = struct_tree->GetChildObject(i);
    auto kid_elem = struct_tree->GetStructElementFromObject(kid_obj);
    auto paragraph = GetFirstTable(kid_elem);
    if (paragraph)
      return paragraph;
  }
  return nullptr;
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

  // get the first P struct element on the first page
  PdsStructElement* table = GetFirstTable(struct_tree);
  if (!table)
    throw std::runtime_error("No table found.");

  // get table bounding box
  PdfRect bbox;
  GetStructElementBBox(table, bbox);

  // remove all items from the table to make it untagged cotnent
  for (int i = table->GetNumChildren() - 1; i >= 0; i--)
    table->RemoveChild(i);

  PdfDocTemplate* doc_tmpl = doc->GetTemplate();

  // tag page
  auto page_deleter = [](PdfPage* page) { page->Release(); };
  std::unique_ptr<PdfPage, decltype(page_deleter)> page(doc->AcquirePage(0), page_deleter);

  PdePageMap* page_map = page->AcquirePageMap();
  if (page_map->CreateElements())
    throw PdfixException();

  PdeElement* elem = page_map->CreateElement(kPdeImage, nullptr);
  elem->SetBBox(&bbox);
  elem->SetAlt(L"This is image caption");

  doc_tmpl->SetProperty(L"ignore_tags", 1);

  if (!page_map->CreateElements())
    throw PdfixException();

  if (!page_map->AddTags(table, false, &params))
    throw PdfixException();

  if (!table->SetType(L"Sect"))
    throw PdfixException();

  page_map->Release();

  // save document
  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}

}  // namespace TagTableAsFigure
