////////////////////////////////////////////////////////////////////////////////////////////////////
// StandardLicenseDeactivate.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage StandardLicenseDeactivate_cpp
*/
/*!
\page StandardLicenseDeactivate_cpp Activate the license
Example how to activate product with the standard license.
\snippet /StandardLicenseDeactivate.hpp StandardLicenseDeactivate_cpp
*/

#include "pdfixsdksamples/StandardLicenseDeactivate.h"

//! [StandardLicenseDeactivate_cpp]
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
