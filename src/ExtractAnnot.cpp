////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractAnnots.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"

namespace ExtractData {
  
  // Extract annotation data
  void ExtractAnnot(PdfAnnot* annot, ptree& node, const DataType& data_types) {
    auto annot_dict = annot->GetObject();
    node.put("subtype", EncodeText(annot_dict->GetText(L"Subtype")));
    if (data_types.extract_bbox) {
      ptree bbox_node;
      ExtractBBox(annot->GetBBox(), bbox_node, data_types);
      node.put_child("bbox", bbox_node);
    }
  }

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
}