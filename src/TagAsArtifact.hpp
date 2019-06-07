////////////////////////////////////////////////////////////////////////////////////////////////////
// TagAsArtifact.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage TagAsArtifact_cpp
*/
/*!
\page TagAsArtifact_cpp Mark tagged page objects as artifact
Example how to remove paragraph from a structure tree and mark all contained page objects as artifact.
\snippet /TagAsArtifact.hpp TagAsArtifact_cpp
*/

#pragma once

//! [TagAsArtifact_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

namespace TagAsArtifact {
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // MarkUntaggedObjectsAsArtifact
  // find any non-tagged objects in the page content and mark them as artifact
  //////////////////////////////////////////////////////////////////////////////////////////////////
  void MarkUntaggedObjectsAsArtifact(PdfPage* page) {
    PdfDoc* doc = page->GetDoc();
    
    for (int i = 0; i < page->GetNumPageObjects(); i++) {
      PdsPageObject* page_obj = page->GetPageObject(i);
      
      PdsContentMark* content_mark = page_obj->GetContentMark();
      if (!content_mark->GetTagArtifact() && content_mark->GetTagMcid() == -1) {
        PdsDictionary* artifact_dict = doc->CreateDictObject(false);
        artifact_dict->Put(L"Type", doc->CreateNameObject(false, L"Pagination"));
        artifact_dict->Put(L"Subtype", doc->CreateNameObject(false, L"Footer"));
        content_mark->AddTag("Artifact", artifact_dict, false);
      }
    }
    page->SetContent();
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Remove paragraph from structure tree
  // re-tag the struct element to heading based on font properties
  //////////////////////////////////////////////////////////////////////////////////////////////////
  void RemoveParagraph(PdsStructElement* struct_elem) {
    
    // remove last 2 P struct elements from struct tree
    static int count = 0;
    for (int i = struct_elem->GetNumKids() - 1; i >= 0 ; i--) {
      if (struct_elem->GetKidType(i) == kPdsStructKidElement) {
        PdsObject* kid_obj = struct_elem->GetKidObject(i);
        auto elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
        std::unique_ptr<PdsStructElement, decltype(elem_deleter)>
        kid_elem(struct_elem->GetStructTree()->AcquireStructElement(kid_obj), elem_deleter);
        
        std::wstring type;
        type.resize(kid_elem->GetType(true, nullptr, 0));
        kid_elem->GetType(true, (wchar_t*)type.c_str(), (int)type.size());
        if (type == L"P") {
          for (int j = kid_elem->GetNumKids() - 1; j >= 0; j--) {
            if (!kid_elem->RemoveKid(j))
              throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
          }
        }
        else if (type == L"Figure") {
          // remove figure if does not contain an alt text
          int alt_len = kid_elem->GetAlt(nullptr, 0);
          if (alt_len == 0) {
            for (int j = kid_elem->GetNumKids() - 1; j >= 0; j--) {
              if (!kid_elem->RemoveKid(j))
                throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
            }
          }
        }
        else {
          RemoveParagraph(kid_elem.get());
        }
        // remove this element if it has no kids
        if (kid_elem->GetNumKids() == 0)
          struct_elem->RemoveKid(i);
      }
      // remove only 2 paragraphs in this sample
      if (++count >= 2) break;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Mark tagged page content as Artifact
  // re-tag P tags to H1 or H2 tag based on predefined font properties (size, font name)
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
    
    // tag text on the bottom of the page as artifact
    for (int i = 0; i < struct_tree->GetNumKids(); i++) {
      PdsObject* kid_obj = struct_tree->GetKidObject(i);
      auto elem_deleter = [](PdsStructElement* elem) { elem->Release(); };
      std::unique_ptr<PdsStructElement, decltype(elem_deleter)>
        kid_elem(struct_tree->AcquireStructElement(kid_obj), elem_deleter);
      RemoveParagraph(kid_elem.get());
    }
    
    // the struct tree was updates, save page content on each page to apply changes
    for (int i = 0; i < doc->GetNumPages(); i++) {
      auto page_deleter = [](PdfPage* page) { page->Release(); };
      std::unique_ptr<PdfPage, decltype(page_deleter)>
        page(doc->AcquirePage(i), page_deleter);
      MarkUntaggedObjectsAsArtifact(page.get());
    }
    
    // save document
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    
    doc->Close();
    pdfix->Destroy();
  }
  
} //  namespace TagAsArtifact

//! [TagAsArtifact_cpp]
