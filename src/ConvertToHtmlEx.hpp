////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHtmlEx.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ConvertToHtmlEx_cpp
*/
/*!
 \page ConvertToHtmlEx_cpp Pdf To Html Sample #2
Example how to convert PDF to HTML by pages for REST API service.
\snippet /ConvertToHtmlEx.hpp ConvertToHtmlEx_cpp
*/

#pragma once

//! [ConvertToHtmlEx_cpp]
#include <string>
#include <cstdlib>
#include <iostream>
#include "Pdfix.h"
#include "PdfToHtml.h"

extern std::string ToUtf8(const std::wstring& wstr);

void ConvertToHtmlEx(
  const std::wstring& email,          // authorization email   
  const std::wstring& license_key,    // authorization license key
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path,      // output HTML file
  const std::wstring& config_path,    // configuration
  PdfHtmlParams& html_params,         // conversion parameters
  const std::wstring& param1,         // param 1
  const std::wstring& param2          // param 2
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

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
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  
  // prepare output stream
  PsStream* stm = pdfix->CreateFileStream(save_path.c_str(), kPsTruncate);
  if (!stm)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  
  if (param1 == L"js") {
    pdf_to_html->SaveJavaScript(stm);
  }
  else if (param1 == L"css") {
    pdf_to_html->SaveCSS(stm);
  }
  else {
    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    PdfHtmlDoc* html_doc = pdf_to_html->OpenHtmlDoc(doc);
    if (!html_doc)
      throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

    html_params.flags |= kHtmlNoExternalCSS | kHtmlNoExternalJS | kHtmlNoExternalIMG |
      kHtmlNoExternalFONT;

    if (param1 == L"document") {
      if (!html_doc->SaveDocHtml(stm, &html_params, nullptr, nullptr))
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    }
    else if (param1 == L"page") {
      auto page_num = atoi(ToUtf8(param2).c_str());
      if (page_num == 0)
        throw std::runtime_error("Invalid page num");

      if (!html_doc->SavePageHtml(stm, &html_params, page_num - 1, nullptr, nullptr))
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    }
    html_doc->Close();
    doc->Close();
  }
  
  stm->Destroy();
  pdf_to_html->Destroy();
  pdfix->Destroy();
}
//! [ConvertToHtmlEx_cpp]
