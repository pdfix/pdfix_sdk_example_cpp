////////////////////////////////////////////////////////////////////////////////////////////////////
// CommandDeleteEmptyTags.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CommandDeleteEmptyTags.h"

#include <string>
#include <iostream>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

void CommandDeleteEmptyTags(
  const std::wstring& open_path,        // source PDF document
  const std::wstring& save_path        // output PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  auto command = doc->GetCommand();  
  if (!command)
    throw PdfixException();

  // prepare json params for command
  std::string params = '{ \
        "delete_empty": { "value": "true" }, \
        "delete_invalid": { "value": "true" }, \
        "ignore_table": { "value": "true" }, \
        "ignore_heading": { "value": "false" } \
    }';
  
  auto memStm = pdfix->CreateMemStream();
  memStm->Write(0, (uint8_t*)params.c_str(), params.length());

  if (!command->LoadParamsFromStream(memStm)) 
    throw PdfixException();

  memStm->Destroy();

  // get top level struct element
  auto structTree = doc->GetStructTree();
  if (!structTree) 
    throw PdfixException();

  auto childObj = structTree->GetChildObject(0);
  if (!childObj)
    throw PdfixException();
  auto structElem = structTree->GetStructElementFromObject(childObj);
  if (!structElem)
    throw PdfixException();
  
  if (!command->DeleteEmptyTags(structElem, nullptr, nullptr))
    throw PdfixException();

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();
  doc->Close();
}