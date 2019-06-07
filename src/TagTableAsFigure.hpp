////////////////////////////////////////////////////////////////////////////////////////////////////
// TagTableAsFigure.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage TagTableAsFigure_cpp
*/
/*!
\page TagTableAsFigure_cpp Tag elements as Figure
Example how to manually change table structure element to a figure using initial elements.
\snippet /TagTableAsFigure.hpp TagTableAsFigure_cpp
*/

#pragma once

//! [TagTableAsFigure_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

namespace TagTableAsFigure {

  auto struct_elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
  typedef std::unique_ptr<PdsStructElement, decltype(struct_elem_deleter)> PdsStructElementP;
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // GetParagraphBBox
  // get the text state of the text objects inside paragraph by iterating content kid objects
  //////////////////////////////////////////////////////////////////////////////////////////////////
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
            PdfRect elem_bbox;
            page_object->GetBBox(&elem_bbox);
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

  //////////////////////////////////////////////////////////////////////////////////////////////////
  // TagParagraphAsHeading
  // re-tag the struct element to heading based on font properties
  //////////////////////////////////////////////////////////////////////////////////////////////////
  PdsStructElementP GetFirstTable(PdsStructElement* struct_elem) {
    // search kid struct elements
    for (int i = 0; i < struct_elem->GetNumKids(); i++) {
      if (struct_elem->GetKidType(i) == kPdsStructKidElement) {
        PdsObject* kid_obj = struct_elem->GetKidObject(i);
        PdsStructElementP kid_elem(struct_elem->GetStructTree()->AcquireStructElement(kid_obj),
                                   struct_elem_deleter);
        if (!kid_elem)
          throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
        
        std::wstring type;
        type.resize(kid_elem->GetType(true, nullptr, 0));
        kid_elem->GetType(true, (wchar_t*)type.c_str(), (int)type.size());
        if (type == L"Table")
          return kid_elem;

        auto paragraph = GetFirstTable(kid_elem.get());
        if (paragraph)
          return paragraph;
      }
    }
    return PdsStructElementP(nullptr, struct_elem_deleter);
  }

  PdsStructElementP GetFirstTable(PdsStructTree* struct_tree) {
    for (int i = 0; i < struct_tree->GetNumKids(); i++) {
      PdsObject* kid_obj = struct_tree->GetKidObject(i);
      auto elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
      std::unique_ptr<PdsStructElement, decltype(elem_deleter)>
        kid_elem(struct_tree->AcquireStructElement(kid_obj), elem_deleter);
      auto paragraph = GetFirstTable(kid_elem.get());
      if (paragraph)
        return paragraph;
    }
    return PdsStructElementP(nullptr, struct_elem_deleter);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // TagTableAsFigure
  // add annotation on the page and add it to the struct tree
  //////////////////////////////////////////////////////////////////////////////////////////////////
  void Run(
    const std::wstring& email,            // authorization email
    const std::wstring& license_key,      // authorization license key
    const std::wstring& open_path,        // source PDF document
    const std::wstring& save_path         // output PDF document
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
    
    // cleanup any previous structure tree
    if (!doc->RemoveTags(nullptr, nullptr))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    
    // autotag document first
    if (!doc->AddTags(nullptr, nullptr))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    // get the struct tree
    auto struct_tree = doc->GetStructTree();
    if (!struct_tree)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    // get the first P struct element on the first page
    PdsStructElementP table = GetFirstTable(struct_tree);
    if (!table)
      throw std::runtime_error("No table found.");

    // get table bounding box
    PdfRect bbox;
    GetStructElementBBox(table.get(), bbox);
    
    // remove all items from the table to make it untagged cotnent
    for (int i = table->GetNumKids() - 1; i >=0; i--)
      table->RemoveKid(i);
    
    PdfDocTemplate* doc_tmpl = doc->GetDocTemplate();
    
    // tag page
    auto page_deleter = [](PdfPage* page) {page->Release();};
    std::unique_ptr<PdfPage, decltype(page_deleter)> page(doc->AcquirePage(0), page_deleter);

    PdePageMap* page_map = page->CreatePageMap();
    PdeElement* elem = page_map->CreateElement(kPdeImage, nullptr);
    elem->SetBBox(&bbox);
    elem->SetAlt(L"This is image caption");
    
    doc_tmpl->SetProperty(L"ignore_tags", 1);

    if (!page_map->AcquireElements(nullptr, nullptr))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    if (!page_map->AddTags(table.get(), nullptr, nullptr))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    if (!table->SetType(L"Sect"))
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    // save document
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    
    doc->Close();
    pdfix->Destroy();
  }
  
} // namespace TagTableAsFigure
//! [TagTableAsFigure_cpp]
