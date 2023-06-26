////////////////////////////////////////////////////////////////////////////////////////////////////
// Imposition.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/Imposition.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace Imposition {
void Run(const std::wstring& open_path,  // source PDF document
         const std::wstring& save_path   // output PDF doucment
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  // create XObject from a page
  auto page = doc->AcquirePage(0);
  auto xobj = doc->CreateXObjectFromPage(page);
  page->Release();

  // create new page to place form to
  PdfRect bbox = {0, 1000, 1000, 0};
  page = doc->CreatePage(-1, &bbox);
  auto content = page->GetContent();
  PdfMatrix mx1 = {1, 0, 0, 1, 0, 0};
  content->AddNewForm(-1, xobj, &mx1);
  PdfMatrix mx2 = {1, 0, 0, 1, 200, 300};
  content->AddNewForm(-1, xobj, &mx2);
  PdfMatrix mx3 = {1, 0, 0, 1, 400, 600};
  content->AddNewForm(-1, xobj, &mx3);

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
}  // namespace Imposition
