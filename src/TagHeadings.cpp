////////////////////////////////////////////////////////////////////////////////////////////////////
// TagHeadings.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace TagHeadings {

// get text object's of specified mcid the text state
bool GetPageObjectTextState(PdsPageObject* page_object, int mcid, PdfTextState* ts) {
  if (page_object->GetObjectType() == kPdsPageText) {
    PdsText* text = (PdsText*)page_object;

    // check if this text page object has the same mcid
    PdsContentMark* content_mark = page_object->GetContentMark();
    if (content_mark && content_mark->GetTagMcid() == mcid) {
      text->GetTextState(ts);
      return true;
    }
  } else if (page_object->GetObjectType() == kPdsPageForm) {
    // search for the text object inside of the form XObject
    PdsForm* form = (PdsForm*)page_object;
    auto content = form->GetContent();
    for (int i = 0; i < content->GetNumObjects(); i++) {
      if (GetPageObjectTextState(content->GetObject(i), mcid, ts))
        return true;
    }
  }
  return false;
}

// get the text state of the text objects inside paragraph by iterating content kid objects
bool GetParagraphTextState(PdsStructElement* struct_elem, PdfTextState* ts) {
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
        if (GetPageObjectTextState(content->GetObject(j), mcid, ts))
          return true;
      }
    }
  }
  return false;
}

// re-tag the struct element to heading based on font properties
void TagParagraphAsHeading(PdsStructElement* struct_elem) {
  auto type = struct_elem->GetType(true);
  struct_elem->GetType(true, (wchar_t*)type.c_str(), (int)type.size());
  if (type == L"P") {
    // get the paragraph text_state
    PdfTextState ts;
    if (GetParagraphTextState(struct_elem, &ts)) {
      // get the font name
      auto font_name = ts.font->GetFontName();

      std::wstring tag_type;
      if (font_name.find(L"Black") != std::wstring::npos && ts.font_size >= 25)
        tag_type = L"H1";
      else if (font_name.find(L"Bold") != std::wstring::npos && ts.font_size >= 16)
        tag_type = L"H2";

      // update tag type
      if (!tag_type.empty()) {
        struct_elem->SetType(tag_type.c_str());
      }
    }
    return;  // this was a P tag, no need to continue to kid struct elements
  }
  // search kid struct elements
  for (int i = 0; i < struct_elem->GetNumChildren(); i++) {
    if (struct_elem->GetChildType(i) == kPdsStructChildElement) {
      PdsObject* kid_obj = struct_elem->GetChildObject(i);
      auto kid_elem = struct_elem->GetStructTree()->GetStructElementFromObject(kid_obj);
      TagParagraphAsHeading(kid_elem);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// TagParagraphAsHeading
// re-tag P tags to Hx tag based on predefined font properties (size, font name)
//////////////////////////////////////////////////////////////////////////////////////////////////
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

  // tag headings
  for (int i = 0; i < struct_tree->GetNumChildren(); i++) {
    PdsObject* kid_obj = struct_tree->GetChildObject(i);
    auto kid_elem = struct_tree->GetStructElementFromObject(kid_obj);
    TagParagraphAsHeading(kid_elem);
  }

  // save document
  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}

}  // namespace TagHeadings
