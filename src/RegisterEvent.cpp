////////////////////////////////////////////////////////////////////////////////////////////////////
// RegisterEvent.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RegisterEvent.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

// DocDidOpenCallback gets title when the document is opened.
void DocDidOpenCallback(void* data) {
  PsEvent* event = GetPdfix()->GetEvent();
  if (!event)
    throw std::runtime_error("No event!");
  
  if (event->GetType() != kEventDocDidOpen)
    throw std::runtime_error("This should never happen!");
  
  PdfDoc* doc = event->GetDoc();
  if (doc == nullptr)
    throw std::runtime_error("This should never happen!");
  std::cout << "Document was opened!" << std::endl;
  auto title = doc->GetInfo(L"Title");
  std::wcout << title << std::endl;
}

// DocWillCallback notifies you when the event happens.
void DocWillCallback(void* data) {
  PsEvent* event = GetPdfix()->GetEvent();
  if (!event)
    throw std::runtime_error("No event!");
  

  switch (event->GetType()) {
  case kEventDocWillClose:
    std::cout << "Document will be closed!" << std::endl;
    break;
  case kEventDocWillSave:
    std::cout << "Document will be saved!" << std::endl;
    break;
  }
}

// Registers different kinds of events.
void RegisterEvent(
  const std::wstring& open_path                  // source PDF document
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  // add events
  pdfix->RegisterEvent(kEventDocDidOpen, &DocDidOpenCallback, nullptr);
  pdfix->RegisterEvent(kEventDocWillClose, &DocWillCallback, nullptr);
  pdfix->RegisterEvent(kEventDocWillSave, &DocWillCallback, nullptr);

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();
  doc->Close();
  pdfix->Destroy();
}