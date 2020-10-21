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
  const std::wstring& img_path,                     // watermark to apply
  PdfWatermarkParams& watermark_params              // watermark parameters
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

  // set watermark params
//  watermark_params.h_value = .5;
//  watermark_params.v_value = .3;
//  watermark_params.percentage_vals = true;
//  watermark_params.scale = 1.5;
//  watermark_params.opacity = 0.5;
//  watermark_params.order_top = true;
  
  if (!doc->AddWatermarkFromImage(&watermark_params, image_obj))
    throw PdfixException();
  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}