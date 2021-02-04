////////////////////////////////////////////////////////////////////////////////////////////////////
// EditContent.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/EditContent.h"

#include "Pdfix.h"

#include <sstream>
#include <iterator>

using namespace PDFixSDK;

namespace EditContent {
  // edit content
  void EditPageContent(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const std::wstring& img_path, 
    const ObjectProps& object_prop) {

    if (object_prop.path)
      AddPath(doc, content, object_prop);
    if (object_prop.image) 
      AddImage(pdfix, doc, content, img_path, object_prop);
    if (object_prop.text)
      AddText(pdfix, doc, content, object_prop);
  }

  //path
  void AddPath(PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop) {

    auto rgb_colorspace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceRGB);

    auto matrix = PdfMatrix();
    matrix.a = 1;
    matrix.b = 0;
    matrix.c = 0;
    matrix.d = 1;
    matrix.e = object_prop.pos_x;
    matrix.f = object_prop.pos_y;

    auto path_obj = content->AddNewPath(-1, &matrix);
    if (!path_obj)
      throw PdfixException();
    auto rect = PdfRect();
    rect.left = 0;
    rect.bottom = 0;
    rect.right = object_prop.width;
    rect.top = object_prop.height;

    CreateRect(&rect,path_obj);
    path_obj->SetStroke(false);
    path_obj->SetFillType(kFillRuleEvenOdd);

    matrix.f = object_prop.pos_y + 150;
    auto path_obj2 = content->AddNewPath(-1, &matrix);
    if (!path_obj2)
      throw PdfixException();
   
    CreateTriangle(&rect, path_obj2);
    path_obj2->SetStroke(true);
    path_obj2->SetFillType(kFillRuleEvenOdd);

    matrix.f = object_prop.pos_y + 300;
    auto path_obj3 = content->AddNewPath(-1, &matrix);
    if (!path_obj3)
      throw PdfixException();
   
    CreateCircle(&rect, path_obj3);

    path_obj3->SetStroke(true);
    path_obj3->SetFillType(kFillRuleNone);

    
    auto gs = PdfGraphicState();
    auto stroke_color = rgb_colorspace->CreateColor();
    stroke_color->SetValue(0, 1.0f);
    gs.color_state.stroke_color = stroke_color;
    gs.color_state.stroke_opacity = 255;
    gs.line_width = 3.0;
    gs.miter_limit = 1.0;
    gs.line_cap = kPdfLineCapRound;
    gs.line_join = kPdfLineJoinRound;
    auto fill_color = rgb_colorspace->CreateColor();
    fill_color->SetValue(1, 1.0f);
    gs.color_state.fill_color = fill_color;
    gs.color_state.fill_opacity = 255;
    gs.blend_mode = kBlendModeNormal;
    auto matrixgs = PdfMatrix();
    matrixgs.a = 1;
    matrixgs.b = 0;
    matrixgs.c = 0;
    matrixgs.d = 1;
    matrixgs.e = 0;
    matrixgs.f = 0;
    gs.matrix = matrixgs;

