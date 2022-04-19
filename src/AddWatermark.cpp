////////////////////////////////////////////////////////////////////////////////////////////////////
// AddWatermark.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AddWatermark.h"
#include "pdfixsdksamples/Utils.h"
#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

  // Adds watermark from img_path and saves the document to save_path.
void AddWatermark(
  const std::wstring& open_path,                    // source PDF document
  const std::wstring& save_path,                    // path to save PDF docuemnt
  const std::wstring& img_path,                     // watermark to apply
  int start_page,                                   // start page number
  int end_page,                                     // end page number
  int order_top,                                    // number specifying where in the page z-order the watermark should be added
  bool percentage_vals,                             // boolean specifying the units of horizValue and vertValue
  PdfAlignment h_align,                             // the horizontal alignment to be used when adding the watermark to a page
  PdfAlignment v_align,                             // the vertical alignment to be used when adding the watermark to a page
  float h_value,                                    // the horizontal offset value to be used when adding the watermark on a page
  float v_value,                                    // the vertical offset value to be used when adding the watermark on a page
  float scale,                                      // the scale factor to be used when adding the watermark, with 1.0 meaning 100%
  float rotation,                                   // the counter-clockwise rotation, in degrees, to be used when adding the watermark
  float opacity                                     // the opacity to be used when adding the watermark
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
    
  auto image_stm = pdfix->CreateFileStream(img_path.c_str(), kPsReadOnly);
  if (!image_stm)
    throw PdfixException();
    
  PdfImageFormat format = kImageFormatJpg;
  if ( (img_path.rfind(L".png") != std::wstring::npos)
    || (img_path.rfind(L".PNG") != std::wstring::npos) )
    format = kImageFormatPng;
      
  auto image_obj = doc->CreateXObjectFromImage(image_stm, format, 0);
  if (!image_obj)
    throw PdfixException();

  auto page_num = doc->GetNumPages();
  if (end_page == -1 || end_page>=page_num)
    end_page = page_num - 1;

  if (start_page>=page_num || end_page<start_page)
    throw std::runtime_error("Page number out of range");

  for (int i=start_page; i<=end_page; i++) {
    auto page = doc->AcquirePage(i);
    if (!page)
      throw PdfixException();

    auto content = page->GetContent();
    if (!content)
      throw PdfixException();

    auto xobjdict = image_obj->GetStreamDict();
    auto width = xobjdict->GetNumber(L"Width");
    auto height = xobjdict->GetNumber(L"Height");

    // prepare the matrix
    auto page_mx = page->GetDefaultMatrix();
    auto crop_rect = page->GetCropBox();
    PdfMatrix page_mx_rev;
    PdfMatrixInverse(page_mx_rev, page_mx);

    // scale
    auto width_scaled = width * scale;
    auto height_scaled = height * scale;

    auto matrix = PdfMatrix();
    matrix.a = width_scaled;
    matrix.b = 0;
    matrix.c = 0;
    matrix.d = height_scaled;
    matrix.e = 0;
    matrix.f = 0;

    if (rotation != 0.0f) {
      PdfMatrixTranslate(matrix, -width_scaled / 2, -height_scaled / 2, false);
      PdfMatrixRotate(matrix, (rotation / 180.0f) * kPi, false);
      PdfMatrixTranslate(matrix, width_scaled / 2, height_scaled / 2, false);
    }

    // horizontal align
    auto rect_h = crop_rect.right - crop_rect.left;
    auto rect_v = crop_rect.top - crop_rect.bottom;
    if (h_align == kAlignmentCenter)
      PdfMatrixTranslate(matrix, (rect_h - width_scaled) / 2, 0.0f, false);
    else if (h_align == kAlignmentRight)
      PdfMatrixTranslate(matrix, (rect_h - width_scaled), 0.0f, false);

    // vertical align
    if (v_align == kAlignmentCenter)
      PdfMatrixTranslate(matrix, 0.0f, (rect_v - height_scaled) / 2, false);
    else if (v_align == kAlignmentTop)
      PdfMatrixTranslate(matrix, 0.0f, (rect_v - height_scaled), false);

    // horizontal and vertical offset
    auto offs_h = percentage_vals ? rect_h * h_value : h_value;
    auto offs_v = percentage_vals ? rect_v * v_value : v_value;

    //-offs_v because y coordinate from top to bottom
    PdfMatrixTranslate(matrix, offs_h, -offs_v, false);

    auto position = order_top == 1 ? -1 : 0;
    auto imageobject = content->AddNewImage(position, image_obj, &matrix);

    // set opacity of the image 0-255
    auto graphicState = imageobject->GetGState();
    graphicState.color_state.fill_opacity = opacity * 255 ;
    imageobject->SetGState(&graphicState);

    page->SetContent();

    page->Release();
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}