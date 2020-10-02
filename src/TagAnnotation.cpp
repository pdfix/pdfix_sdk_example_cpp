////////////////////////////////////////////////////////////////////////////////////////////////////
// TagAnnotation.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage TagAnnotation_cpp
*/
/*!
\page TagAnnotation_cpp Tag annotation added on a page
Example how to manually tag link annotation added on top of the text object.
\snippet /TagAnnotation.hpp TagAnnotation_cpp
*/

#pragma once

//! [TagAnnotation_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace TagAnnotation {

  auto struct_elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
  typedef std::unique_ptr<PdsStructElement, decltype(struct_elem_deleter)> PdsStructElementP;

  // get the text state of the text objects inside paragraph by iterating content kid objects
  bool GetStructElementBBox(PdsStructElement* struct_elem, PdfRect& bbox) {
    bool result = false;
    for (int i = 0; i < struct_elem->GetNumKids(); i++) {
      if (struct_elem->GetKidType(i) == kPdsStructKidPageContent) {
        // acquire page on which the element is present
        PdfDoc* doc = struct_elem->GetStructTree()->GetDoc();
        auto page_deleter = [](PdfPage* page) { page->Release(); };
        std::unique_ptr<PdfPage, decltype(page_deleter)>
          page(doc->AcquirePage(struct_elem->GetKidPageNumber(i)), page_deleter);
        
        // find text object with mcid on the page to get the text state
        int mcid = struct_elem->GetKidMcid(i);
        for (int j = 0; j < page->GetNumPageObjects(); j++) {
          PdsPageObject* page_object = page->GetPageObject(j);
          
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
      }
      else if (struct_elem->GetKidType(i) == kPdsStructKidElement) {
        PdsObject* kid_obj = struct_elem->GetKidObject(i);
        PdsStructElementP kid_elem(struct_elem->GetStructTree()->AcquireStructElement(kid_obj),
          struct_elem_deleter);
        GetStructElementBBox(kid_elem.get(), bbox);
      }
    }
    return result;
  }

  // get reference to the first paragraph on the page
  PdsStructElementP GetFirstParagraph(PdsStructElement* struct_elem) {
    // search kid struct elements
    for (int i = 0; i < struct_elem->GetNumKids(); i++) {
      if (struct_elem->GetKidType(i) == kPdsStructKidElement) {
        PdsObject* kid_obj = struct_elem->GetKidObject(i);
        PdsStructElementP kid_elem(struct_elem->GetStructTree()->AcquireStructElement(kid_obj),
                                   struct_elem_deleter);
        if (!kid_elem)
          throw PdfixException();
        
        auto type = kid_elem->GetType(true);
        kid_elem->GetType(true, (wchar_t*)type.c_str(), (int)type.size());
        if (type == L"P")
          return kid_elem;

        auto paragraph = GetFirstParagraph(kid_elem.get());
        if (paragraph)
          return paragraph;
      }
    }
    return PdsStructElementP(nullptr, struct_elem_deleter);
  }

  PdsStructElementP GetFirstParagraph(PdsStructTree* struct_tree) {
    for (int i = 0; i < struct_tree->GetNumKids(); i++) {
      PdsObject* kid_obj = struct_tree->GetKidObject(i);
      auto elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
      std::unique_ptr<PdsStructElement, decltype(elem_deleter)>
        kid_elem(struct_tree->AcquireStructElement(kid_obj), elem_deleter);
      auto paragraph = GetFirstParagraph(kid_elem.get());
      if (paragraph)
        return paragraph;
    }
    return PdsStructElementP(nullptr, struct_elem_deleter);
  }

  // add annotation on the page and add it to the struct tree
  void Run(
    const std::wstring& open_path,        // source PDF document
    const std::wstring& save_path         // output PDF document
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
    
    // cleanup any previous structure tree
    if (!doc->RemoveTags(nullptr, nullptr))
      throw PdfixException();
    
    // autotag document first
    if (!doc->AddTags(nullptr, nullptr))
      throw PdfixException();

    // get the struct tree
    auto struct_tree = doc->GetStructTree();
    if (!struct_tree)
      throw PdfixException();

    // get the first P struct element on the first page
    PdsStructElementP paragraph = GetFirstParagraph(struct_tree);
    if (!paragraph)
      throw std::runtime_error("No paragraph found.");

    PdfRect annot_bbox;
    GetStructElementBBox(paragraph.get(), annot_bbox);
    
    // add new link annotation to the page
    auto page_deleter = [](PdfPage* page) { page->Release(); };
    std::unique_ptr<PdfPage, decltype(page_deleter)> page(doc->AcquirePage(0), page_deleter);
    PdfLinkAnnot* annot = page->AddNewLinkAnnot(0, &annot_bbox);
    if (!annot)
      throw PdfixException();
    
    // re-tag the document the link annotation
    if (!doc->RemoveTags(nullptr, nullptr))
      throw PdfixException();
    if (!doc->AddTags(nullptr, nullptr))
      throw PdfixException();

    // save document
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();
    
    doc->Close();
    pdfix->Destroy();
  }
  
} // namespace TagAnnotation
//! [TagAnnotation_cpp]
