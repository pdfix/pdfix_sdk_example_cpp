////////////////////////////////////////////////////////////////////////////////////////////////////
// SetAnnotationAppearance.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/SetAnnotationAppearance.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace SetAnnotationAppearance {

    // Adds watermark from img_path and saves the document to save_path.
  void Run(
    const std::wstring& open_path,                    // source PDF document
    const std::wstring& save_path,                    // path to save PDF docuemnt
    const std::wstring& img_path                      // image to apply
  ) {
    auto pdfix = PdfixEngine::Get();

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
        
    auto image_obj = doc->CreateXObjectFromImage(image_stm, format, 0);
    if (!image_obj)
      throw PdfixException();
      
    auto page = doc->AcquirePage(0);
    if (!page)
      throw PdfixException();
      
    auto page_view = page->AcquirePageView(1, kRotate0);
    if (!page_view)
      throw PdfixException();
    
    PdfDevRect dev_rect;
    dev_rect.left = page_view->GetDeviceWidth() / 3;
    dev_rect.right = page_view->GetDeviceWidth() - dev_rect.left;
    dev_rect.bottom = page_view->GetDeviceHeight() / 3;
    dev_rect.top = page_view->GetDeviceHeight() - dev_rect.bottom;
    
    PdfRect annot_rect;
    page_view->RectToPage(&dev_rect, &annot_rect);
     
    auto annot = page->GetAnnot(0);
    if (!annot) {
      annot = page->CreateAnnot(kAnnotStamp, &annot_rect);
      page->AddAnnot(0, annot);
    }
      
    
    if (!annot->SetAppearanceFromXObject(image_obj, kAppearanceNormal))
      throw PdfixException();
    
    if (!doc->Save(save_path.c_str(), kSaveFull))
      throw PdfixException();

    page_view->Release();
    page->Release();
    doc->Close();
  }
}