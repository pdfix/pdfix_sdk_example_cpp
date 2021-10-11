////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractAnnots.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"

namespace ExtractData {

  // extract widget annotation data
  void ExtractWidgetAnnot(PdfWidgetAnnot* widget, ptree& node, const DataType& data_types) {

    auto form_field = widget->GetFormField();
    if (form_field) {
      node.put("field_name", EncodeText(form_field->GetFullName()));
      PdfFieldType field_type = kFieldUnknown;
      switch (field_type) {
      case kFieldButton: node.put("field_type", "button");
        break;
      case kFieldText: node.put("field_type", "text");
        break;
      case kFieldCombo: node.put("field_type", "dropdown");
        break;
      case kFieldCheck: node.put("field_type", "checkbox");
        break;
      case kFieldRadio: node.put("field_type", "radio");
        break;
      case kFieldList: node.put("field_type", "list");
        break;
      case kFieldSignature: node.put("field_type", "signature");
        break;
      default:
        break;
      }
    }
  }

  // Extract annotation data
  void ExtractAnnot(PdfAnnot* annot, ptree& node, const DataType& data_types) {
    auto annot_dict = annot->GetObject();
    node.put("object_id", annot_dict->GetId());    
    auto subtype = annot_dict->GetText(L"Subtype");
    node.put("subtype", EncodeText(subtype));
    if (data_types.extract_bbox) {
      ptree bbox_node;
      ExtractBBox(annot->GetBBox(), bbox_node, data_types);
      node.put_child("bbox", bbox_node);
    }

    if (subtype == L"Widget")
      ExtractWidgetAnnot((PdfWidgetAnnot *)annot, node, data_types);

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