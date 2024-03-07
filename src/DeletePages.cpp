////////////////////////////////////////////////////////////////////////////////////////////////////
// DeletePages.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/DeletePages.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

// Delete pages from document.
void DeletePages(const std::wstring& open_file,  // source PDF document
                 const std::wstring& save_file,  // file where to save PDF docuemnt
                 int from,
                 int to) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), L"");
  if (!doc)
    throw PdfixException();

  doc->DeletePages(from, to);
  doc->Save(save_file.c_str(), kSaveFull);
  doc->Close();
}
