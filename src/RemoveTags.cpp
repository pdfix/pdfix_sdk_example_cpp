////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveTags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RemoveTags.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void RemoveTags(const std::wstring& open_path,  // source PDF document
                const std::wstring& save_path   // output PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  if (!doc->RemoveTags())
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
}
