////////////////////////////////////////////////////////////////////////////////////////////////////
// TagAsArtifact.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <memory>

#include "pdfixsdksamples/TagAsArtifact.h"

using namespace PDFixSDK;

namespace TagAsArtifact {
  
  // find any non-tagged objects in the page content and mark them as artifact
  void MarkUntaggedObjectsAsArtifact(PdfPage* page) {
    PdfDoc* doc = page->GetDoc();
    
    auto content = page->GetContent();
    for (int i = 0; i < content->GetNumObjects(); i++) {
      PdsPageObject* page_obj = content->GetObject(i);

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
  
  // re-tag the struct element to heading based on font properties
  void RemoveParagraph(PdsStructElement* struct_elem) {
    
    // remove last 2 P struct elements from struct tree
    static int count = 0;
    for (int i = struct_elem->GetNumChildren() - 1; i >= 0 ; i--) {
      if (struct_elem->GetChildType(i) == kPdsStructChildElement) {
        PdsObject* kid_obj = struct_elem->GetChildObject(i);
        auto kid_elem = struct_elem->GetStructTree()->GetStructElementFromObject(kid_obj);
        
        auto type = kid_elem->GetType(true);
        kid_elem->GetType(true, (wchar_t*)type.c_str(), (int)type.size());
        if (type == L"P") {
          for (int j = kid_elem->GetNumChildren() - 1; j >= 0; j--) {
            if (!kid_elem->RemoveChild(j))
              throw PdfixException();
          }
        }
        else if (type == L"Figure") {
          // remove figure if does not contain an alt text
          int alt_len = kid_elem->GetAlt(nullptr, 0);
          if (alt_len == 0) {
            for (int j = kid_elem->GetNumChildren() - 1; j >= 0; j--) {
              if (!kid_elem->RemoveChild(j))
                throw PdfixException();
            }
          }
        }
        else {
          RemoveParagraph(kid_elem);
        }
        // remove this element if it has no kids
        if (kid_elem->GetNumChildren() == 0)
          struct_elem->RemoveChild(i);
      }
      // remove only 2 paragraphs in this sample
      if (++count >= 2) break;
    }
  }

  // re-tag P tags to H1 or H2 tag based on predefined font properties (size, font name)
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
    
    // tag text on the bottom of the page as artifact
    for (int i = 0; i < struct_tree->GetNumChildren(); i++) {
      PdsObject* kid_obj = struct_tree->GetChildObject(i);
      auto kid_elem = struct_tree->GetStructElementFromObject(kid_obj);
      RemoveParagraph(kid_elem);
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
      throw PdfixException();
    
    doc->Close();
    pdfix->Destroy();
  }
  
}