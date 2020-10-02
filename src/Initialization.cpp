////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage Initialization_cpp
*/
/*!
\page Initialization_cpp Initialization Sample
Example how to initialize PDFix SDK in c++.
\snippet /Initialization.hpp Initialization_cpp
*/

#include "pdfixsdksamples/Initialization.h"

//! [Initialization_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void Initialization(
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
    pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
    pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
    throw std::runtime_error("Incompatible version");

  // ...

  pdfix->Destroy();
}
//! [Initialization_cpp]
