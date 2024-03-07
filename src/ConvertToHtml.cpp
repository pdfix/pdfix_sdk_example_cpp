////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHtml.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertToHtml.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace ConvertToHtml {

void Run(const std::wstring& open_path,      // source PDF document
         const std::wstring& password,       // open password
         const std::wstring& save_path,      // output HTML file
         const std::wstring& config_path,    // configuration file
         PdfHtmlParams& html_params,         // conversion parameters
         const bool preflight,               // preflight document template before processing
         const std::wstring& open_html_path  // file with html to append
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
  if (!doc)
    throw PdfixException();

  auto* html_conv = doc->CreateHtmlConversion();
  if (!html_conv)
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
      if (!doc_template->AddPage(i))
        throw PdfixException();
    }

    // run document preflight
    if (!doc_template->Update())
      throw PdfixException();
  }
  /* set html_param
  html_params.type = kPdfHtmlResponsive;
  html_params.width = 1200;
  html_params.flags |= kHtmlExportJavaScripts;
  html_params.flags |= kHtmlExportFonts;
  html_params.flags |= kHtmlRetainFontSize;
  html_params.flags |= kHtmlRetainTextColor;
  html_params.flags |= kHtmlNoExternalCSS | kHtmlNoExternalJS | kHtmlNoExternalIMG |
  kHtmlNoExternalFONT;
  */

  // load a html file to append
  if (auto stm = pdfix->CreateFileStream(open_html_path.c_str(), kPsReadOnly)) {
    html_conv->AddHtml(stm);
    stm->Destroy();
  }

  if (!html_conv->SetParams(&html_params))
    throw PdfixException();

  if (!html_conv->Save(save_path.c_str()))
    throw PdfixException();

  html_conv->Destroy();
  doc->Close();
}
}  // namespace ConvertToHtml
