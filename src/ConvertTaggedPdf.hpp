////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertTaggedPdf.hpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ConvertTaggedPdf_cpp
*/
/*!
\page ConvertTaggedPdf_cpp Tagged Pdf To JSON, XML and HTML Sample
Example how to convert Tagged PDF document to different structured formats.
\snippet /ConvertTaggedPdf.hpp ConvertTaggedPdf_cpp
*/

#pragma once

//! [ConvertTaggedPdf_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"
#include "TaggedPdf.h"

TaggedPdf_statics;

void ConvertTaggedPdf(
  const std::wstring& email,          // authorization email   
  const std::wstring& license_key,    // authorization license key
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
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  // initialize TaggedPdf
  if (!TaggedPdf_init(TaggedPdf_MODULE_NAME))
    throw std::runtime_error("TaggedPdf_init fail");
    
  TaggedPdf* tagged_pdf = GetTaggedPdf();
  if (!tagged_pdf)
    throw std::runtime_error("GetTaggedPdf fail");

  std::cout << "PDFix Tagged PDF" << tagged_pdf->GetVersionMajor() << "." <<
    tagged_pdf->GetVersionMinor() << "." <<
    tagged_pdf->GetVersionPatch() << std::endl;
    
  if (!tagged_pdf->Initialize(pdfix))
    throw std::runtime_error(pdfix->GetError()); 

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  // customize conversion 
  if (!config_path.empty()) {
    PdfDocTemplate* doc_tmpl = doc->GetDocTemplate();
    if (!doc_tmpl)
      throw std::runtime_error(pdfix->GetError());
    PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
    if (stm) {
      if (!doc_tmpl->LoadFromStream(stm, kDataFormatJson))
        throw std::runtime_error(pdfix->GetError());
      stm->Destroy();
    }
  }

  TaggedPdfHtmlDoc* html_doc = tagged_pdf->OpenTaggedPdfHtmlDoc(doc);
  if (!html_doc)
    throw std::runtime_error(pdfix->GetError());

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
    throw std::runtime_error(pdfix->GetError());

  html_doc->Close();
  doc->Close();

  tagged_pdf->Destroy();
  pdfix->Destroy();
}
//! [ConvertTaggedPdf_cpp]