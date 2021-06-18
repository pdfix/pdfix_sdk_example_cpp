////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractPageMap.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"

namespace ExtractData {
  // extract text page object data
  void ExtractTextObject(PdsText *text, ptree &node, const DataType &data_types) {
    node.put("text", EncodeText(text->GetText()));

    if (data_types.extract_text_state) {
      ptree text_state_node;
      PdfTextState ts = text->GetTextState();
      ExtractTextState(&ts, text_state_node, data_types);
      node.put_child("text_state", text_state_node);
    }
  }

  // extract image page object data
  void ExtractImageObject(PdsImage *image, ptree &node, const DataType &data_types) {
    auto page = image->GetPage();
    auto bbox = image->GetBBox();
    RenderPageArea(page, bbox, node, data_types);
  }

  // extract form page object data
  void ExtractFormObject(PdsForm *form, ptree &node, const DataType &data_types) {
    auto content = form->GetContent();
    if (!content)
      throw PdfixException();

    ptree content_node;
    ExtractContent(content, content_node, data_types);
    node.put_child("content", content_node);
  }

  // extract path page object data
  void ExtractPathObject(PdsPath *path, ptree &node, const DataType &data_types) {
    // ...
  }

  // extract page object data
  void ExtractPageObject(PdsPageObject *object, ptree &node, const DataType &data_types) {
    // general information
    auto get_object_type_string = [&]() {
      switch (object->GetObjectType()) {
      case kPdsPageForm: return std::string("pds_form");
      case kPdsPageImage: return std::string("pds_image");
      case kPdsPagePath: return std::string("pds_path");
      case kPdsPageShading: return std::string("pds_shading");
      case kPdsPageText: return std::string("pds_text");
      default: return std::string("unknown");       
      }
    };
    node.put("type", get_object_type_string());

    if (data_types.extract_bbox) {
      ptree bbox_node;
      ExtractBBox(object->GetBBox(), bbox_node, data_types);
      node.put_child("bbox", bbox_node);
    }

    if (data_types.extract_graphic_state) {
      ptree gs_node;
      ExtractGraphicState(object->GetGState(), gs_node, data_types);
      node.put_child("graphic_state", gs_node);
    }

    switch (object->GetObjectType()) {
      case kPdsPageText: 
        if (data_types.extract_text)
          ExtractTextObject((PdsText *)object, node, data_types);
        break;
      case kPdsPageForm: 
        ExtractFormObject((PdsForm *)object, node, data_types);
        break;
      case kPdsPagePath: 
        if (data_types.extract_paths)
          ExtractPathObject((PdsPath *)object, node, data_types);
        break;
      case kPdsPageImage: 
        if (data_types.extract_images)
          ExtractImageObject((PdsImage *)object, node, data_types);
        break;
      default:;
      }
  }

  // extract data from a PdsContnet object
  void ExtractContent(PdsContent *content, ptree &node, const DataType &data_types) {
    ptree objects_node;
    for (int i = 0; i < content->GetNumObjects(); i++) {
      ptree object_node;
      ExtractPageObject(content->GetObject(i), object_node, data_types);
      objects_node.push_back(std::make_pair("", object_node));
    }
    node.put_child("kids", objects_node);
  }

  void ExtractPageContent(PdfPage* page, ptree &node, const DataType &data_types) {    
    auto content = page->GetContent();

    ptree contnet_node;
    ExtractContent(content, contnet_node, data_types);
    node.put_child("content", contnet_node);
  }

}