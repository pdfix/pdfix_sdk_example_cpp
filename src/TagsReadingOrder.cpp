////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadingOrder.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace TagsReadingOrder {

  auto struct_elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
  typedef std::unique_ptr<PdsStructElement, decltype(struct_elem_deleter)> PdsStructElementP;

  PdsStructElement* GetFirstParagraph(PdsStructElement* struct_elem) {
    // search kid struct elements
    for (int i = 0; i < struct_elem->GetNumKids(); i++) {
      if (struct_elem->GetKidType(i) == kPdsStructKidElement) {
        PdsObject* kid_obj = struct_elem->GetKidObject(i);
        PdsStructElement* kid_elem = struct_elem->GetStructTree()->GetStructElement(kid_obj);
        if (!kid_elem)
          throw PdfixException();
        
        auto type = kid_elem->GetType(true);
        if (type == L"P")
          return kid_elem;

        auto paragraph = GetFirstParagraph(kid_elem.get());
        if (paragraph)
          return paragraph;
      }
    }
    return nullptr;
    //return PdsStructElementP(nullptr, struct_elem_deleter);
  }

  PdsStructElement* GetFirstParagraph(PdsStructTree* struct_tree) {
    for (int i = 0; i < struct_tree->GetNumKids(); i++) {
      PdsObject* kid_obj = struct_tree->GetKidObject(i);
      auto elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
      //std::unique_ptr<PdsStructElement, decltype(elem_deleter)>
//        kid_elem(struct_tree->AcquireStructElement(kid_obj), elem_deleter);

    PdsStructElement* kid_elem = struct_tree->GetStructElement(kid_obj),
      auto paragraph = GetFirstParagraph(kid_elem.get());
      if (paragraph)
        return paragraph;
    }
    return nullptr;
    //return PdsStructElementP(nullptr, struct_elem_deleter);
  }

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

    // move paragraph to the back of it's parent
    //PdsStructElementP parent(struct_tree->AcquireStructElement(paragraph->GetParentObject()),
    //  struct_elem_deleter);
    PdsStructElement* parent = struct_tree->GetStructElement(paragraph->GetParentObject()),
      struct_elem_deleter);
    if (!parent)
      throw PdfixException();
    
    if (!paragraph->SetParent(parent.get(), parent->GetNumKids()-1))
      throw PdfixException();

    // save document
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();
    
    doc->Close();
    pdfix->Destroy();
  }
  
}