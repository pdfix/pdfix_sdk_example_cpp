////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractPage.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"


namespace ExtractData {
  void ExtractPageAnnots(PdfPage* page, ptree& node, const DataType& data_types) {
    // annotations
    ptree annots_node;
    for (int i = 0; i < page->GetNumAnnots(); i++) {
      auto annot = page->GetAnnot(i);
      if (!annot)
        continue;
      ptree annot_node;
      ExtractAnnot(annot, annot_node, data_types);
      annots_node.push_back(std::make_pair("", annot_node));
    }
    if (annots_node.size())
      node.put_child("annots", annots_node);
  }

  void ExtractPageMapData(PdfPage *page, ptree &node, const DataType &data_types) {
    auto page_map_deleter = [&](PdePageMap* page_map) { page_map->Release(); };
    std::unique_ptr<PdePageMap, decltype(page_map_deleter)> 
      page_map(page->AcquirePageMap(nullptr, nullptr), page_map_deleter);  
    if (!page_map)
      throw PdfixException();

    ptree page_map_node;

    ptree bbox_node;
    ExtractPageMap(page_map.get(), page_map_node, data_types);
    page_map_node.put_child("bbox", bbox_node);

    node.put_child("content", page_map_node);
  }

  void ExtractPageContentData(PdfPage* page, ptree &node, const DataType &data_types) {    
    auto content = page->GetContent();

    ptree contnet_node;
    ExtractPageContent(content, contnet_node, data_types);
    node.put_child("content", contnet_node);
  }

  void ExtractPageInfo(PdfPage *page, ptree &node, const DataType &data_types) {
    node.put("page_num", page->GetNumber());
    // bbox
    ptree bbox_node;
    ExtractBBox(page->GetCropBox(), bbox_node, data_types);
    node.put_child("crop_box", bbox_node);
    node.put("rotate", page->GetRotate());
  }

  // save page data
  void ExtractPageData(PdfPage* page, ptree& node, const DataType& data_types) {
    if (data_types.page_info)
      ExtractPageInfo(page, node, data_types);

    if (data_types.page_annots) 
      ExtractPageAnnots(page, node, data_types);

    if (data_types.page_map) 
      ExtractPageMapData(page, node, data_types);

    if (data_types.page_content) 
      ExtractPageContentData(page, node, data_types);
  }
}