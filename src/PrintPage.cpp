////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintPage.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/PrintPage.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void PrintPage(const std::wstring& open_path  // source PDF document
) {
#ifdef _WIN32
  auto pdfix = PdfixEngine::Get();

  // find the printer
  DWORD sz = 0;
  GetDefaultPrinter(NULL, &sz);

  auto malloc_deleter = [=](void* buf) { free(buf); };
  std::unique_ptr<wchar_t, decltype(malloc_deleter)> name(
      (wchar_t*)malloc(sz * sizeof(wchar_t) + 1), malloc_deleter);
  if (!name)
    throw std::runtime_error("Memory allocation error");

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
  std::unique_ptr<unsigned char, decltype(malloc_deleter)> data_2(
      (unsigned char*)malloc(sz * sizeof(unsigned char) + 1), malloc_deleter);
  if (!data_2)
    throw std::runtime_error("Memory allocation error");

  PRINTER_INFO_2* pi_2 = (PRINTER_INFO_2*)data_2.get();
  GetPrinter(handle, 2, (LPBYTE)pi_2, sz, &sz);

  if (!pi_2->pDevMode)
    throw std::runtime_error("Printer properties not available");

  int prn_width = pi_2->pDevMode->dmPaperWidth;
  int prn_height = pi_2->pDevMode->dmPaperLength;

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();

  PdfRect crop_box;
  page->GetCropBox(&crop_box);

  auto page_width = crop_box.right - crop_box.left;
  auto page_height = crop_box.top - crop_box.bottom;
  auto rotate = page->GetRotate();
  if (rotate / 90 % 2)
    std::swap(page_width, page_height);

  HDC hdc = CreateDC(pi_2->pDriverName, pi_2->pPrinterName, pi_2->pPortName, NULL);

  int w = GetDeviceCaps(hdc, PHYSICALWIDTH);
  int h = GetDeviceCaps(hdc, PHYSICALHEIGHT);

  // calculate zoom and prepare the page view
  auto zoom = std::min(w / page_width, h / page_height);
  PdfPageView* page_view = page->AcquirePageView(zoom, kRotate0);
  if (!page_view)
    throw PdfixException();

  PdfMatrix matrix;
  page_view->GetDeviceMatrix(&matrix);

  // center page on the device
  matrix.e += ((w - page_view->GetDeviceWidth()) / 2.);
  matrix.f += ((h - page_view->GetDeviceHeight()) / 2.);

  DOCINFO di;
  memset(&di, 0, sizeof(di));
  di.cbSize = sizeof(di);
  di.lpszDocName = L"some_file.pdf";

  if (StartDoc(hdc, &di) > 0) {
    if (StartPage(hdc) > 0) {
      PdfDevRect clip_rect;
      page_view->RectToDevice(&crop_box, &clip_rect);

      PdfPageRenderParams params;
      params.device = pdfix->CreateRenderDeviceContext(hdc, kRenderDeviceTypeGDI);
      params.matrix = matrix;
      params.clip_box = crop_box;
      params.render_flags = kRenderAnnot;  // | kRenderGrayscale;
      if (!page->DrawContent(&params))
        std::cout << std::to_string(GetPdfix()->GetErrorType()) << std::endl;

      EndPage(hdc);
    }
    EndDoc(hdc);
  }
  DeleteDC(hdc);

  page->Release();
  doc->Close();
#endif
}
