////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeAccessible.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/MakeAccessible.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include "OcrTesseract.h"
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void MakeAccessible(const std::wstring& open_path,           // source PDF document
                    const std::wstring& save_path,           // output PDF/UA document
                    const std::wstring& command_path         // command configuration file
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  auto cmd = doc->GetCommand();
  
  auto stm = pdfix->CreateFileStream(command_path.c_str(), kPsReadOnly);
  if (!stm) {
      throw PdfixException();
  }

  if (!cmd->LoadParamsFromStream(stm, kDataFormatJson)) {
    throw PdfixException();
  }
  stm->Destroy();

  if (!cmd->Run()) {
    throw PdfixException();
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
