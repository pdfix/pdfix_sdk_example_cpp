#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Pdfix.h"

using namespace PDFixSDK;

typedef struct _PdfJsonRec {
  PdfDoc* doc;
  PdfPage* page;
  PdfPageView* page_view;
  PdePageMap* page_map;
  PsImage* page_image;
  PdeElement* element;
  PdeElement* parent;
  std::ofstream* ofs;
  _PdfJsonRec() {
    doc = nullptr;
    page = nullptr;
    page_view = nullptr;
    page_map = nullptr;
    page_image = nullptr;
    element = nullptr;
    parent = nullptr;
    ofs = nullptr;
  }
  _PdfJsonRec(_PdfJsonRec& params);
  void Write(const std::string& str);
  void Write(const std::wstring& str);
  void Write(const PdfRGB& color);
  void Write(double value);
  void Write(int value);
  void Write(const PdfRect& elem_rect);
  void WriteNestedJsonObj(const std::string& str);
} PdfJsonRec;

void ContainerToJson(PdfJsonRec& params);
PdfAnnot* GetAnnotAtRect(PdfJsonRec& params, PdfRect& char_rect, PdfAnnotSubtype subtype);
void WriteAnnot(PdfJsonRec& params, PdfAnnot* annot);
void WriteTextState(PdfJsonRec& params, PdfTextState& ts);
std::wstring EncodeTextJson(const std::wstring& text);
void TextToJson(PdfJsonRec& params);
void ListToJson(PdfJsonRec& params);
void TableToJson(PdfJsonRec& params);
void RectToJson(PdfJsonRec& params);
void ImageToJson(PdfJsonRec& params);
void FormFieldToJson(PdfJsonRec& params);
void ElementToJson(PdfJsonRec& params);
void PageToJson(PdfJsonRec& params, int page_num);
void ConvertToJson(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& save_path,      // output JSON file
    const std::wstring& config_path     // configuration file
    );
