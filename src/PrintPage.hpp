////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintPage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage PrintPage_cpp
*/
/*!
\page PrintPage_cpp Print Page Sample
Example how to print page.
\snippet /PrintPage.hpp PrintPage_cpp
*/

#pragma once

//! [PrintPage_cpp]
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Pdfix.h"

#if defined (__ANDROID__) || defined (__APPLE__) || defined (__GNUC__)
typedef unsigned char BYTE;
#endif

template<typename T>
struct MallocDeleter {
  void operator() (T* ptr) {
    if (ptr) {
      free(ptr);
      ptr = nullptr;
    }
  }
};
typedef std::unique_ptr<char, MallocDeleter<char> > unique_charp;
typedef std::unique_ptr<wchar_t, MallocDeleter<wchar_t> > unique_wcharp;
typedef std::unique_ptr<BYTE, MallocDeleter<BYTE> > unique_BYTEp;

void PrintPage(
  std::wstring email,                         // authorization email   
  std::wstring license_key,                   // authorization license key
  std::wstring open_path                      // source PDF document
) {
#ifdef _WIN32
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  // find the printer
  DWORD sz = 0;

  GetDefaultPrinter(NULL, &sz);
  unique_wcharp name((wchar_t*)malloc(sz * sizeof(wchar_t) + 1));
  GetDefaultPrinter(name.get(), &sz);

  PRINTER_DEFAULTS pd;
  memset(&pd, 0, sizeof(pd));
  pd.DesiredAccess = PRINTER_ACCESS_USE;

  auto printer_deleter = [=](HANDLE printer) { ClosePrinter(printer); };
  std::unique_ptr<HANDLE, decltype(printer_deleter)> printer(0, printer_deleter);
  HANDLE handle = printer.get();
  if (0 == OpenPrinter(name.get(), &handle, &pd))
    throw std::runtime_error("Error OpenPrinter");

  GetPrinter(handle, 2, 0, 0, &sz);
  unique_BYTEp data_2((BYTE*)malloc(sz));
  PRINTER_INFO_2* pi_2 = (PRINTER_INFO_2*)data_2.get();
  GetPrinter(handle, 2, (LPBYTE)pi_2, sz, &sz);

  if (!pi_2->pDevMode)
    throw std::runtime_error("Printer properties not available");

  int prn_width = pi_2->pDevMode->dmPaperWidth;
  int prn_height = pi_2->pDevMode->dmPaperLength;

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw std::runtime_error(pdfix->GetError());

  PdfRect crop_box;
  page->GetCropBox(&crop_box);

  HDC hdc = CreateDC(pi_2->pDriverName, pi_2->pPrinterName, pi_2->pPortName, NULL);

  int w = GetDeviceCaps(hdc, PHYSICALWIDTH);
  int h = GetDeviceCaps(hdc, PHYSICALHEIGHT);

  auto zoom = std::min(w / (crop_box.right - crop_box.left),
    h / (crop_box.top - crop_box.bottom));
  std::cout << zoom << std::endl;

  DOCINFO di;
  memset(&di, 0, sizeof(di));
  di.cbSize = sizeof(di);
  di.lpszDocName = L"some_file.pdf";

  if (StartDoc(hdc, &di) > 0) {
    if (StartPage(hdc) > 0) {
      // render first page to jpg image
      PdfPageView* page_view = page->AcquirePageView(zoom, kRotate0);
      if (!page_view)
        throw std::runtime_error(pdfix->GetError());

      PdfDevRect clip_rect;
      page_view->RectToDevice(&crop_box, &clip_rect);

      PdfPageRenderParams params;
      params.device = hdc;
      page_view->GetDeviceMatrix(&params.matrix);
      params.clip_rect = clip_rect;
      params.render_flags = kRenderAnnot;// | kRenderGrayscale;
      if (!page->DrawContent(&params, nullptr, nullptr))
        std::cout << pdfix->GetError() << std::endl;

      page->ReleasePageView(page_view);
      EndPage(hdc);
    }
    EndDoc(hdc);
  }
  DeleteDC(hdc);

  doc->ReleasePage(page);
  doc->Close();
  pdfix->Destroy();
#endif
}
//! [PrintPage_cpp]
