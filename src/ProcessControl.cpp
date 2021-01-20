////////////////////////////////////////////////////////////////////////////////////////////////////
// ProgressAndCancelProcess.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

// Example how to monitor process or cancel it

#include "pdfixsdksamples/ProcessControl.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ProcessControl{

  void Run(
    const std::wstring& open_path        // source PDF document
  ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();

    // private data to control the process
    struct process_control {
      int stop = 0;
    };
    process_control control;

    // progress event callback
    auto event_proc = [](void* data) -> void {
      auto event = GetPdfix()->GetEvent();
      auto progress = event->GetProgress();
//      output << "status: " << (int)(event->GetProgress() * 100) << "%" << std::endl;
      // stop the process after processing half of pages
      if (progress > .5)
        ((process_control*)data)->stop = 1;
    };
    // cancel process callback
    auto cancel_proc = [](void* data) -> int {
      return ((process_control*)data)->stop;
    };

    // register progress did change event
    pdfix->RegisterEvent(kEventProgressDidChange, event_proc, &control);

    std::cout << "AddTags" << std::endl;
    // run long process on the document and throw exception if the process was not correctly cancelled
    if (!doc->AddTags(cancel_proc, &control)) {
      if (pdfix->GetErrorType() != 6)
        throw PdfixException(); 
    }

    pdfix->UnregisterEvent(kEventProgressDidChange, event_proc, &control);

    doc->Close();
    pdfix->Destroy();
  }

} // namespace

