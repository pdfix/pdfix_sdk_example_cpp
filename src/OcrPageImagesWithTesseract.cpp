////////////////////////////////////////////////////////////////////////////////////////////////////
// OcrPageImagesWithTesseract.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/OcrPageImagesWithTesseract.h"

#include <string>
#include <iostream>
#include <vector>
#include "Pdfix.h"
#include "OcrTesseract.h"

using namespace PDFixSDK;

#define kPi 3.1415926535897932384626433832795f

extern void PdfMatrixRotate(PdfMatrix& m, float radian, bool prepend);
extern void PdfMatrixScale(PdfMatrix& m, float sx, float sy, bool prepend);
extern void PdfMatrixTranslate(PdfMatrix& m, float x, float y, bool prepend);

void parse_page_element(PdeElement* elem, std::vector<PdfRect>& image_bbox_arr) {
  if (!elem)
    return;
  if (elem->GetType() == kPdeImage) {
    PdfRect bbox = elem->GetBBox();
    image_bbox_arr.push_back(bbox);
  }
  else {
    for (int i = 0; i < elem->GetNumChildren(); i++) {
      PdeElement* child = elem->GetChild(i);
      parse_page_element(child, image_bbox_arr);
    }
  }
}

void OcrPageImagesWithTesseract(
  const std::wstring& open_path,                  // source PDF document
  const std::wstring& save_path,                  // searchable PDF document
  const std::wstring& data_path,                  // path to OCR data
  const std::wstring& language,                   // default OCR language
  const float zoom,                              // zoom to control page rendering quality
  const PdfRotate rotate                          // page rotation to be applied
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  // initialize OcrTesseract
  if (!OcrTesseract_init(OcrTesseract_MODULE_NAME))
    throw std::runtime_error("OcrTesseract_init fail");

  OcrTesseract* ocr = GetOcrTesseract();
  if (!ocr)
    throw std::runtime_error("GetOcrTesseract fail");

  if (!ocr->Initialize(pdfix))
    throw PdfixException();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
  
  // collect page images
  std::vector<PdfRect> image_bbox_arr;
  
  PdfPage* page = doc->AcquirePage(0);
  if (!page)
    throw PdfixException();

  // find images on the page and collect bounding boxes to ocr
  PdePageMap* page_map = page->AcquirePageMap();
  if (!page_map)
    throw PdfixException();
  if (!page_map->CreateElements(nullptr, nullptr))
    throw PdfixException();
    
  PdeElement* elem = page_map->GetElement();
  parse_page_element(elem, image_bbox_arr);
  
  page_map->Release();

  // setup the ocr engine
  ocr->SetLanguage(language.c_str());
  ocr->SetDataPath(data_path.c_str());

  TesseractDoc* ocr_doc = ocr->OpenOcrDoc(doc);
  if (!ocr_doc)
    throw PdfixException();
  
  // prepare page rendering matrix
  PdfPageView* page_view = page->AcquirePageView(zoom, rotate);
  
  // run ocr on each image bbox
  for (auto& bbox : image_bbox_arr) {
    // render page to image
    PdfDevRect dev_rect;
    page_view->RectToDevice(&bbox, &dev_rect);
    int width = dev_rect.right - dev_rect.left;
    int height = dev_rect.bottom - dev_rect.top;
    
    PsImage* image = pdfix->CreateImage(width, height, kImageDIBFormatArgb);

    // render portion of the page - the image
    PdfPageRenderParams render_params;
    page_view->GetDeviceMatrix(&render_params.matrix);
    render_params.image = image;
    render_params.clip_box = bbox;
    if (!page->DrawContent(&render_params, nullptr, nullptr))
      throw PdfixException();
    
    // calculate PdfMatrix to position the recognized text on the page
    auto rotate = ((page->GetRotate() / 90) % 4);
    PdfMatrix matrix;
    PdfMatrixRotate(matrix, rotate * kPi / 2, false);
    PdfMatrixScale(matrix, 1/zoom, 1/zoom, false);
    switch (rotate) {
      case 0: PdfMatrixTranslate(matrix, bbox.left, bbox.bottom, false); break;
      case 1: PdfMatrixTranslate(matrix, bbox.right, bbox.bottom, false); break;
      case 2: PdfMatrixTranslate(matrix, bbox.right, bbox.top, false); break;
      case 3: PdfMatrixTranslate(matrix, bbox.left, bbox.top, false); break;
    }
    
    if (!ocr_doc->OcrImageToPage(image, &matrix, page, nullptr, nullptr))
      throw PdfixException();
    
    image->Destroy();
  }
  
  page->Release();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  ocr_doc->Close();
  ocr->Destroy();

  doc->Close();
  pdfix->Destroy();
}