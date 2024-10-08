////////////////////////////////////////////////////////////////////////////////////////////////////
// InsertPages.cpp
// Copyright (c) 2021 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/InsertPages.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

// Insert pages from one document to another.
void InsertPages(const std::wstring& source_doc_file,  // source PDF document
                 const std::wstring& dest_doc_file,    // destination PDF document
                 const std::wstring& save_file,        // file where to save PDF docuemnt
                 int where,                            // index where to insert pages
                 int from,                             // from page
                 int to                                // to oage
) {
  auto pdfix = PdfixEngine::Get();

  auto source_doc = pdfix->OpenDoc(source_doc_file.c_str(), L"");
  if (!source_doc)
    throw PdfixException();

  auto dest_doc = pdfix->OpenDoc(dest_doc_file.c_str(), L"");
  if (!dest_doc)
    throw PdfixException();

  auto result = dest_doc->InsertPages(where, source_doc, from, to, kPageInsertAll);
  if (!result)
    throw PdfixException();

  dest_doc->Save(save_file.c_str(), kSaveFull);
  dest_doc->Close();
  source_doc->Close();
}
