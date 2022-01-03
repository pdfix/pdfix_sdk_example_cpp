////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHtmlEx.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertToHtmlEx.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

namespace ConvertToHtmlEx {
  void Run(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& password,       // open password
    const std::wstring& save_path,      // output HTML file
    const std::wstring& config_path,    // configuration
    PdfHtmlParams& html_params,         // conversion parameters
    const std::wstring& param1,         // param 1
    const std::wstring& param2          // param 2
  ) {
    auto pdfix = PdfixEngine::Get();

    // prepare output stream
    PsStream* stm = pdfix->CreateFileStream(save_path.c_str(), kPsTruncate);
    if (!stm)
      throw PdfixException();

    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
    if (!doc)
      throw PdfixException();

    PdfHtmlDoc* html_doc = doc->CreateHtmlDoc();
    if (!html_doc)
      throw PdfixException();
    
    if (param1 == L"js") {
      html_doc->SaveJavaScript(stm);
    }
    else if (param1 == L"css") {
      html_doc->SaveCSS(stm);
    }
    else {

      html_params.flags |= kHtmlNoExternalCSS | kHtmlNoExternalJS | kHtmlNoExternalIMG |
        kHtmlNoExternalFONT;

      if (param1 == L"document") {
        if (!html_doc->SaveDocHtml(stm, &html_params, nullptr, nullptr))
          throw PdfixException();
      }
      else if (param1 == L"page") {
        auto page_num = atoi(ToUtf8(param2).c_str());
        if (page_num == 0)
          throw std::runtime_error("Invalid page num");

        if (!html_doc->SavePageHtml(stm, &html_params, page_num - 1, nullptr, nullptr))
          throw PdfixException();
      }
      html_doc->Close();
      doc->Close();
    }
    
    stm->Destroy();
  }
}