    path_obj->SetGState(&gs);
    path_obj2->SetGState(&gs);
    path_obj3->SetGState(&gs);
  }

  void CreatePathFromSvg(std::string svg_path, PdsPath* path_obj) {
    std::istringstream iss(svg_path);
    std::istream_iterator<std::string> it(iss);
    auto end = std::istream_iterator<std::string>();
    for (;it != end; ++it) {
      auto token = *it;
      if (token == "M") {
        PdfPoint pt;
        pt.x = std::stod(*(++it));
        pt.y = std::stod(*(++it));
        path_obj->MoveTo(&pt);
      }else if (token == "L") {
        PdfPoint pt;
        pt.x = std::stod(*(++it));
        pt.y = std::stod(*(++it));
        path_obj->LineTo(&pt);
      } else if (token == "C") {
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
      } else if (token == "A") {
        PdfPoint r;
        r.x = std::stod(*(++it));
        r.y = std::stod(*(++it));

        double x_angle = std::stod(*(++it));

        bool large = std::stoi(*(++it)) != 0;
        bool sweep = std::stoi(*(++it)) != 0;

        PdfPoint to;
        to.x = std::stod(*(++it));
        to.y = std::stod(*(++it));

        path_obj->ArcTo(&to, &r, x_angle, large, sweep);
      } else if (token == "Z") {
        path_obj->ClosePath();
      }
    }
  }

  //rect
  void CreateRect(PdfRect* rect, PdsPath* path_obj) {
    auto ld = PdfPoint();
    auto lu = PdfPoint();
    auto ru = PdfPoint();
    auto rd = PdfPoint();
    ld.x = rect->left;
    ld.y = rect->bottom;
    lu.x = rect->left;
    lu.y = rect->top;
    ru.x = rect->right;
    ru.y = rect->top;
    rd.x = rect->right;
    rd.y = rect->bottom;
    path_obj->MoveTo(&ld);
    path_obj->LineTo(&lu);
    path_obj->LineTo(&ru);
    path_obj->LineTo(&rd);
    path_obj->LineTo(&ld);
    path_obj->ClosePath();
  }

  void CreateTriangle(PdfRect* rect, PdsPath* path_obj) {
    auto x = PdfPoint();
    auto y = PdfPoint();
    auto z = PdfPoint();
    
    x.x = rect->left;
    x.y = rect->bottom;
    y.x = rect->right;
    y.y = rect->bottom;
    z.x = rect->right+((rect->left - rect->right)/2);
    z.y = rect->top;
    
    path_obj->MoveTo(&x);
    path_obj->LineTo(&y);
    path_obj->LineTo(&z);
    path_obj->LineTo(&x);
    path_obj->ClosePath();
  }

  void CreateCircle(PdfRect* rect, PdsPath* path_obj) {

    auto c1 = PdfPoint();
    auto c2 = PdfPoint();
    auto c3 = PdfPoint();
    auto c4 = PdfPoint();
    c1.x = rect->left;
    c1.y = rect->bottom;
    c2.x = rect->left;
    c2.y = rect->top;
    c3.x = rect->right;
    c3.y = rect->top;
    c4.x = rect->right;
    c4.y = rect->bottom;

    auto width = ((c4.x - c1.x) / 2);
    auto height = ((c2.y - c1.y) / 2);

    auto s1 = PdfPoint();
    s1.x = c1.x + (width);
    s1.y = c1.y;
    auto s2 = PdfPoint();
    s2.x = c1.x;
    s2.y = c1.y + (height);
    auto s3 = PdfPoint();
    s3.x = c1.x + (width);
    s3.y = c2.y;
    auto s4 = PdfPoint();
    s4.x = c3.x;
    s4.y = c1.y + (height);

    path_obj->MoveTo(&s1);
    path_obj->CurveTo(&s1, &c1, &s2);
    path_obj->CurveTo(&s2, &c2, &s3);
    path_obj->CurveTo(&s3, &c3, &s4);
    path_obj->CurveTo(&s4, &c4, &s1);
    path_obj->ClosePath();
  }
  
  void CreateArc(PdsPath* path_obj) {
    std::string svg = "M 110 215 A 36 60 0 0 0 150.71 170.29 "
                      "M 110 215 A 36 60 0 0 1 150.71 170.29 "
                      "M 110 215 A 36 60 0 1 0 150.71 170.29 "
                      "M 110 215 A 36 60 0 1 1 150.71 170.29";

    CreatePathFromSvg(svg, path_obj);
  }

  //image
  void AddImage(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const std::wstring& img_path, 
    const ObjectProps& object_prop) {

    auto image_stm = pdfix->CreateFileStream(img_path.c_str(), kPsReadOnly);
    if (!image_stm)
      throw PdfixException();
    PdfImageFormat format = kImageFormatJpg;
    if ((img_path.rfind(L".png") != std::wstring::npos)
      || (img_path.rfind(L".PNG") != std::wstring::npos))
      format = kImageFormatPng;
    auto xobj = doc->CreateXObjectFromImage(image_stm, format);
    auto image_dict = xobj->GetStreamDict();
    auto width = object_prop.width ? object_prop.width : image_dict->GetNumber(L"Width");
    auto height = object_prop.height ? object_prop.height : image_dict->GetNumber(L"Height");
    auto matrix = PdfMatrix();
    matrix.a = width;
    matrix.b = 0;
    matrix.c = 0;
    matrix.d = height;
    matrix.e = object_prop.pos_x;
    matrix.f = object_prop.pos_y;
    auto image_obj = content->AddNewImage(-1, xobj, &matrix);

  }

  //annot appearance 
  void AddStampAnnot(Pdfix* pdfix, PdfDoc* doc, PdfPage* page, const std::wstring& img_path,
    const ObjectProps& object_prop) {

    auto rectAnnot = PdfRect();
    rectAnnot.bottom = 0;
    rectAnnot.left = 0;
    rectAnnot.top = object_prop.height;
    rectAnnot.right = object_prop.width;

    auto annot = page->CreateAnnot(kAnnotSquare, &rectAnnot);
    auto appearance = annot->GetAppearanceXObject(kAppearanceNormal);
    auto matrix = PdfMatrix();
    auto formobj = doc->CreateFormFromObject(appearance, &matrix);
    auto content = formobj->GetContent();
    auto image_stm = pdfix->CreateFileStream(img_path.c_str(), kPsReadOnly);
    if (!image_stm)
      throw PdfixException();
    PdfImageFormat format = kImageFormatJpg;
    if ((img_path.rfind(L".png") != std::wstring::npos)
      || (img_path.rfind(L".PNG") != std::wstring::npos))
      format = kImageFormatPng;
    auto xobj = doc->CreateXObjectFromImage(image_stm, format);
    auto image_dict = xobj->GetStreamDict();
    auto width = object_prop.width ? object_prop.width : image_dict->GetNumber(L"Width");
    auto height = object_prop.height ? object_prop.height : image_dict->GetNumber(L"Height");
    auto matrixImgObj = PdfMatrix();
    matrixImgObj.a = width;
    matrixImgObj.b = 0;
    matrixImgObj.c = 0;
    matrixImgObj.d = height;
    matrixImgObj.e = 0;
    matrixImgObj.f = 0;
    auto image_obj = content->AddNewImage(-1, xobj, &matrixImgObj);
    
    auto contentParams = PdsContentParams();
    auto rectContent = PdfRect();
    rectContent.bottom = 0;
    rectContent.left = 0;
    rectContent.top = height;
    rectContent.right = width;
    contentParams.bbox = rectAnnot;
    contentParams.matrix = matrix;
    contentParams.form_type = 1;
    contentParams.flags = 2;
    auto streamappear = content->ToObject(doc, &contentParams);
    annot->SetAppearanceFromXObject(streamappear, kAppearanceNormal);
    auto added = page->AddAnnot(page->GetNumAnnots(), annot);

  }

  void AddText(Pdfix* pdfix, PdfDoc* doc, PdsContent* content, const ObjectProps& object_prop) {

    auto rgb_colorspace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceRGB);

    auto matrix = PdfMatrix();
    matrix.a = 1;
    matrix.b = 0;
    matrix.c = 0;
    matrix.d = 1;
    matrix.e = object_prop.pos_x;
    matrix.f = object_prop.pos_y;

    auto sys_font = pdfix->FindSysFont(L"Arial", kFontForceBold, PdfFontCodepage::kFontDefANSICodepage);

    if (!sys_font) {
      throw PdfixException();
    }

    auto font = doc->CreateFont(sys_font, PdfFontCharset::kFontAnsiCharset, 0);
    sys_font->Destroy();

    // std::wcout << font->GetFaceName() << std::endl;
    // std::wcout << font->GetFontName() << std::endl;
    // std::wcout << font->GetSystemFontName() << std::endl;

    auto text_obj = content->AddNewText(-1, font, &matrix);
    if (!text_obj)
      throw PdfixException();

    text_obj->SetText(L"Hello world!");

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

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  void Run(
    const std::wstring &open_path,
    const std::wstring & save_path,
    const std::wstring& img_path,
    const ObjectProps& object_prop
    )
  {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
      pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
      pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
      throw std::runtime_error("Incompatible version");

    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();
    
    auto page = doc->AcquirePage(0);
    if (!page)
      throw PdfixException();
    auto content = page->GetContent();
    
    EditPageContent(pdfix, doc, content, img_path, object_prop);

    //AddStampAnnot(pdfix, doc, page, img_path, object_prop);
        
    page->SetContent();

    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();

    page->Release();
    doc->Close();
    pdfix->Destroy();
    
  }
} // namespace EditContent