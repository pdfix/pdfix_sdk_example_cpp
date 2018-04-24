////////////////////////////////////////////////////////////////////////////////////////////////////
// AddComment.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage Initialization_cpp
*/
/*!
\page Initialization_cpp Add Comment Sample
Example how to add a text annotation with a reply.
\snippet /Initialization.hpp Initialization_cpp
*/

#pragma once

//! [Initialization_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

  // Adds a new text annotation.
void Initialization(
  std::wstring email,                   // authorization email   
  std::wstring license_key              // authorization license key
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail.");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
    pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
    pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
    throw std::runtime_error("Incompatible version");

  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());

  // ...

  pdfix->Destroy();
}
//! [Initialization_cpp]