////////////////////////////////////////////////////////////////////////////////////////////////////
// EditContent.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/EditContent.h"

#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

#include <iterator>
#include <sstream>

using namespace PDFixSDK;

namespace EditContent {

PropsBuilder& PropsBuilder::AddText(float x, float y, const std::wstring& text) {
  ObjectProps props;
  props.pos.x = x;
  props.pos.y = y;
  props.obj_type = PdfPageObjectType::kPdsPageText;
  props.data = text;
  m_properties.push_back(props);
  return *this;
}

PropsBuilder& PropsBuilder::AddPath(float x, float y, const std::wstring& svg) {
  ObjectProps props;
  props.pos.x = x;
  props.pos.y = y;
  props.obj_type = PdfPageObjectType::kPdsPagePath;
  props.data = svg;
  m_properties.push_back(props);
  return *this;
}

PropsBuilder& PropsBuilder::AddImage(float x, float y, const std::wstring& path) {
  ObjectProps props;
  props.pos.x = x;
  props.pos.y = y;
  props.obj_type = PdfPageObjectType::kPdsPageImage;
  props.data = path;
  m_properties.push_back(props);
  return *this;
}

std::vector<ObjectProps>& PropsBuilder::Get() {
  return m_properties;
}

// edit content
void EditPageContent(Pdfix* pdfix,
                     PdfDoc* doc,
                     PdsContent* content,
                     const std::vector<ObjectProps>& object_props) {
  for (auto& object_prop : object_props) {
    switch (object_prop.obj_type) {
      case PdfPageObjectType::kPdsPageText:
        AddText(pdfix, doc, content, object_prop);
        break;
      case PdfPageObjectType::kPdsPagePath:
        AddPath(doc, content, object_prop);
        break;
      case PdfPageObjectType::kPdsPageImage:
        AddImage(pdfix, doc, content, object_prop);
        break;
      default:
        throw std::runtime_error("not implemented");
    }
  }
}

void AddText(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop) {
  auto rgb_colorspace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceRGB);

  auto matrix = PdfMatrix();
  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = object_prop.pos.x;
  matrix.f = object_prop.pos.y;

  auto sys_font =
      pdfix->FindSysFont(L"Arial", kFontForceBold, PdfFontCodepage::kFontDefANSICodepage);

  if (!sys_font) {
    throw PdfixException();
  }

  auto font = doc->CreateFont(sys_font, PdfFontCharset::kFontAnsiCharset, 0);
  sys_font->Destroy();

  auto text_obj = content->AddNewText(-1, font, &matrix);
  if (!text_obj)
    throw PdfixException();

  text_obj->SetText(object_prop.data.c_str());

  PdfTextState ts;
  auto stroke_color = rgb_colorspace->CreateColor();
  stroke_color->SetValue(0, 1.0f);
  ts.color_state.stroke_color = stroke_color;
  ts.color_state.stroke_opacity = 255;
  ts.color_state.stroke_type = kFillTypeSolid;

  auto fill_color = rgb_colorspace->CreateColor();
  fill_color->SetValue(1, 1.0f);
  ts.color_state.fill_color = fill_color;
  ts.color_state.fill_opacity = 255;
  ts.color_state.fill_type = kFillTypeSolid;

  ts.font = font;
  ts.font_size = 20;

  ts.char_spacing = 7;
  ts.word_spacing = 10;

  text_obj->SetTextState(&ts);
}

void CreatePathFromSvg(std::wstring svg_path, PdsPath* path_obj) {
  std::wistringstream iss(svg_path);
  std::istream_iterator<std::wstring, wchar_t, std::char_traits<wchar_t>> it(iss);
  auto end = std::istream_iterator<std::wstring, wchar_t, std::char_traits<wchar_t>>();
  for (; it != end; ++it) {
    auto token = *it;
    if (token == L"M") {
      PdfPoint pt;
      pt.x = std::stod(*(++it));
      pt.y = std::stod(*(++it));
      path_obj->MoveTo(&pt);
    } else if (token == L"L") {
      PdfPoint pt;
      pt.x = std::stod(*(++it));
      pt.y = std::stod(*(++it));
      path_obj->LineTo(&pt);
    } else if (token == L"C") {
      PdfPoint cp1;
      cp1.x = std::stod(*(++it));
      cp1.y = std::stod(*(++it));

      PdfPoint cp2;
      cp2.x = std::stod(*(++it));
      cp2.y = std::stod(*(++it));

      PdfPoint pt;
      pt.x = std::stod(*(++it));
      pt.y = std::stod(*(++it));
      path_obj->CurveTo(&cp1, &cp2, &pt);
    } else if (token == L"A") {
      PdfPoint r;
      r.x = std::stod(*(++it));
      r.y = std::stod(*(++it));

      float x_angle = std::stod(*(++it));

      bool large = std::stoi(*(++it)) != 0;
      bool sweep = std::stoi(*(++it)) != 0;

      PdfPoint to;
      to.x = std::stod(*(++it));
      to.y = std::stod(*(++it));

      path_obj->ArcTo(&to, &r, x_angle, large, sweep);
    } else if (token == L"Z") {
      path_obj->ClosePath();
    }
  }
}

