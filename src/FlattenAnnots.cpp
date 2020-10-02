////////////////////////////////////////////////////////////////////////////////////////////////////
// FlattenAnnots.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage FlattenAnnots_cpp
*/
/*!
\page FlattenAnnots_cpp Flatten Annots Sample
Example how to flatten annotations into the PDF content.
\snippet /FlattenAnnots.hpp FlattenAnnots_cpp
*/

#include "pdfixsdksamples/FlattenAnnots.h"

//! [FlattenAnnots_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

void FlattenAnnots(
  const std::wstring& open_path,               // source PDF document
  const std::wstring& save_path,               // output PDF doucment
  PdfFlattenAnnotsParams& params               // flatten annotations parameters
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  if (!doc->FlattenAnnots(&params))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}
//! [FlattenAnnots_cpp]
