////////////////////////////////////////////////////////////////////////////////////////////////////
// ProgressAndCancelProcess.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

// Example how to monitor process or cancel it

#include "pdfixsdksamples/ProcessControl.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace ProcessControl {

void Run(const std::wstring& open_path  // source PDF document
) {
  auto pdfix = PdfixEngine::Get();

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
    auto progress = event->GetProgressControl();
    //      output << "status: " << (int)(event->GetProgress() * 100) << "%" << std::endl;
    // stop the process after processing half of pages
    if (progress->GetState() > .5) {
      ((process_control*)data)->stop = 1;
    }
  };
  // cancel process callback
  auto cancel_proc = [](void* data) -> int { return ((process_control*)data)->stop; };

  /// TODO: PsProgresscontrol example
  auto progress_control = doc->GetProgressControl();
  progress_control->SetCancelProc(cancel_proc, nullptr);
  auto proc_id = progress_control->StartProcess(3);

  // register progress did change event
  pdfix->RegisterEvent(kEventProgressDidChange, event_proc, &control);

  progress_control->Step(proc_id);

  std::cout << "AddTags" << std::endl;
  // run long process on the document and throw exception if the process was not correctly cancelled
  PdfTagsParams params;
  if (!doc->AddTags(&params)) {
    if (pdfix->GetErrorType() != 6) {
      throw PdfixException();
    }
  }
  progress_control->Step(proc_id);

  pdfix->UnregisterEvent(kEventProgressDidChange, event_proc, &control);
  progress_control->Step(proc_id);
  progress_control->EndProcess(proc_id);

  doc->Close();
}

}  // namespace ProcessControl
