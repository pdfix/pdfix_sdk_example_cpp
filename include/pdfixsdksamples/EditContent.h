////////////////////////////////////////////////////////////////////////////////////////////////////
// EditContent.h
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace EditContent {

  struct ObjectProps {
    int pos_x = 0;            // position x for object
    int pos_y = 0;            // position y for object
    int width = 0;            // position x for object
    int height = 0;           // position y for object
    bool path = false;           // add path
    bool image = false;          // add image
    bool text = false;           // add text
  };

  void EditPageContent(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const std::wstring& img_path,
    const ObjectProps& object_prop);
  void AddPath(PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop);
  void CreateRect(PdfRect* rect, PdsPath* path);
  void CreateTriangle(PdfRect* rect, PdsPath* path_obj);
  void CreateCircle(PdfRect* rect, PdsPath* path_obj);
  void AddImage(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const std::wstring& img_path, 
    const ObjectProps& object_prop);
  void AddStampAnnot(Pdfix* pdfix, PdfDoc* doc, PdfPage* page, const std::wstring& img_path,
    const ObjectProps& object_prop);
  //void AddText(PdsContent* content, const ObjectProps& object_prop);

  void Run(
    const std::wstring& open_path,    // source PDF document
    const std::wstring& save_path,    // output PDF document
    const std::wstring& img_path,    // output PDF document
    const ObjectProps& object_prop    // structure containing object properties
  );
}