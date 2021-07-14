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
    PdfPoint pos;               // position of the object
    PdfPageObjectType obj_type; // type of the object
    std::wstring data;          // data associated with the object
  };

  class PropsBuilder {
    std::vector<ObjectProps> m_properties;

  public:
    PropsBuilder& AddText(float x, float y, const std::wstring& text);
    PropsBuilder& AddPath(float x, float y, const std::wstring& svg);
    PropsBuilder& AddImage(float x, float y, const std::wstring& path);
    std::vector<ObjectProps>& Get();
  };

  void EditPageContent(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const std::vector<ObjectProps>& object_props);
  
  void AddText(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop);
  
  void AddPath(PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop);
  
  void AddImage(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop);

  void Run(
    const std::wstring& output_path,               // output PDF document
    const std::vector<ObjectProps>& object_props   // structure containing object properties
  );
}