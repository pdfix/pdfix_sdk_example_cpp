////////////////////////////////////////////////////////////////////////////////////////////////////
// StandardLicenseActivate.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/StandardLicenseActivate.h"
// system
#include <string>
#include <iostream>
// other libraries
#include "Pdfix.h"

using namespace PDFixSDK;

namespace StandardLicenseActivate {
    // Adds a new text annotation.
  void Run(
    const std::wstring& license_key                    // authorization license key
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    auto authorization = pdfix->GetStandardAuthorization();
    if (!authorization)
      throw PdfixException();
      
    if (!authorization->Activate(license_key.c_str()))
      throw PdfixException();
    
    pdfix->Destroy();
  }
}