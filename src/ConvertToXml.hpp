////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToXml.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ConvertToXml_cpp
*/
/*!
\page ConvertToXml_cpp Pdf To XML Sample
Example how to convert whole PDF document to XML.
\snippet /ConvertToXml.hpp ConvertToXml_cpp
*/

#pragma once

//! [ConvertToXml_cpp]
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include "Pdfix.h"

extern std::string ToUtf8(const std::wstring& wstr);
static int id = 1;
static std::wstring xml_save_path;

typedef struct _PdfXmlRec {
  PdfDoc* doc;
  PdfPage* page;
  PdfPageView* page_view;
  PdePageMap* page_map;
  PsImage* page_image;
  PdeElement* element;
  PdeElement* parent;
  std::ofstream* ofs;
  _PdfXmlRec() {
    doc = nullptr;
    page = nullptr;
    page_view = nullptr;
    page_map = nullptr;
    page_image = nullptr;
    element = nullptr;
    parent = nullptr;
    ofs = nullptr;
  }
  _PdfXmlRec(_PdfXmlRec& params) {
    doc = params.doc;
    page = params.page;
    page_view = params.page_view;
    page_map = params.page_map;
    page_image = params.page_image;
    element = nullptr;
    parent = nullptr;
    ofs = params.ofs;
  }
  void Write(const std::string& str) {
    if (str.empty())
      return;
    *ofs << str.c_str();
  }
  void Write(const std::wstring& str) {
    if (str.empty())
      return;
    *ofs << ToUtf8(str).c_str();
  }
  void Write(const PdfRGB& color) {
    std::stringstream stream;
    stream << "#";
    stream << std::setfill('0');
    stream << std::hex << std::setw(2) << color.r;
    stream << std::hex << std::setw(2) << color.g;
    stream << std::hex << std::setw(2) << color.b;
    *ofs << stream.str();
  }
  void Write(double value) {
    *ofs << std::to_string(value);
  }
  void Write(int value) {
    *ofs << std::to_string(value);
  }
  void Write(const PdfRect& elem_rect) {
    Write("<BBox>\n");
    Write("<Left>\n");
    Write(elem_rect.left);
    Write("</Left>\n");
    Write("<Bottom>\n");
    Write(elem_rect.bottom);
    Write("</Bottom>\n");
    Write("<Right>\n");
    Write(elem_rect.right);
    Write("</Right>\n");
    Write("<Top>\n");
    Write(elem_rect.top);
    Write("</Top>\n");
    Write("</BBox>\n");
  }
} PdfXmlRec;

extern void ElementToXml(PdfXmlRec& params);
extern void ImageToXml(PdfXmlRec& params);

