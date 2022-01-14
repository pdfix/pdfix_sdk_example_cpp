////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderPage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RenderPage.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace RenderPage {

  void Run(
    const std::wstring& open_path,              // source PDF document
    const std::wstring& password,               // open password
    const std::wstring& img_path,               // output image
    PdfImageParams img_params,                  // output image params
    int page_num,                               // page number
    float zoom,                                 // page zoom
    PdfRotate rotate,                           // page rotation
    PdfDevRect clip_rect                        // clip region
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw PdfixException();

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
    if (!doc)
      throw PdfixException();

    // render first page to jpg image
    PdfPage* page = doc->AcquirePage(page_num);
    if (!page)
      throw PdfixException();
    PdfPageView* page_view = page->AcquirePageView(zoom, rotate);
    if (!page_view)
      throw PdfixException();

    int width = page_view->GetDeviceWidth();
    int height = page_view->GetDeviceHeight();
    
    PdfRect clip_box;
    if (!(clip_rect.left == 0 && clip_rect.right == 0 &&
        clip_rect.top == 0 && clip_rect.bottom == 0)) {
      width = clip_rect.right - clip_rect.left;
      height = clip_rect.bottom - clip_rect.top;
      page_view->RectToPage(&clip_rect, &clip_box);
    }
    
    PsImage* image = pdfix->CreateImage(width, height, kImageDIBFormatArgb);
    if (!image)
      throw PdfixException();

    PdfPageRenderParams params;
    params.image = image;
    params.clip_box = clip_box;
    page_view->GetDeviceMatrix(&params.matrix);
    params.render_flags = kRenderAnnot; // | kRenderGrayscale;
    if (!page->DrawContent(&params, nullptr, nullptr))
      throw PdfixException();

    auto stream = pdfix->CreateFileStream(img_path.c_str(), kPsTruncate);
    if (!stream)
      throw PdfixException();
    if (!image->SaveToStream(stream, &img_params))
      throw PdfixException();
    stream->Destroy();

    page->Release();
    doc->Close();

    pdfix->Destroy();
  }
}
