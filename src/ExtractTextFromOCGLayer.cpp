////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractTextFromOCGLayer.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "ReadOCGLayers.hpp"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

namespace ExtractTextFromOCGLayer {

  // get the ocg layers of the page object
  std::map<std::wstring, int> GetPageObjectLayers(PdsPageObject* page_object) {
    std::map<std::wstring, int> layers;
    auto content_mark = page_object->GetContentMark();
    if (content_mark) {
      for (auto i = 0; i < content_mark->GetNumTags(); i++) {
        std::wstring name = content_mark->GetTagName(i);
        if (name == L"OC") {
          auto content_mark_obj = content_mark->GetTagObject(i);
          if (content_mark_obj) {
            std::wstring type = content_mark_obj->GetText(L"Type");
            
            auto push_ocg = [&](auto ocg) {
              std::wstring ocg_name = ocg->GetText(L"Name");
              auto id = ocg->GetId();
              layers.insert(std::make_pair(ocg_name, id));
            };

            if (type == L"OCMD") {
              // NOTE: this sample does not handle the OCMD policy
              auto ocgs_dict = content_mark_obj->GetDictionary(L"OCGs");
              if (ocgs_dict) {
                push_ocg(ocgs_dict);
              }
              auto ocgs_arr = content_mark_obj->GetArray(L"OCGs");
              if (ocgs_arr) {
                for (auto j = 0; j < ocgs_arr->GetNumObjects(); j++) {
                  auto ocg_dict = ocgs_arr->GetDictionary(j);
                  push_ocg(ocg_dict);
                }
              }
            }
            else if (type == L"OCG") {
              push_ocg(content_mark_obj);
            }
          }
        }
      }
    }
    return layers;
  }

  // check page object if it fits to
  void CheckPageObject(PdsPageObject* page_object, const std::pair<std::wstring, int>& layer) {
    switch (page_object->GetObjectType()) {
      case kPdsPageText: {
        auto text_obj = (PdsText*)page_object;
        auto text = text_obj->GetText();
        text_obj->GetText((wchar_t*)text.c_str(), (int)text.size());

        auto layers = GetPageObjectLayers(page_object);
        for (auto& l : layers) {
          if (layer.first == l.first && layer.second == l.second) {
            std::cout << ToUtf8(text) << std::endl;
            break;
          }
        }
      }
      default: ;
    }
  }

  void Run(
    const std::wstring& open_file                 // source PDF document
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
    if (!doc)
      throw PdfixException();

    auto page = doc->AcquirePage(0);
    auto layers = ReadOCGLayers::ReadLayerNames(doc->GetRootObject());
    for (auto& layer : layers) {
      std::cout << "Text in layer " + ToUtf8(layer.first) +
        "(" + std::to_string(layer.second) + ")" << std::endl;
      for (auto i = 0; i < page->GetNumPageObjects(); i++) {
        auto page_object = page->GetPageObject(i);
        CheckPageObject(page_object, layer);
      }
    }
    page->Release();

    doc->Close();
    pdfix->Destroy();
  }
}