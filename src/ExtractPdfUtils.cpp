////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractPdfDataUtils.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractData.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
// project
#include "Pdfix.h"

extern std::string ToUtf8(const std::wstring& wstr);
extern std::string PsStreamEncodeBase64(PsStream *stream);

namespace ExtractData {
  auto put_array_node = [](auto &node, auto &value) {
    ptree value_node;
    value_node.put("", value);
    node.push_back(std::make_pair("", value_node));
  };

  void ExtractBBox(PdfRect bbox, ptree& node, const DataType& data_types) {
    ptree left_node, bottom_node, right_node, top_node;
    left_node.put("", bbox.left);
    node.push_back(std::make_pair("", left_node));
    bottom_node.put("", bbox.bottom);
    node.push_back(std::make_pair("", bottom_node));
    right_node.put("", bbox.right);
    node.push_back(std::make_pair("", right_node));
    top_node.put("", bbox.top);
    node.push_back(std::make_pair("", top_node));
  }

  void ExtractMatrix(const PdfMatrix &matrix, ptree& node, const DataType& data_types) {
    put_array_node(node, matrix.a);
    put_array_node(node, matrix.b);
    put_array_node(node, matrix.c);
    put_array_node(node, matrix.d);
    put_array_node(node, matrix.e);
    put_array_node(node, matrix.f);
  }  

  void ExtractColorState(const PdfColorState &color_state, ptree &node, const DataType &data_types) {
    ptree color_state_node;
    
    if (color_state.fill_color) {
      node.put("fill_color_space", "rgb");
      node.put("fill_color_opacity", color_state.fill_opacity);
      ptree fill_color_node;
      auto fill_color = color_state.fill_color->GetRGB();
      put_array_node(fill_color_node, fill_color.r);
      put_array_node(fill_color_node, fill_color.g);
      put_array_node(fill_color_node, fill_color.b);
      node.put_child("fill_color", fill_color_node);
    }

    if (color_state.stroke_color) {
      node.put("stroke_color_space", "rgb");
      node.put("stroke_color_opacity", color_state.stroke_opacity);
      ptree stroke_color_node;
      auto stroke_color = color_state.stroke_color->GetRGB();
      put_array_node(stroke_color_node, stroke_color.r);
      put_array_node(stroke_color_node, stroke_color.g);
      put_array_node(stroke_color_node, stroke_color.b);
      node.put_child("stroke_color", stroke_color_node);
    }
  }

  void ExtractTextState(PdfTextState *text_state, ptree &node, const DataType &data_types) {
    ptree color_state_node;
    ExtractColorState(text_state->color_state, color_state_node, data_types);
    node.put_child("color_state", color_state_node);

    if (text_state->font) {
      node.put("font_name", EncodeText(text_state->font->GetFontName()));
    }
    node.put("font_size", text_state->font_size);
  }

  void ExtractGraphicState(const PdfGraphicState &graphic_state, ptree &node, const DataType &data_types) {  
    ptree matrix_node;
    ExtractMatrix(graphic_state.matrix, matrix_node, data_types);
    node.put_child("matrix", matrix_node);

    ptree color_state_node;
    ExtractColorState(graphic_state.color_state, color_state_node, data_types);
    node.put_child("color_state", color_state_node);

    node.put("line_width", graphic_state.line_width);
  }

  // render page are into an image
  void RenderPageArea(PdfPage* page, PdfRect& bbox, ptree& node, const DataType &data_types) {

    auto page_view_deleter = [&](PdfPageView *page_view) { page_view->Release(); };
    auto page_view = std::unique_ptr<PdfPageView, 
          decltype(page_view_deleter)>(page->AcquirePageView(
            data_types.render_zoom, data_types.render_rotate), 
          page_view_deleter);
    if (!page_view)
      throw PdfixException();

    // calculate the image bounding box
    PdfDevRect elem_dev_rect;
    page_view->RectToDevice(&bbox, &elem_dev_rect);
    int elem_width = elem_dev_rect.right - elem_dev_rect.left;
    int elem_height = elem_dev_rect.bottom - elem_dev_rect.top;
    if (elem_height == 0 || elem_width == 0)
      return;
    
    // prepare the image 
    PsImage* ps_image = GetPdfix()->CreateImage(page_view->GetDeviceWidth(),
      page_view->GetDeviceHeight(), kImageDIBFormatArgb);
    if (!ps_image)
      throw PdfixException();

    PdfPageRenderParams render_params;
    render_params.image = ps_image;
    page_view->GetDeviceMatrix(&render_params.matrix);
    page->DrawContent(&render_params, nullptr, nullptr);

    PdfImageParams img_params;
    img_params.format = data_types.image_format;

    // save image to memory stream, use CreateFileStream to save image to file
    auto stm = GetPdfix()->CreateMemStream();
    if (!stm)
      throw PdfixException();
    ps_image->SaveRectToStream(stm, &img_params, &elem_dev_rect);
    ps_image->Destroy();

    // save image to ptree as base64 stream
    node.put("base64", PsStreamEncodeBase64(stm));

    stm->Destroy();
  }  

  std::string EncodeText(const std::wstring& text) {
    // https://www.w3schools.com/html/html_charset.asp
    std::wstring replace[] = {
      L"<", L"&#60;",   L">", L"&#62;",   L"&", L"&#38;",   L"\"", L"&#34;",  L"\\", L"&#39;",
      L"�", L"&#162;",  L"�", L"&#163;",  L"�", L"&#165;",  L"�", L"&#128;",  L"�", L"&#169;",
      L"�", L"&#174;",  L"", L""
    };

    std::wstring result = text;
    auto replace_all = [&](const std::wstring &from, const std::wstring &to) {
      size_t pos = 0;
      while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
      }
    };

    int i = 0;
    while (1) {
      if (replace[i].empty()) break;
      replace_all(replace[i], replace[i]);
      i += 2;
    }
    return ToUtf8(result);
  }  
}