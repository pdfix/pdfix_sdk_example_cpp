////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertTaggedPdf.hpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertTaggedPdf.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

TaggedPdf_statics;

void ConvertTaggedPdf(
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path,      // output HTML file
  const std::wstring& config_path,    // configuration file
  PdfTaggedParams& params             // conversion parameters
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  // initialize TaggedPdf
  if (!TaggedPdf_init(TaggedPdf_MODULE_NAME))
    throw PdfixException();

  auto tagged_pdf = GetTaggedPdf();
  if (!tagged_pdf)
    throw PdfixException();

  std::cout << "PDFix Tagged PDF" << tagged_pdf->GetVersionMajor() << "." <<
    tagged_pdf->GetVersionMinor() << "." <<
    tagged_pdf->GetVersionPatch() << std::endl;
    
  if (!tagged_pdf->Initialize(pdfix))
    throw PdfixException();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
  
  // customize conversion 
  if (!config_path.empty()) {
    auto doc_tmpl = doc->GetTemplate();
    if (!doc_tmpl)
      throw PdfixException();
    PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
    if (stm) {
      if (!doc_tmpl->LoadFromStream(stm, kDataFormatJson))
        throw PdfixException();
      stm->Destroy();
    }
  }

  TaggedPdfHtmlDoc* html_doc = tagged_pdf->OpenTaggedPdfHtmlDoc(doc);
  if (!html_doc)
    throw PdfixException();

  /* set html_param
  html_params.type = kPdfHtmlResponsive;
  html_params.width = 1200;    
  html_params.flags |= kHtmlExportJavaScripts;
  html_params.flags |= kHtmlExportFonts;
  html_params.flags |= kHtmlRetainFontSize;
  html_params.flags |= kHtmlRetainTextColor;
  html_params.flags |= kHtmlNoExternalCSS | kHtmlNoExternalJS | kHtmlNoExternalIMG | kHtmlNoExternalFONT;
  */

  if (!html_doc->Save(save_path.c_str(), &params, nullptr, nullptr))
    throw PdfixException();

  html_doc->Close();
  doc->Close();

  tagged_pdf->Destroy();
  pdfix->Destroy();
}