#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

typedef struct _PdfXmlRec {
  PdfDoc* doc;
  PdfPage* page;
  PdfPageView* page_view;
  PdePageMap* page_map;
  PsImage* page_image;
  PdeElement* element;
  PdeElement* parent;
  std::ofstream* ofs;
  _PdfXmlRec();
  _PdfXmlRec(_PdfXmlRec& params);
  void Write(const std::string& str);
  void Write(const std::wstring& str);
  void Write(const PdfRGB& color);
  void Write(double value);
  void Write(int value);
  void Write(const PdfRect& elem_rect);
} PdfXmlRec;

void ContainerToXml(PdfXmlRec& params);
PdfAnnot* GetAnnotAtRect(PdfXmlRec& params, PdfRect& char_rect, PdfAnnotSubtype subtype);
void WriteAnnot(PdfXmlRec& params, PdfAnnot* annot);
void WriteTextState(PdfXmlRec& params, PdfTextState& ts);
std::wstring EncodeText(const std::wstring& text);
void TextToXml(PdfXmlRec& params);
void ListToXml(PdfXmlRec& params);
void TableToXml(PdfXmlRec& params);
void RectToXml(PdfXmlRec& params);
void ImageToXml(PdfXmlRec& params);
void FormFieldToXml(PdfXmlRec& params);
void ElementToXml(PdfXmlRec& params);
void PageToXml(PdfXmlRec& params, int page_num);
void ConvertToXml(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& save_path,      // output XML file
    const std::wstring& config_path     // configuration file
    );
