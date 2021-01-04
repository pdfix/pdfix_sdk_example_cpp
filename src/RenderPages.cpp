////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderPages.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RenderPages.h"

#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include "Pdfix.h"

using namespace PDFixSDK;

void RenderPages(
  const std::wstring& open_path,              // source PDF document
  const std::wstring& img_path,               // output image
  PdfImageParams img_params,                  // output image params
  int page_from,                              // page from
  int page_to,                                // page to
  double zoom,                                // page zoom
  PdfRotate rotate,                           // page rotation
  PdfDevRect clip_rect,                       // clip region
  size_t thread_count                         // max number of threads
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw PdfixException();

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  auto page_count = doc->GetNumPages();
  if (page_to == -2)
    page_to = page_count - 1;
  if (page_from > page_count || page_to > page_count)
    throw std::runtime_error("Page number out of range");

  auto render_page = [&](auto from, auto to) {
    for (size_t i = from; i <= to; i++) {
      // render first page to jpg image
      PdfPage* page = doc->AcquirePage(i);
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

      std::wstringstream ss;
      ss << img_path << L"page" << (i + 1) << L".png";
      auto stream = pdfix->CreateFileStream(ss.str().c_str(), kPsTruncate);
      if (!stream)
        throw PdfixException();
      if (!image->SaveToStream(stream, &img_params))
        throw PdfixException();
      stream->Destroy();

      page->Release();
    }
  };

  auto div_round_up = [](auto a, auto b) {
    return (a + b - 1) / b;
  };

  std::vector<std::thread> workers;
  size_t last_page = page_from;
  while (last_page <= page_to) {
    size_t available_threads = (thread_count - workers.size());
    size_t pages_left = page_to - last_page + 1;
    size_t page_count = div_round_up(pages_left, available_threads);
    size_t from = last_page;
    size_t to = from + page_count - 1;
    std::thread w(render_page, from, to);
    workers.emplace_back(std::move(w));

    last_page = to + 1;
  }

  for (auto& w : workers) {
    w.join();
  }

  doc->Close();

  pdfix->Destroy();
}