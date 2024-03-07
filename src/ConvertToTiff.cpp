////////////////////////////////////////////////////////////////////////////////////////////////////
// ConvertToTiff.cpp
// Copyright (c) 2022 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ConvertToTiff.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace ConvertToTiff {

void Run(const std::wstring& open_path,  // source PDF document
         const std::wstring& password,   // open password
         const std::wstring& save_path,  // output TIFF file
         PdfTiffParams& tiff_params      // conversion parameters
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
  if (!doc)
    throw PdfixException();

  auto* tiff_conversion = doc->CreateTiffConversion();
  if (!tiff_conversion)
    throw PdfixException();

  if (!tiff_conversion->SetParams(&tiff_params))
    throw PdfixException();

  if (!tiff_conversion->Save(save_path.c_str()))
    throw PdfixException();

  tiff_conversion->Destroy();
  doc->Close();
}
}  // namespace ConvertToTiff
