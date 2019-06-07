////////////////////////////////////////////////////////////////////////////////////////////////////
// DigitalSignature.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage DigitalSignature_cpp
*/
/*!
\page DigitalSignature_cpp PFX Digital Signature Sample
Example how to apply a digital signature from the PFX file.
\snippet /DigitalSignature.hpp DigitalSignature_cpp
*/

#pragma once

//! [DigitalSignature_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

void DigitalSignature(
  const std::wstring& email,                   // authorization email   
  const std::wstring& license_key,             // authorization license key
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path,               // signed PDF document
  const std::wstring& pfx_path,                // pfx file
  const std::wstring& pfx_password             // pfx password
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDigSig* dig_sig = pdfix->CreateDigSig();
  if (!dig_sig)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  dig_sig->SetReason(L"Testing PDFix API");
  dig_sig->SetLocation(L"Location");
  dig_sig->SetContactInfo(L"info@pdfix.net");
  if (!dig_sig->SetPfxFile(pfx_path.c_str(), pfx_password.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  if (!dig_sig->SignDoc(doc, save_path.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
  dig_sig->Destroy();

  doc->Close();
  pdfix->Destroy();
}
//! [DigitalSignature_cpp]
