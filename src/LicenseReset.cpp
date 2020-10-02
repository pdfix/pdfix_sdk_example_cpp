////////////////////////////////////////////////////////////////////////////////////////////////////
// LicenseReset.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage LicenseReset_cpp
*/
/*!
\page LicenseReset_cpp Reset the license
Example how to reset standard license.
\snippet /LicenseReset.hpp LicenseReset_cpp
*/

#include "pdfixsdksamples/LicenseReset.h"

//! [LicenseReset_cpp]
// system
#include <string>
#include <iostream>
// other libraries
#include "Pdfix.h"

using namespace PDFixSDK;
namespace LicenseReset {
    // write license status into an output stream
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
      
    if (!authorization->Reset())
      throw PdfixException();

    pdfix->Destroy();
  }
}

//! [LicenseReset_cpp]
