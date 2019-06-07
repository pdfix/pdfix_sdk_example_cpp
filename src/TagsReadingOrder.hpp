////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadingOrder.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage TagsReadingOrder_cpp
*/
/*!
\page TagsReadingOrder_cpp Edit structure tree reading order
Example how to manually modify order of struct elements in a structure tree.
\snippet /TagsReadingOrder.hpp TagsReadingOrder_cpp
*/

#pragma once

//! [TagsReadingOrder_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

namespace TagsReadingOrder {

  auto struct_elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
  typedef std::unique_ptr<PdsStructElement, decltype(struct_elem_deleter)> PdsStructElementP;

  //////////////////////////////////////////////////////////////////////////////////////////////////
  // TagParagraphAsHeading
  // re-tag the struct element to heading based on font properties
  //////////////////////////////////////////////////////////////////////////////////////////////////
  PdsStructElementP GetFirstParagraph(PdsStructElement* struct_elem) {
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
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // TagsReadingOrder
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
    PdsStructElementP paragraph = GetFirstParagraph(struct_tree);
    if (!paragraph)
      throw std::runtime_error("No paragraph found.");

    // move paragraph to the back of it's parent
    PdsStructElementP parent(struct_tree->AcquireStructElement(paragraph->GetParentObject()),
      struct_elem_deleter);
    if (!parent)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    
    if (!paragraph->SetParent(parent.get(), parent->GetNumKids()-1))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    // save document
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    
    doc->Close();
    pdfix->Destroy();
  }
  
} // namespace TagsReadingOrder
//! [TagsReadingOrder_cpp]
