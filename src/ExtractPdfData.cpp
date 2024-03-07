////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractData.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
// project
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

namespace ExtractData {

// extract page-based data
void ExtractDocumentPages(PdfDoc* doc, ptree& node, const DataType& data_types) {
  ptree pages_node;  // node holding the page array

  auto from_page = data_types.page_num == -1 ? 0 : data_types.page_num;
  auto to_page = data_types.page_num == -1 ? doc->GetNumPages() - 1 : data_types.page_num;

  for (auto i = from_page; i <= to_page; i++) {
    auto page_deleter = [&](PdfPage* page) { page->Release(); };
    auto page = std::unique_ptr<PdfPage, decltype(page_deleter)>(doc->AcquirePage(i), page_deleter);
    if (!page)
      throw PdfixException();

    ptree page_node;  // node holding the page
    ExtractPageData(page.get(), page_node, data_types);
    if (page_node.size())
      pages_node.push_back(std::make_pair("", page_node));
  }
  if (pages_node.size())
    node.add_child("pages", pages_node);
}

void ExtractDocumentStructTree(PdfDoc* doc, ptree& node, const DataType& data_types) {
  ptree struct_tree_node;
  auto struct_tree = doc->GetStructTree();
  if (struct_tree)
    ExtractStructTree(struct_tree, struct_tree_node, data_types);
  node.add_child("struct_tree", struct_tree_node);
}

// extract general document information (metadata, page count, is tagged, is form)
void ExtractDocumentInfo(PdfDoc* doc, ptree& node, const DataType& data_types) {
  node.put("title", EncodeText(doc->GetInfo(L"Title")));
  node.put("author", EncodeText(doc->GetInfo(L"Author")));
  node.put("creator", EncodeText(doc->GetInfo(L"Creator")));
  node.put("num_pages", doc->GetNumPages());
  node.put("tagged", doc->GetStructTree() != nullptr);
  node.put("secured", doc->IsSecured());
}

// save document information
void ExtractDocumentData(PdfDoc* doc, ptree& node, const DataType& data_types) {
  if (data_types.doc_info)
    ExtractDocumentInfo(doc, node, data_types);

  if (data_types.doc_struct_tree)
    ExtractDocumentStructTree(doc, node, data_types);

  // if (data_types.doc_acroform)
  //   ExtractDocumentAcroForm(doc, ptree & node, data_types);

  // pages
  ExtractDocumentPages(doc, node, data_types);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Run(const std::wstring& open_path,
         const std::wstring& password,
         const std::wstring& config_path,
         std::ostream& output,
         const DataType& data_types,
         bool preflight,
         PsDataFormat format) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
  if (!doc)
    throw PdfixException();

  auto doc_template = doc->GetTemplate();
  if (!doc_template)
    throw PdfixException();

  if (!config_path.empty()) {
    PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
    if (stm) {
      if (!doc_template->LoadFromStream(stm, kDataFormatJson))
        throw PdfixException();
      stm->Destroy();
    }
  }

  if (preflight) {
    // add reference pages for preflight
    for (auto i = 0; i < doc->GetNumPages(); i++) {
      if (!doc_template->AddPage(i))
        throw PdfixException();
    }

    // run document preflight
    if (!doc_template->Update())
      throw PdfixException();
  }

  ptree doc_node;  // node holding the document
  ExtractDocumentData(doc, doc_node, data_types);

  doc->Close();

  // save data to output
  switch (format) {
    case kDataFormatJson:
      write_json(output, doc_node);
      break;
    case kDataFormatXml:
      write_xml(output, doc_node);
      break;
    default:
      throw std::runtime_error("unknown output format");
  }
}
}  // namespace ExtractData