// path
void AddPath(PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop) {
  auto rgb_colorspace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceRGB);

  PdfMatrix matrix;
  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = object_prop.pos.x;
  matrix.f = object_prop.pos.y;

  auto path_obj = content->AddNewPath(-1, &matrix);
  if (!path_obj)
    throw PdfixException();

  CreatePathFromSvg(object_prop.data, path_obj);

  path_obj->SetStroke(true);
  path_obj->SetFillType(kFillRuleEvenOdd);

  PdfGraphicState gs;
  // stroke blue color
  auto stroke_color = rgb_colorspace->CreateColor();
  stroke_color->SetValue(0, 0.2f);
  stroke_color->SetValue(1, 0.7f);
  stroke_color->SetValue(2, 0.8f);

  gs.color_state.stroke_color = stroke_color;
  gs.color_state.stroke_opacity = 255;
  gs.color_state.stroke_type = kFillTypeSolid;

  // fill red color
  auto fill_color = rgb_colorspace->CreateColor();
  fill_color->SetValue(0, 0.99f);
  fill_color->SetValue(1, 0.33f);
  fill_color->SetValue(2, 0.33f);
  //  gs.color_state.fill_color = fill_color;
  //  gs.color_state.fill_opacity = 255;
  //  gs.color_state.fill_type = kFillTypeSolid;

  //  gs.line_width = 3.0;
  //  gs.miter_limit = 1.0;
  //  gs.line_cap = kPdfLineCapRound;
  //  gs.line_join = kPdfLineJoinRound;
  //  gs.blend_mode = kBlendModeNormal;

  path_obj->TransformCTM(&matrix);
  //  gs.matrix = matrix;

  //  path_obj->SetGState(&gs);
}

// image
void AddImage(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop) {
  auto& img_path = object_prop.data;
  auto image_stm = pdfix->CreateFileStream(img_path.c_str(), kPsReadOnly);
  if (!image_stm)
    throw PdfixException();
  PdfImageFormat format = kImageFormatJpg;
  if ((img_path.rfind(L".png") != std::wstring::npos) ||
      (img_path.rfind(L".PNG") != std::wstring::npos))
    format = kImageFormatPng;
  auto xobj = doc->CreateXObjectFromImage(image_stm, format, 0);
  auto image_dict = xobj->GetStreamDict();
  auto width = image_dict->GetNumber(L"Width");
  auto height = image_dict->GetNumber(L"Height");
  auto matrix = PdfMatrix();
  matrix.a = width;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = height;
  matrix.e = object_prop.pos.x;
  matrix.f = object_prop.pos.y;
  auto image_obj = content->AddNewImage(-1, xobj, &matrix);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Run(const std::wstring& output_path,              // output PDF document
         const std::vector<ObjectProps>& object_props  // structure containing object properties
) {
  auto pdfix = PdfixEngine::Get();

  auto doc = pdfix->CreateDoc();
  if (!doc)
    throw PdfixException();

  PdfRect mediabox;
  mediabox.left = 0;
  mediabox.top = 792;
  mediabox.right = 621;
  mediabox.bottom = 0;
  auto page = doc->CreatePage(-1, &mediabox);
  if (!page)
    throw PdfixException();
  auto content = page->GetContent();

  EditPageContent(pdfix, doc, content, object_props);

  page->SetContent();

  if (!doc->Save(output_path.c_str(), kSaveFull))
    throw PdfixException();

  page->Release();
  doc->Close();
}
}  // namespace EditContent
