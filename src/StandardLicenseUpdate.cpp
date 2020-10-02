////////////////////////////////////////////////////////////////////////////////////////////////////
// StandardLicenseUpdate.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage StandardLicenseUpdate_cpp
*/
/*!
\page StandardLicenseUpdate_cpp Activate the license
Example how to update product standard license.
\snippet /StandardLicenseUpdate.hpp StandardLicenseUpdate_cpp
*/

#include "pdfixsdksamples/StandardLicenseUpdate.h"

//! [StandardLicenseUpdate_cpp]
// system
#include <string>
#include <iostream>
// other libraries
#include "Pdfix.h"

using namespace PDFixSDK;
namespace StandardLicenseUpdate {
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
      
    if (!authorization->Update())
      throw PdfixException();
    
    pdfix->Destroy();
  }
}

//! [StandardLicenseUpdate_cpp]
