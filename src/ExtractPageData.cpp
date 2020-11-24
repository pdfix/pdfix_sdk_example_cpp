////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractPage.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"


namespace ExtractData {
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
      ExtractPageMap(page, node, data_types);

    if (data_types.page_content) 
      ExtractPageContent(page, node, data_types);
  }
}