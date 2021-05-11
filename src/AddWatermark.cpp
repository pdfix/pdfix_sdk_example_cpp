////////////////////////////////////////////////////////////////////////////////////////////////////
// AddWatermark.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/AddWatermark.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Adds watermark from img_path and saves the document to save_path.
void AddWatermark(
  const std::wstring& open_path,                    // source PDF document
  const std::wstring& save_path,                    // path to save PDF docuemnt
  const std::wstring& img_path                      // watermark to apply
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

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
      
  auto image_obj = doc->CreateXObjectFromImage(image_stm, format);
  if (!image_obj)
    throw PdfixException();

  auto page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();

  auto content = page->GetContent();
  if (!content)
    throw PdfixException();

  auto xobjdict = image_obj->GetStreamDict();
  auto scale = 1.5f;
  auto width = xobjdict->GetNumber(L"Width") * scale;
  auto height = xobjdict->GetNumber(L"Height") * scale;
  auto h_value = 20.f;
  auto v_value = 50.f;

  auto matrix = PdfMatrix();
  matrix.a = width;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = height;
  matrix.e = h_value;
  matrix.f = v_value;

  // z order represented by the position in the content, -1 or last add to the top, 0 to the
  // background
  auto imageobject = content->AddNewImage(-1, image_obj, &matrix);

  // set opacity of the image 0-255
  auto graphicState = imageobject->GetGState();
  graphicState.color_state.fill_opacity = 200;
  imageobject->SetGState(&graphicState);

  page->SetContent();

  page->Release();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}