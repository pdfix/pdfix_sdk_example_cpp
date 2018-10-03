////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderPage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage RenderPage_cpp
*/
/*!
\page RenderPage_cpp Render Page Sample
Example how to render a PDF document page into an image.
\snippet /RenderPage.hpp RenderPage_cpp
*/

#pragma once

//! [RenderPage_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

void RenderPage(
  const std::wstring& email,                  // authorization email   
  const std::wstring& license_key,            // authorization license key
  const std::wstring& open_path,              // source PDF document
  const std::wstring& img_path,               // output image
  double zoom,                                // page zoom
  PdfRotate rotate                            // page rotation
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  // render first page to jpg image
  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw std::runtime_error(pdfix->GetError());
  PdfPageView* page_view = page->AcquirePageView(zoom, rotate);
  if (!page_view)
    throw std::runtime_error(pdfix->GetError());

  int width = page_view->GetDeviceWidth();
  int height = page_view->GetDeviceHeight();

  PsImage* image = pdfix->CreateImage(width, height, kImageDIBFormatArgb);
  if (!image)
    throw std::runtime_error(pdfix->GetError());

  PdfPageRenderParams params;
  params.image = image;
  page_view->GetDeviceMatrix(&params.matrix);
  params.render_flags = kRenderAnnot; // | kRenderGrayscale;
  if (!page->DrawContent(&params, nullptr, nullptr))
    throw std::runtime_error(pdfix->GetError());

  auto stream = pdfix->CreateFileStream(img_path.c_str(), kPsTruncate);
  PdfImageParams img_params;
  img_params.format = kImageFormatJpg;
  img_params.quality = 75;
  if (!image->SaveToStream(stream, &img_params))
    throw std::runtime_error(pdfix->GetError());
  stream->Destroy();

  page->ReleasePageView(page_view);
  page->ReleasePageMap();
  doc->ReleasePage(page);
  doc->Close();

  pdfix->Destroy();
}
//! [RenderPage_cpp]