////////////////////////////////////////////////////////////////////////////////////////////////////
void ContainerToXml(PdfXmlRec& params) {
  Pdfix* pdfix = GetPdfix();
  PdeElement* container = (PdeElement*)params.element;
  int count = container->GetNumChildren();
  if (count == 0)
    return;

  // Part
  params.Write("<Part>\n");

  // process children
  for (int i = 0; i < count; i++) {
    PdfXmlRec child_params(params);
    child_params.parent = container;
    PdeElement* child = container->GetChild(i);
    if (!child)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    child_params.element = child;
    ElementToXml(child_params);
  }

  params.Write("</Part>\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
PdfAnnot* GetAnnotAtRect(PdfXmlRec& params, PdfRect& char_rect, PdfAnnotSubtype subtype) {
  int num_annots = params.page->GetNumAnnotsAtRect(&char_rect);
  for (int i = 0; i < num_annots; i++) {
    PdfAnnot* annot = params.page->GetAnnotAtRect(&char_rect, i);
    if (annot && (annot->GetSubtype() == subtype))
      return annot;
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteAnnot(PdfXmlRec& params, PdfAnnot* annot) {
  PdfAnnotAppearance ap;
  annot->GetAppearance(&ap);
  switch (annot->GetSubtype()) {
  case kAnnotLink: {
    PdfLinkAnnot* link_annot = (PdfLinkAnnot*)annot;
    PdfAction* action = link_annot->GetAction();
    if (action) {
      std::wstring link;
      link.resize(action->GetURI(nullptr, 0));
      action->GetURI((wchar_t*)link.c_str(), (int)link.size());

      // Link
      params.Write("<Link>\n");
      params.Write(link);
      params.Write("</Link>\n");
    }
  } break;
  case kAnnotHighlight:
    params.Write("<Annot>\n");
    params.Write("<TextDecorationType>Highlight</TextDecorationType>\n");
    params.Write("<TextDecorationColor>");
    params.Write(ap.border_color);
    params.Write("</TextDecorationColor>\n");
    params.Write("</Annot>\n");
    break;
  case kAnnotUnderline:
    params.Write("<Annot>\n");
    params.Write("<TextDecorationType>Underline</TextDecorationType>\n");
    params.Write("<TextDecorationColor>");
    params.Write(ap.border_color);
    params.Write("</TextDecorationColor>\n");
    params.Write("</Annot>\n");
    break;
  case kAnnotSquiggly:
  case kAnnotStrikeOut:
    params.Write("<Annot>\n");
    params.Write("<TextDecorationType>LineThrough</TextDecorationType>\n");
    params.Write("<TextDecorationColor>");
    params.Write(ap.border_color);
    params.Write("</TextDecorationColor>\n");
    params.Write("</Annot>\n");
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteTextState(PdfXmlRec& params, PdfTextState& ts) {
  std::wstring font_name;
  std::wstring sys_font_name;
  bool bold = false;
  bool italic = false;

  // TextState
  params.Write("<TextState>\n");

  PdfFont* font = ts.font;
  if (font) {
    PdfFontState fs;
    font->GetFontState(&fs);
    bold = fs.bold == 1;
    italic = fs.italic == 1;
    sys_font_name.resize(font->GetSystemFontName(nullptr, 0));
    font->GetSystemFontName((wchar_t*)sys_font_name.c_str(), (int)sys_font_name.size());

    font_name.resize(font->GetFontName(nullptr, 0));
    font->GetFontName((wchar_t*)font_name.c_str(), (int)font_name.size());

    // Font
    params.Write("<Font>\n");

    params.Write("<FontName>");
    params.Write(font_name);
    params.Write("</FontName>\n");

    params.Write("<SysFontName>");
    params.Write(sys_font_name);
    params.Write("</SysFontName>\n");

    params.Write("<FontItalic>");
    params.Write(fs.italic);
    params.Write("</FontItalic>\n");

    params.Write("<FontWeight>");
    params.Write(fs.bold);
    params.Write("</FontWeight>\n");

    params.Write("<FontSize>");
    params.Write(std::to_string(ts.font_size) + "px");
    params.Write("</FontSize>\n");

    params.Write("</Font>\n");
  }

  // Layout
  params.Write("<Layout>\n");  
  // Layout/Color
  params.Write("<Color>");
  params.Write(ts.color_state.fill_color);
  params.Write("</Color>\n");
  params.Write("</Layout>\n");

  params.Write("</TextState>\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
std::wstring EncodeText(const std::wstring& text) {
  // https://www.w3schools.com/html/html_charset.asp
  std::wstring result;
  for (auto& c : text) {
    switch (c) {
    case '<': result += L"&#60;"; break;
    case '>': result += L"&#62;"; break;
    case '&': result += L"&#38;"; break;
    case '"': result += L"&#34;"; break;
    case '\'': result += L"&#39;"; break;
    case '¢': result += L"&#162;"; break;
    case '£': result += L"&#163;"; break;
    case '¥': result += L"&#165;"; break;
    case '€': result += L"&#128;"; break;
    case '©': result += L"&#169;"; break;
    case '®': result += L"&#174;"; break;
    default: result += c;
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TextToXml(PdfXmlRec& params) {
  Pdfix* pdfix = GetPdfix();
  PdeText* text = static_cast<PdeText*>(params.element);

  PdfRect elem_rect;
  PdfDevRect elem_dev_rect;
  params.element->GetBBox(&elem_rect);
  params.page_view->RectToDevice(&elem_rect, &elem_dev_rect);
  double elem_width = elem_dev_rect.right - elem_dev_rect.left;
  double elem_height = elem_dev_rect.bottom - elem_dev_rect.top;
  if (elem_dev_rect.right == elem_dev_rect.left || elem_dev_rect.top == elem_dev_rect.bottom)
    return;

  // text indent
  double indent = text->GetIndent();
  // text angle
  double angle = text->GetAngle();

  bool first_text = true;
  PdfTextState ts;
  if (text->HasTextState())
    text->GetTextState(&ts);
  std::wstring link = L"";

  PdfAnnot* link_annot = nullptr;
  PdfAnnot* highlight_annot = nullptr;
  PdfAnnot* underline_annot = nullptr;
  PdfAnnot* squiggly_annot = nullptr;
  PdfAnnot* strikeOut_annot = nullptr;

  std::string tag = "P";
  // special processing if parent is a list element
  //if (params.parent && (params.parent->GetType() == kPdeList)) {
  //  params.Write("<Lbl>\n");
  //}
  //else if (params.parent && (params.parent->GetType() == kPdeToc)) {
  //  params.Write("<TOCI>\n");
  //}
  /*else */{
    PdfTextStyle text_type = text->GetTextStyle();
    switch (text_type) {
    case kTextH1: tag = "H1"; break;
    case kTextH2: tag = "H2"; break;
    case kTextH3: tag = "H3"; break;
    case kTextH4: tag = "H4"; break;
    case kTextH5: tag = "H5"; break;
    case kTextH6: tag = "H6"; break;
    default:;
    }
    params.Write("<" + tag + ">");
  }

  bool first_word = !params.parent || (params.parent->GetType() != kPdeList);
  
  // process text characters
  int num_lines = text->GetNumTextLines();
  for (int l = 0; l < num_lines; l++) {
    PdeTextLine* line = text->GetTextLine(l);
    if (!line)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    int num_words = line->GetNumWords();
    for (int w = 0; w < num_words; w++) {
      PdeWord* word = line->GetWord(w);
      if (!word)
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

      // process word flags
      int word_flags = word->GetWordFlags();
      if ((word_flags & kWordImage) == kWordImage) {
        PdeElement* image = word->GetBackground();
        if (image && image->GetType() == kPdeImage) {
          PdfXmlRec background_params(params);
          background_params.element = image;
          background_params.parent = params.parent;
          ImageToXml(background_params);
        }
      }
      else {
        // iterate through each character
        int length = word->GetNumChars();
        for (int i = 0; i < length; i++) {
          PdfTextState char_ts;
          word->GetCharTextState(i, &char_ts);
          std::wstring char_link;
          PdfRect char_bbox;
          word->GetCharBBox(i, &char_bbox);
          PdfPoint char_point = {
            (char_bbox.right + char_bbox.left) / 2.,
            (char_bbox.top + char_bbox.bottom) / 2.,
          };

          PdfAnnotAppearance ap;
          PdfAnnot* char_annot = nullptr;
          std::wstring char_str;
          char_str.resize(word->GetCharText(i, nullptr, 0));
          word->GetCharText(i, (wchar_t*)char_str.c_str(), (int)char_str.size());


          // if there where any change in annotations or text syle, start a new span
           // compare annotations
          PdfAnnot* char_link_annot = GetAnnotAtRect(params, char_bbox, kAnnotLink);
          PdfAnnot* char_highlight_annot = GetAnnotAtRect(params, char_bbox, kAnnotHighlight);
          PdfAnnot* char_underline_annot = GetAnnotAtRect(params, char_bbox, kAnnotUnderline);
          PdfAnnot* char_squiggly_annot = GetAnnotAtRect(params, char_bbox, kAnnotSquiggly);
          PdfAnnot* char_strikeOut_annot = GetAnnotAtRect(params, char_bbox, kAnnotStrikeOut);
          auto same_annot = (char_link_annot == link_annot) &&
            (char_highlight_annot == highlight_annot) &&
            (char_underline_annot == underline_annot) &&
            (char_squiggly_annot == squiggly_annot) &&
            (char_strikeOut_annot == strikeOut_annot);
          // compare text_state
          auto same_ts = memcmp(&ts, &char_ts, sizeof(ts)) == 0;

          if (first_text || !same_ts || !same_annot) {
            // end previous style 
            if (!first_text) {
              params.Write("</Text>\n");
              params.Write("</Span>\n");
            }

            memcpy(&ts, &char_ts, sizeof(char_ts));
            link_annot = char_link_annot;
            highlight_annot = char_highlight_annot;
            underline_annot = char_underline_annot;
            squiggly_annot = char_squiggly_annot;
            strikeOut_annot = char_strikeOut_annot;

            params.Write("<Span>\n");
            if (first_text || !same_ts)
              WriteTextState(params, ts);
            if (first_text || !same_annot) {
              if (link_annot) WriteAnnot(params, link_annot);
              if (highlight_annot) WriteAnnot(params, highlight_annot);
              if (underline_annot) WriteAnnot(params, underline_annot);
              if (squiggly_annot) WriteAnnot(params, squiggly_annot);
              if (strikeOut_annot) WriteAnnot(params, strikeOut_annot);
            }
            params.Write("<Text>");

            first_text = false;
          }

          // write character
          params.Write(ToUtf8(EncodeText(char_str).c_str()));
        }
      }
      // add whitespace between words
      if (w != num_words - 1)
        params.Write(" ");
    }
    // add whitespace between lines
    if (l != num_lines - 1)
      params.Write(" ");
  }

  params.Write("</Text>\n");
  params.Write("</Span>\n");
  params.Write("</" + tag + ">\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ListToXml(PdfXmlRec& params) {
  Pdfix* pdfix = GetPdfix();
  PdeList* list = (PdeList*)params.element;
  int children_count = list->GetNumChildren();
  if (children_count == 0)
    return;

  // List
  params.Write("<L>\n");

  // Attributes
  params.Write("<Attributes>\n");
  // List
  params.Write("<List>\n");
  // List/ListNumbering
  params.Write("<ListNumbering>");
  params.Write("None");
  params.Write("</ListNumbering>\n");
  params.Write("</List>\n");
  params.Write("</Attributes>\n");

  // process LI children
  for (int i = 0; i < children_count; i++) {
    PdeElement* child = list->GetChild(i);
    if (!child)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    PdfElementType child_type = child->GetType();
    if (child_type != kPdeList)
      params.Write("<LI>\n");

    PdfXmlRec child_params(params);
    child_params.element = child;
    child_params.parent = list;    
    ElementToXml(child_params);

    if (child_type != kPdeList)
      params.Write("</LI>\n");
  }

  params.Write("</L>\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TableToXml(PdfXmlRec& params) {
  Pdfix* pdfix = GetPdfix();
  PdeTable* table = (PdeTable*)params.element;
  int row_count = table->GetNumRows();
  int col_count = table->GetNumCols();

  // Table
  params.Write("<Table>\n");

  PdfRect elem_rect;
  PdfDevRect elem_dev_rect;
  params.element->GetBBox(&elem_rect);
  params.page_view->RectToDevice(&elem_rect, &elem_dev_rect);

  // Attributes
  params.Write("<Attributes>\n");
  // Layout
  params.Write("<Layout>\n");
  // Layout/BBox
  params.Write(elem_rect);
  params.Write("</Layout>\n");
  params.Write("</Attributes>\n");

  // process table cells
  for (int row = 0; row < row_count; row++) {
    params.Write("<TR>\n");
    for (int col = 0; col < col_count; col++) {
      PdeCell* cell = (PdeCell*)table->GetCell(row, col);
      if (!cell)
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

      int row_span = cell->GetRowSpan();
      int col_span = cell->GetColSpan();
      if (row_span == 0 || col_span == 0)
        continue;

      params.Write("<TD>\n");

      // TD Attributes
      params.Write("<Attributes>\n");

      // Layout
      params.Write("<Layout>\n");

      // Layout/Width - columns width
      PdfRect cell_bbox;
      cell->GetBBox(&cell_bbox);
      params.Write("<Width>");
      params.Write(cell_bbox.right - cell_bbox.left);
      params.Write("</Width>\n");

      PdfGraphicState gstate;
      cell->GetGraphicState(&gstate);
      // Layout/BackgroundColor
      if (gstate.color_state.fill_type != kFillTypeNone) {
        params.Write("<BackgroundColor>");
        params.Write(gstate.color_state.fill_color);
        params.Write("</BackgroundColor>\n");
      }
      // Layout/BorderColor
      if (gstate.color_state.stroke_type != kFillTypeNone) {
        params.Write("<BorderColor>");
        params.Write(gstate.color_state.stroke_color);
        params.Write("</BorderColor>\n");
      }
      // Layout/BorderThickness
      params.Write("<BorderThickness>\n");
      if (cell->HasBorderGraphicState(3)) {
        // Layout/BorderThickness/Left
        params.Write("<Left>");
        params.Write(gstate.line_width);
        params.Write("</Left>\n");
      }
      if (cell->HasBorderGraphicState(0)) {
        // Layout/BorderThickness/Left
        params.Write("<Top>");
        params.Write(gstate.line_width);
        params.Write("</Top>\n");
      }
      if (cell->HasBorderGraphicState(1)) {
        // Layout/BorderThickness/Right
        params.Write("<Right>");
        params.Write(gstate.line_width);
        params.Write("</Right>\n");
      }
      if (cell->HasBorderGraphicState(2)) {
        // Layout/BorderThickness/Bottom
        params.Write("<Bottom>");
        params.Write(gstate.line_width);
        params.Write("</Bottom>\n");
      }
      params.Write("</BorderThickness>\n");

      params.Write("</Layout>\n");

      //Table
      params.Write("<Table>\n");

      //Table/RowSpan
      params.Write("<RowSpan>");
      params.Write(row_span);
      params.Write("</RowSpan>\n");

      //Table/ColSpan
      params.Write("<ColSpan>");
      params.Write(col_span);
      params.Write("</ColSpan>\n");

      params.Write("</Table>\n");

      params.Write("</Attributes>\n");

      // process cell's children
      int children_count = cell->GetNumChildren();
      if (children_count > 0) {
        for (int i = 0; i < children_count; i++) {
          PdeElement* child = cell->GetChild(i);
          if (!child)
            throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
          PdfXmlRec child_params(params);
          child_params.element = child;
          child_params.parent = cell;
          child_params.parent = params.parent;
          ElementToXml(child_params);
        }
      }
      params.Write("</TD>\n");
    }
    params.Write("</TR>\n");
  }
  params.Write("</Table>\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void RectToXml(PdfXmlRec& params) {
  Pdfix* pdfix = GetPdfix();
  PdeRect* rect = (PdeRect*)params.element;
  int count = rect->GetNumChildren();
  if (count == 0)
    return;

  // Div
  params.Write("<Div>\n");

  // Attributes
  params.Write("<Attributes>\n");
  
  // Layout
  params.Write("<Layout>\n");
  
  // Layout/BackgroundColor
  PdfGraphicState gstate;
  rect->GetGraphicState(&gstate);
  if (gstate.color_state.fill_type != kFillTypeNone) {
    params.Write("<BackgroundColor>");
    params.Write(gstate.color_state.fill_color);
    params.Write("</BackgroundColor>\n");
  }
  // Layout/BorderColor
  if (gstate.color_state.stroke_type != kFillTypeNone) {
    params.Write("<BorderColor>");
    params.Write(gstate.color_state.stroke_color);
    params.Write("</BorderColor>\n");
  }
  // Layout/BorderThickness
  params.Write("<BorderThickness>");
  params.Write(gstate.line_width);
  params.Write("</BorderThickness>\n");
  
  params.Write("</Layout>\n");
  params.Write("</Attributes>\n");

  // process children
  for (int i = 0; i < count; i++) {
    PdeElement* child = rect->GetChild(i);
    if (!child)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    PdfXmlRec child_params(params);
    child_params.element = child;
    child_params.parent = rect;
    ElementToXml(child_params);
  }

  params.Write("</Div>\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ImageToXml(PdfXmlRec& params) {
  Pdfix* pdfix = GetPdfix();
  PdeImage* image = (PdeImage*)params.element;

  // Figure
  params.Write("<Figure>\n");
  
  PdfRect elem_rect;
  PdfDevRect elem_dev_rect;
  params.element->GetBBox(&elem_rect);
  params.page_view->RectToDevice(&elem_rect, &elem_dev_rect);

  // Attributes
  params.Write("<Attributes>\n");
  // Layout
  params.Write("<Layout>\n");
  // Layout/BBox
  params.Write(elem_rect);
  params.Write("</Layout>\n");
  params.Write("</Attributes>\n");

  // save image data
  std::wstring path = xml_save_path + L"image_" + std::to_wstring(id++) + L".jpg";
  PdfImageParams image_params;
  image_params.format = kImageFormatJpg;
  image_params.quality = 75;
  params.page_image->SaveRect(path.c_str(), &image_params, &elem_dev_rect);

  // image resource
  params.Write("<ImageRef>");
  params.Write(ToUtf8(path));
  params.Write("</ImageRef>\n");

  // encoded image data
  // params.Write("<ImageData>\n");

  // add responsive child elements
  int children_count = image->GetNumChildren();
  for (int i = 0; i < children_count; i++) {
    PdeElement* child = image->GetChild(i);
    if (!child)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    PdfXmlRec child_params(params);
    child_params.element = child;
    child_params.parent = image;
    ElementToXml(child_params);
  }

  params.Write("</Figure>\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FormFieldToXml(PdfXmlRec& params) {
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ElementToXml(PdfXmlRec& params) {
  PdfElementType elem_type = params.element->GetType();
  switch (elem_type) {
  case kPdeContainer:
  case kPdeHeader:
  case kPdeFooter:
  case kPdeToc: ContainerToXml(params); break;
  case kPdeText: TextToXml(params); break;
  case kPdeList: ListToXml(params); break;
  case kPdeTable: TableToXml(params); break;
  case kPdeRect:
  case kPdeLine: RectToXml(params); break;
  case kPdeImage: ImageToXml(params); break;
  case kPdeFormField: FormFieldToXml(params); break;
  default: break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void PageToXml(PdfXmlRec& params, int page_num) {
  Pdfix* pdfix = GetPdfix();

  auto deleter = [&](PdfPage* page) { 
    page->Release();
    params.page = nullptr;
    params.page_image = nullptr;
    params.page_map = nullptr;
    params.page_view = nullptr;
  };
  std::unique_ptr<PdfPage, decltype(deleter)> page(params.doc->AcquirePage(page_num), deleter);  
  params.page = page.get();
  if (!params.page)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  // set output images quality
  PdfRect crop_box;
  params.page->GetCropBox(&crop_box);
  double page_width = (crop_box.right - crop_box.left);
  double page_height = (crop_box.top - crop_box.bottom);
  double zoom = 1200 / std::min(page_width, page_height);

  // render page
  params.page_view = params.page->AcquirePageView(zoom, kRotate0);
  if (!params.page_view)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  // create page_map
  params.page_map = params.page->AcquirePageMap(nullptr, nullptr);
  if (!params.page_map)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  // render page without texts
  params.page_image = pdfix->CreateImage(params.page_view->GetDeviceWidth(),
    params.page_view->GetDeviceHeight(),
    kImageDIBFormatArgb);
  if (!params.page_image)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  PdfPageRenderParams render_params;
  render_params.render_flags |= kRenderNoText;
  render_params.image = params.page_image;
  params.page_view->GetDeviceMatrix(&render_params.matrix);
  params.page->DrawContent(&render_params, nullptr, nullptr);

  // get page container
  params.element = params.page_map->GetElement();
  if (!params.element)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  // convert page container to XML
  ElementToXml(params);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ConvertToXml(
  const std::wstring& email,          // authorization email   
  const std::wstring& license_key,    // authorization license key
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path,      // output XML file
  const std::wstring& config_path     // configuration file
) {
  xml_save_path = save_path;

  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  // customize conversion 
  if (!config_path.empty()) {
    PdfDocTemplate* doc_tmpl = doc->GetDocTemplate();
    if (!doc_tmpl)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    // load from 
    PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
    if (stm) {
      if (!doc_tmpl->LoadFromStream(stm, kDataFormatJson))
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
      stm->Destroy();
    }
  }

  // prepare output stream
  auto ofs_deleter = [&](std::ofstream* ofs) {ofs->close(); delete(ofs); };
  std::unique_ptr<std::ofstream, decltype(ofs_deleter)> ofs(new std::ofstream, ofs_deleter);
  if (!ofs)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  ofs->open(ToUtf8(save_path));

  PdfXmlRec params;
  params.doc = doc;
  params.ofs = ofs.get();
	std::string xml_header = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	params.Write(xml_header);
	params.Write("<!--Created from PDF via PDFix-->\n");
  params.Write("<Document>\n");

	PsMetadata* metadata = doc->GetMetadata();
	PsMemoryStream* stream = pdfix->CreateMemStream();
	if (!metadata->SaveToStream(stream))
			throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
	int size = stream->GetSize();
  
  std::vector<unsigned char> data;
  data.resize(size);
	stream->Read(0, &data[0], size);
  
	std::string metadata_str(begin(data), begin(data) + size);

	// remove xml header from metadata stream
	size_t pos = metadata_str.find(xml_header);
	if (pos != std::string::npos)
		metadata_str.erase(pos, xml_header.length());

	// need to modify <dc:format> to xml
	params.Write(metadata_str);
  params.Write("\n");

  auto num_pages = doc->GetNumPages();
  for (auto i = 0; i < num_pages; i++) {
    std::cout << std::endl;
    std::cout << "Processing pages..." << i + 1 << "/" << num_pages;    
    PdfXmlRec page_params(params);
    PageToXml(page_params, i);
  }

  params.Write("</Document>\n");
	std::cout << "\n";
    
  doc->Close();
  pdfix->Destroy();
}
//! [ConvertToXml_cpp]
