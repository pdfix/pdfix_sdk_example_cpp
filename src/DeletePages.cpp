////////////////////////////////////////////////////////////////////////////////////////////////////
// DeletePages.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage DeletePages_cpp
*/
/*!
\page DeletePages_cpp Delete Pages Sample
Example how to delete pages. 
\snippet /DeletePages.hpp DeletePages_cpp
*/

#include "pdfixsdksamples/DeletePages.h"

//! [DeletePages_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Delete pages from document.
void DeletePages(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // file where to save PDF docuemnt
  int from,
  int to
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
  if (!doc)
    throw PdfixException();

  doc->DeletePages(from,to,nullptr,nullptr);
  doc->Save(save_file.c_str(), kSaveFull);
  doc->Close();

  pdfix->Destroy();
}
//! [DeletePages_cpp]
