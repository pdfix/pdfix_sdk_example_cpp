////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderPageWithoutText.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage RenderPageWithoutText_cpp
*/
/*!
\page RenderPageWithoutText_cpp Render Page Sample
Example how to render a PDF document page without text into an image.
\snippet /RenderPageWithoutText.hpp RenderPageWithoutText_cpp
*/

#pragma once

//! [RenderPageWithoutText_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"
#include "PdfixEngine.h"

using namespace PDFixSDK;

// set page object render flag
void SetPageObjectsRenderFlag(PdsPageObject* obj) {
  if (obj->GetObjectType() == kPdsPageText) {
    // disable text object rendering
    obj->SetRender(false);
    return;
  }
  
  obj->SetRender(true);  
  if (obj->GetObjectType() == kPdsPageForm) {
    // form object - process child objects
    obj->SetRender(true);
    PdsForm* form = (PdsForm*)obj;
    int num_objects = form->GetNumPageObjects();
    for (int i = 0; i < num_objects; i++)
      SetPageObjectsRenderFlag(form->GetPageObject(i));
  }
}

void RenderPageWithoutText(
  const std::wstring& open_path,              // source PDF document
  const std::wstring& img_path,               // output image
  double zoom,                                // page zoom
  PdfRotate rotate                            // page rotation
) {
  auto pdfix = PdfixEngine::Get();

  // render first page to jpg image
  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();
  PdfPageView* page_view = page->AcquirePageView(zoom, rotate);
  if (!page_view)
    throw PdfixException();

  int width = page_view->GetDeviceWidth();
  int height = page_view->GetDeviceHeight();

  PsImage* image = pdfix->CreateImage(width, height, kImageDIBFormatArgb);
  if (!image)
    throw PdfixException();
  
  // enumerate page elements to disable rendering of the text objects
  auto num_page_objects = page->GetNumPageObjects();
  for (int i = 0; i < num_page_objects; i++) {
    SetPageObjectsRenderFlag(page->GetPageObject(i));
  }

  PdfPageRenderParams params;
  params.image = image;
  page_view->GetDeviceMatrix(&params.matrix);
  params.render_flags = kRenderAnnot; // | kRenderGrayscale;
  if (!page->DrawContent(&params, nullptr, nullptr))
    throw PdfixException();

  auto stream = pdfix->CreateFileStream(img_path.c_str(), kPsTruncate);
  PdfImageParams img_params;
  img_params.format = kImageFormatJpg;
  img_params.quality = 75;
  if (!image->SaveToStream(stream, &img_params))
    throw PdfixException();
  stream->Destroy();

  page->Release();
  doc->Close();
}
//! [RenderPageWithoutText_cpp]
