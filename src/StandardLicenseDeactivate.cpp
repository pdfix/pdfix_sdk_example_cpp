////////////////////////////////////////////////////////////////////////////////////////////////////
// StandardLicenseDeactivate.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/StandardLicenseDeactivate.h"
// system
#include <string>
#include <iostream>
// other libraries
#include "Pdfix.h"

using namespace PDFixSDK;

namespace StandardLicenseDeactivate {
    // Adds a new text annotation.
  void Run() {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    auto authorization = pdfix->GetStandardAuthorization();
    if (!authorization)
      throw PdfixException();
      
    if (!authorization->Deactivate())
      throw PdfixException();
    
    pdfix->Destroy();
  }
}

//! [StandardLicenseDeactivate_cpp]
