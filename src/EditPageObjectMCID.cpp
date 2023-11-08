////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/Initialization.h"

#include <iostream>
#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

void EditPageObjectMCID(const std::wstring& open_path,  // source PDF document
                        const std::wstring& save_path   // output PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  auto doc = pdfix->OpenDoc(open_path.c_str(), L"");

  // load first page cotnent and the first page object
  auto page = doc->AcquirePage(0);
  auto content = page->GetContent();
  auto pageObject = content->GetObject(18);

  // get the page object MCID
  auto mcid = pageObject->GetMcid();

  // load the page object content mark and
  // update the tag properties dictionary with new MCID
  auto contentMark = pageObject->GetContentMark();
  auto tagMcid = contentMark->GetTagMcid();
  auto tagDict = contentMark->GetTagObject(tagMcid);
  tagDict->PutNumber(L"MCID", mcid + 1);
  contentMark->SetTagObject(tagMcid, tagDict, false);

  // release page resources
  page->Release();

  // save and close document
  doc->Save(save_path.c_str(), kSaveFull);
  doc->Close();
  pdfix->Destroy();
}
