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
}