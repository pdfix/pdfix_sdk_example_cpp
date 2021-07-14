#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace ExtractData {

  // structure defining the data to be extracted from the pdf document
  struct DataType {
    // document data extraction
    bool doc_info = false;                // general document information (title, author, creator, page count)
    bool doc_acroform = false;            // pdf form fields
    bool doc_outlines = false;            // document bookmarks
    bool doc_struct_tree = false;         // document structure tree (list pdf tags)

    // page data extraction
    int page_num = -1;                    // page to process, default -1 to process all pages
    bool page_info = false;               // general page information (bbox, rotation, etc.)
    bool page_map = false;                // page logical content extraction, scrape pdf data
    bool page_content = false;            // page raw contnet
    bool page_annots = false;             // page annotations

    // page content
    bool page_object_info = false;        // extract general page object information (bbox, etc.)

    // page map and page content extraction
    bool extract_text = false;           // extract text when extracting page map or page content
    bool extract_text_style = false;     // extract text style of the text element
    bool extract_text_state = false;     // extract text state if the text object or element
    bool extract_tables = false;         // extract table strecture extracting page map
    bool extract_images = false;         // extract images when extracting page map or page content
    bool extract_paths = false;          // extract images when extracting page content
    bool extract_bbox = false;           // extract element or object bbox
    bool extract_graphic_state = false;  // extract object's graphic state

    // structure tree
    bool struct_tree_content = false;     // extract content when listing structure tree using doc_struct_tree

    // rendering and images
    float render_zoom = 1.;              // page rasterizing zoom of image extraction
    PdfRotate render_rotate = kRotate0;   // page rasterizing rotation of image extraction
    PdfImageFormat image_format = kImageFormatJpg;  // format of the image
  };

  // annotations
  void ExtractAnnot(PdfAnnot *annot, ptree &node, const DataType& data_types);

  // page content
  void ExtractTextObject(PdsText *text, ptree &node, const DataType &data_types);
  void ExtractFormObject(PdsForm *form, ptree &node, const DataType &data_types);
  void ExtractPathObject(PdsPath *path, ptree &node, const DataType &data_types);
  void ExtractImageObject(PdsImage *image, ptree &node, const DataType &data_types);
  void ExtractPageObject(PdsPageObject *page_object, ptree &node, const DataType &data_types);
  void ExtractContent(PdsContent *content, ptree &node, const DataType &data_types);

  // page map - data scraping
  void ExtractTextElement(PdeText *text, ptree &node, const DataType& data_types);
  void ExtractTableElement(PdeTable *table, ptree &node, const DataType &data_types);
  void ExtractImageElement(PdeImage *image, ptree &node, const DataType &data_types);
  void ExtractPageElement(PdeElement *element, ptree &node, const DataType &data_types);
  void ExtractPageMap(PdePageMap *page_map, ptree &node, const DataType &data_types);

  // page 
  void ExtractPageAnnots(PdfPage *page, ptree &node, const DataType& data_types);
  void ExtractPageData(PdfPage *page, ptree &node, const DataType &data_types);
  void ExtractPageMap(PdfPage *page, ptree &node, const DataType &data_types);
  void ExtractPageContent(PdfPage *page, ptree &node, const DataType &data_types);

  // struct tree
  void ExtractStructObject(PdsStructTree *struct_tree, PdsObject *object, ptree &node,
                           const DataType &data_types);
  void ExtractStructTree(PdsStructTree *struct_tree, ptree &node, const DataType &data_types);

  // document
  void ExtractDocumentPages(PdfDoc *doc, ptree &node, const DataType &data_types);
  void ExtractDocumentInfo(PdfDoc *doc, ptree &node, const DataType &data_types);
  void ExtractDocumentData(PdfPage *page, ptree &node, const DataType &data_types);
  void ExtractDocumentStructTree(PdfDoc *doc, ptree &node, const DataType &data_types);

  // utils
  std::string EncodeText(const std::wstring &text);
  void ExtractBBox(PdfRect bbox, ptree &node, const DataType& data_types);
  void ExtractTextState(PdfTextState *text_state, ptree &node, const DataType &data_types);
  void ExtractGraphicState(const PdfGraphicState &graphics_state, ptree &node, const DataType &data_types);
  void RenderPageArea(PdfPage *page, PdfRect &bbox, ptree &node, const DataType &data_types);

  void Run(
      const std::wstring &open_path,    // source PDF document
      const std::wstring &password,     // open password
      const std::wstring &config_path,  // configuration file
      std::ostream &output,             // output stream
      const DataType& data_types,       // structure containing data types to extract
      bool preflight,                   // make preflight before processing
      PsDataFormat format               // output format
      );       
};
