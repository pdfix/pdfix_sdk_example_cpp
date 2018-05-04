////////////////////////////////////////////////////////////////////////////////////////////////////
// OcrWithTesseract.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage OcrWithTesseract_cpp
*/
/*!
\page OcrWithTesseract_cpp Ocr With Tesseract Sample
Example how to convert an image based PDF to searchable document.
NOTE: If your tessdata dir is in the /usr/share/tesseract-ocr dir, data_path should be set to /usr/share/tesseract-ocr.
\snippet /OcrWithTesseract.hpp OcrWithTesseract_cpp
*/

#pragma once

//! [OcrWithTesseract_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"
#include "OcrTesseract.h"

OcrTesseract_statics;

  // OCRs the document. 
void OcrWithTesseract(
  const std::wstring& email,                     // authorization email   
  const std::wstring& license_key,               // authorization license key
  const std::wstring& open_path,                 // source PDF document
  const std::wstring& save_path,                 // searchable PDF document
  const std::wstring& data_path,                 // path to OCR data
  const std::string& language,                   // default OCR language
  OcrTesseractParams& ocr_params
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  // initialize OcrTesseract
  if (!OcrTesseract_init(OcrTesseract_MODULE_NAME))
    throw std::runtime_error("OcrTesseract_init fail");

  OcrTesseract* ocr = GetOcrTesseract();
  if (!ocr)
    throw std::runtime_error("GetOcrTesseract fail");

  std::cout << "PDFix OCR Tesseract " << ocr->GetVersionMajor() << "." <<
    ocr->GetVersionMinor() << "." <<
    ocr->GetVersionPatch() << std::endl;

  if (!ocr->Initialize(pdfix))
    throw std::runtime_error(pdfix->GetError());

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());

  ocr->SetLanguage(language.c_str());
  ocr->SetData(data_path.c_str());

  TesseractDoc* ocr_doc = ocr->OpenOcrDoc(doc);
  if (!ocr_doc)
    throw std::runtime_error(pdfix->GetError());

  if (!ocr_doc->Save(save_path.c_str(), &ocr_params, nullptr, nullptr))
    throw std::runtime_error(pdfix->GetError());

  ocr_doc->Close();

  doc->Close();
  pdfix->Destroy();
}
//! [OcrWithTesseract_cpp]