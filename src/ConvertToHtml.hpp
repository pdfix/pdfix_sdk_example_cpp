////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHtml.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ConvertToHtml_cpp
*/
/*!
\page ConvertToHtml_cpp Pdf To Html Sample
Example how to convert whole PDF document to HTML.
\snippet /ConvertToHtml.hpp ConvertToHtml_cpp
*/

#pragma once

//! [ConvertToHtml_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"
#include "PdfToHtml.h"

PdfToHtml_statics;

// Converts PDF to HTML.
void ConvertToHtml(
  const std::wstring& email,          // authorization email   
  const std::wstring& license_key,    // authorization license key
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path,      // output HTML file
  const std::wstring& config_path,    // configuration file
  PdfHtmlParams& html_params    // conversion parameters
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  // initialize PdfToHtml
  if (!PdfToHtml_init(PdfToHtml_MODULE_NAME))
    throw std::runtime_error("PdfToHtml_init fail");
    
  PdfToHtml* pdf_to_html = GetPdfToHtml();
  if (!pdf_to_html)
    throw std::runtime_error("GetPdfToHtml fail");

  std::cout << "PDFix PDF to HTML " << pdf_to_html->GetVersionMajor() << "." <<
    pdf_to_html->GetVersionMinor() << "." <<
    pdf_to_html->GetVersionPatch() << std::endl;
    
  if (!pdf_to_html->Initialize(pdfix))
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

  PdfHtmlDoc* html_doc = pdf_to_html->OpenHtmlDoc(doc);
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

  if (!html_doc->Save(save_path.c_str(), &html_params, nullptr, nullptr))
    throw std::runtime_error(pdfix->GetError());

  html_doc->Close();
  doc->Close();

  pdf_to_html->Destroy();
  pdfix->Destroy();
}
//! [ConvertToHtml_cpp]