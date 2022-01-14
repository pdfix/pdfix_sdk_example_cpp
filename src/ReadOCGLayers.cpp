////////////////////////////////////////////////////////////////////////////////////////////////////
// ReadOCGLayers.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ReadOCGLayers.h"

#include <string>
#include <map>
#include <iostream>
#include "pdfixsdksamples/Utils.h"
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ReadOCGLayers {
  std::map<std::wstring, int> ReadLayerNames(PdsDictionary* root_obj) {
    std::map<std::wstring, int> layers;
    
    auto ocprops = root_obj->GetDictionary(L"OCProperties");
    if (ocprops) {
      auto ocgs = ocprops->GetArray(L"OCGs");
      if (ocgs) {
        for (auto index = 0; index < ocgs->GetNumObjects(); index++) {
          auto ocg = ocgs->GetDictionary(index);
          if (ocg) {
            auto name = ocg->GetText(L"Name");
            auto id = ocg->GetId();
            layers.insert(std::make_pair(name, id));
          }
        }
      }
    }
    return layers;
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

    auto layers = ReadLayerNames(doc->GetRootObject());
    for (auto& layer : layers) {
      std::cout << ToUtf8(layer.first) << "(" << std::to_string(layer.second)<< ")" << std::endl;
    }

    doc->Close();
    pdfix->Destroy();
  }
}