////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateNewDocuments.cpp
// Copyright (c) 2020 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CreateNewDocuments.h"

#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include "Pdfix.h"

using namespace PDFixSDK;

// Creates new documents
void CreateNewDocuments(
  const std::wstring& save_path,                 // directory where to save PDF docuemnts
  size_t document_count,                         // count of documents to be created in the directory
  size_t thread_count                            // maximal number of threads to be used
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

  auto create_doc = [&](size_t doc_count, size_t start_doc) {
    for (size_t i = 0; i < doc_count; i++) {
      std::wstringstream ss;
      ss << save_path << L"document" << (start_doc + i) << L".pdf";

      PdfDoc* doc = pdfix->CreateDoc();
      if (!doc)
        throw PdfixException();

      PdfRect* mBox = new PdfRect();
      mBox->left = 0;
      mBox->right = 595;
      mBox->bottom = 0;
      mBox->top = 842;
      auto page = doc->CreatePage(-1, mBox);
      page->Release();
      if (!doc->Save(ss.str().c_str(), kSaveFull))
        throw PdfixException();
      doc->Close();
    }
  };

  auto div_round_up = [](auto a, auto b) {
    return (a + b - 1) / b;
  };

  std::vector<std::thread> workers;

  size_t available_documents = document_count;
  while (available_documents > 0) {
    size_t available_threads = (thread_count - workers.size());
    size_t thread_doc_count = div_round_up(available_documents, available_threads);
    size_t start_doc = document_count - available_documents;
    std::thread w(create_doc, thread_doc_count, start_doc);
    workers.emplace_back(std::move(w));

    available_documents -= thread_doc_count;
  }

  for (auto& w : workers) {
    w.join();
  }

  pdfix->Destroy();
}