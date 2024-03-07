////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToHtmlEx.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertToHtmlEx.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

namespace ConvertToHtmlEx {
void Run(const std::wstring& open_path,    // source PDF document
         const std::wstring& password,     // open password
         const std::wstring& save_path,    // output HTML file
         const std::wstring& config_path,  // configuration
         PdfHtmlParams& html_params,       // conversion parameters
         const std::wstring& param1,       // param 1
         const std::wstring& param2        // param 2
) {
  auto pdfix = PdfixEngine::Get();

  // prepare output stream
  PsStream* stm = pdfix->CreateFileStream(save_path.c_str(), kPsTruncate);
  if (!stm)
    throw PdfixException();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
  if (!doc)
    throw PdfixException();

  auto* html_conv = doc->CreateHtmlConversion();
  if (!html_conv)
    throw PdfixException();

  if (param1 == L"js") {
    html_conv->SaveJavaScript(stm);
  } else if (param1 == L"css") {
    html_conv->SaveCSS(stm);
  } else {
    html_params.flags |=
        kHtmlNoExternalCSS | kHtmlNoExternalJS | kHtmlNoExternalIMG | kHtmlNoExternalFONT;

    if (param1 == L"document") {
      html_params.flags |= kHtmlNoHeadNode | kHtmlNoPagesNode;

      if (!html_conv->SetParams(&html_params))
        throw PdfixException();

      if (!html_conv->SaveToStream(stm))
        throw PdfixException();
    } else if (param1 == L"page") {
      html_params.flags |= kHtmlNoHeadNode | kHtmlNoDocumentNode;

      if (!html_conv->SetParams(&html_params))
        throw PdfixException();

      auto page_num = atoi(ToUtf8(param2).c_str());
      if (page_num == 0)
        throw std::runtime_error("Invalid page num");

      if (!html_conv->AddPage(page_num - 1))
        throw PdfixException();

      if (!html_conv->SaveToStream(stm))
        throw PdfixException();
    }
  }
  html_conv->Destroy();
  doc->Close();

  stm->Destroy();
}
}  // namespace ConvertToHtmlEx
