////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHtml.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertToHtml.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "PdfToHtml.h"

using namespace PDFixSDK;

namespace ConvertToHtml {

  void Run(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& password,       // open password
    const std::wstring& save_path,      // output HTML file
    const std::wstring& config_path,    // configuration file
    PdfHtmlParams& html_params,         // conversion parameters
    const bool preflight                // preflight document template before processing
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    // initialize PdfToHtml
    if (!PdfToHtml_init(PdfToHtml_MODULE_NAME))
      throw std::runtime_error("PdfToHtml_init fail");
      
    auto pdf_to_html = GetPdfToHtml();
    if (!pdf_to_html)
      throw std::runtime_error("GetPdfToHtml fail");

    if (!pdf_to_html->Initialize(pdfix))
      throw PdfixException();

    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
    if (!doc)
      throw PdfixException();

    PdfHtmlDoc* html_doc = pdf_to_html->OpenHtmlDoc(doc);
    if (!html_doc)
      throw PdfixException();

    // initializ edocument template
    auto doc_template = doc->GetTemplate();
    if (!doc_template)
      throw PdfixException();

    if (!config_path.empty()) {
      PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
      if (stm) {
        if (!doc_template->LoadFromStream(stm, kDataFormatJson))
          throw PdfixException();
        stm->Destroy();
      }
    }

    if (preflight) {
      // add reference pages for preflight
      for (auto i = 0; i < doc->GetNumPages(); i++) {
        if (!doc_template->AddPage(i, nullptr, nullptr))
          throw PdfixException();
      }
        
      // run document preflight
      if (!doc_template->Update(nullptr, nullptr))
        throw PdfixException();
    }
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
      throw PdfixException();

    html_doc->Close();
    doc->Close();

    pdf_to_html->Destroy();
    pdfix->Destroy();
  }
}
