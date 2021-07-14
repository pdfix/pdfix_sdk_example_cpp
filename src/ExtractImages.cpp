////////////////////////////////////////////////////////////////////////////////////////////////////
// ExtractImages.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ExtractImages.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

// SaveImage processes each element recursively. If the element is an image, it saves it to save_path.
void SaveImage(PdeElement* element, 
  const std::wstring& save_path, 
  PdfImageParams& img_params,
  PdfPage* page, 
  PdfPageView* page_view, 
  int& image_index) {

  Pdfix* pdfix = GetPdfix();
    
  PdfElementType elem_type = element->GetType();
    
  if (elem_type == kPdeImage) {
    PdeImage* image = static_cast<PdeImage*>(element);
      
    PdfRect elem_rect = element->GetBBox();
    PdfDevRect elem_dev_rect;
    page_view->RectToDevice(&elem_rect, &elem_dev_rect);
    int elem_width = elem_dev_rect.right - elem_dev_rect.left;
    int elem_height = elem_dev_rect.bottom - elem_dev_rect.top;
    if (elem_height == 0 || elem_width == 0)
      return;
      
    PsImage* ps_image = pdfix->CreateImage(page_view->GetDeviceWidth(),
      page_view->GetDeviceHeight(), kImageDIBFormatArgb);
    if (!ps_image)
      throw PdfixException();

    PdfPageRenderParams render_params;
    render_params.image = ps_image;
    page_view->GetDeviceMatrix(&render_params.matrix);
    page->DrawContent(&render_params, nullptr, nullptr);
      
    std::wstring path = save_path + L"/ExtractImages_" + std::to_wstring(image_index++) + L".png";
    ps_image->SaveRect(path.c_str(), &img_params, &elem_dev_rect);
    ps_image->Destroy();
  }

  int count = element->GetNumChildren();
  if (count == 0)
    return;
  for (int i = 0; i < count; i++) {
    PdeElement* child = element->GetChild(i);
    if (child)
      SaveImage(child, save_path, img_params, page, page_view, image_index);
  }
}

// Extracts all images from the document and saves them to save_path.
void ExtractImages(
  const std::wstring& open_path,                // source PDF document
  const std::wstring& save_path,                // directory where to extract images
  int render_width,                             // with of the rendered page in pixels (image )
  PdfImageParams& img_params                    // image parameters
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  std::cout << "PDFix " << pdfix->GetVersionMajor() << "." <<
    pdfix->GetVersionMinor() << "." <<
    pdfix->GetVersionPatch() << std::endl;

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  img_params.format = kImageFormatPng;
  int image_index = 1;

  auto num_pages = doc->GetNumPages();

  for (auto i = 0; i < num_pages; i++) {
    std::cout << std::endl;
    std::cout << "Processing pages..." << i + 1 << "/" << num_pages;

    PdfPage* page = doc->AcquirePage(i);
    if (!page)
      throw PdfixException();

    PdfRect crop_box;
    page->GetCropBox(&crop_box);
    float page_width = (crop_box.right - crop_box.left);
    float zoom = render_width / page_width;
    PdfPageView* page_view = page->AcquirePageView(zoom, kRotate0);
    if (!page_view)
      throw PdfixException();

    PdePageMap* page_map = page->AcquirePageMap();
    if (!page_map)
      throw PdfixException();
    if (!page_map->CreateElements(nullptr, nullptr))
      throw PdfixException();

    auto element = page_map->GetElement();
    if (!element)
      throw PdfixException();
    SaveImage(element, save_path.c_str(), img_params, page, page_view, image_index);

    page->Release();
  }
  std::cout << std::endl << image_index - 1 << " images found" << std::endl;

  doc->Close();
  pdfix->Destroy();
}