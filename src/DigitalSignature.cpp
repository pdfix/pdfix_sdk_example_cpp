////////////////////////////////////////////////////////////////////////////////////////////////////
// DigitalSignature.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/DigitalSignature.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void DigitalSignature(
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path,               // signed PDF document
  const std::wstring& pfx_path,                // pfx file
  const std::wstring& pfx_password             // pfx password
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  PdfDigSig* dig_sig = pdfix->CreateDigSig();
  if (!dig_sig)
    throw PdfixException();

  dig_sig->SetReason(L"Testing PDFix API");
  dig_sig->SetLocation(L"Location");
  dig_sig->SetContactInfo(L"info@pdfix.net");
  if (!dig_sig->SetPfxFile(pfx_path.c_str(), pfx_password.c_str()))
    throw PdfixException();
  if (!dig_sig->SignDoc(doc, save_path.c_str()))
    throw PdfixException();
  dig_sig->Destroy();

  doc->Close();
}