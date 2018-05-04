////////////////////////////////////////////////////////////////////////////////////////////////////
// AddWatermark.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! 
\page CPP_Samples C++ Samples
- \subpage AddWatermark_cpp
*/
/*! 
\page AddWatermark_cpp Add Watermark Sample
// Example how to add a watermark.
\snippet /AddWatermark.hpp AddWatermark_cpp
*/

#pragma once

//! [AddWatermark_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // Adds watermark from img_path and saves the document to save_path.
void AddWatermark(
  const std::wstring& email,                        // authorization email   
  const std::wstring& license_key,                  // authorization license key
  const std::wstring& open_path,                    // source PDF document
  const std::wstring& save_path,                    // path to save PDF docuemnt
  const std::wstring& img_path,                     // watermark to apply
  PdfWatermarkParams& watermark_params              // watermark parameters
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  // set watermark params
  watermark_params.h_value = .5;
  watermark_params.v_value = .3;
  watermark_params.percentage_vals = true;
  watermark_params.scale = 1.5;
  watermark_params.opacity = 0.5;
  watermark_params.order_top = true;
  
  if (!doc->AddWatermarkFromImage(&watermark_params, img_path.c_str()))
    throw std::runtime_error(pdfix->GetError());
  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw std::runtime_error(pdfix->GetError());

  doc->Close();
  pdfix->Destroy();
}

//! [AddWatermark_cpp]