////////////////////////////////////////////////////////////////////////////////////////////////////
// ProgressAndCancelProcess.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

// Example how to monitor process or cancel it

#include "pdfixsdksamples/ProcessControl.h"

#include <iostream>
#include <string>
#include "Pdfix.h"
#include "PdfixErrors.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

extern std::string ToUtf8(const std::wstring& wstr);

namespace ProcessControl {

// helper class to handle process control
class CProgressControl {
  // progress did change event callback
  static void event_proc(void* data) {
    auto event = GetPdfix()->GetEvent();
    auto progress = event->GetProgressControl();
    auto string = progress->GetText();

    std::cout << "Progress status: " << progress->GetState() << " text: " << ToUtf8(string)
              << std::endl;
    // stop the process after reaching 60%
    if (progress->GetState() > .6) {
      ((CProgressControl*)data)->request_stop();
    }
  };

  // cancel process callback
  static int cancel_proc(void* data) { return ((CProgressControl*)data)->stop(); };

 public:
  CProgressControl(PdfDoc* doc, int steps) : m_doc(doc) {
    // register progress did change event
    GetPdfix()->RegisterEvent(kEventProgressDidChange, event_proc, this);

    auto progress_control = doc->GetProgressControl();
    progress_control->SetCancelProc(cancel_proc, this);
    m_id = progress_control->StartProcess(steps);
  }
  ~CProgressControl() {
    // terminate the process and stop receiving signals
    auto progress_control = m_doc->GetProgressControl();
    progress_control->EndProcess(m_id);
    GetPdfix()->UnregisterEvent(kEventProgressDidChange, event_proc, this);
  }
  void step() {
    auto progress_control = m_doc->GetProgressControl();
    if (!progress_control->Step(m_id)) {
      throw PdfixException();
    }
  }
  float state() {
    auto progress_control = m_doc->GetProgressControl();
    return progress_control->GetState();
  }
  void request_stop() { m_stop = true; }
  bool stop() { return m_stop; }
  void set_text(const std::wstring& text) {
    auto progress_control = m_doc->GetProgressControl();
    progress_control->SetText(text.c_str());
  }

 private:
  PdfDoc* m_doc = nullptr;
  int m_id = -1;        // progess id
  bool m_stop = false;  // indicator to stop the process
};

void Run(const std::wstring& open_path  // source PDF document
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  // start the document progress control
  std::unique_ptr<CProgressControl> progress_control(new CProgressControl(doc, 3));
  progress_control->set_text(L"Starting prgress...");
  progress_control->step();  // proceed to next step

  progress_control->set_text(L"Running AutoTag...");
  // Run long process on the document. The process connects to stared progress control and emits
  // kEventProgressDidChange signals
  PdfTagsParams params;
  if (!doc->AddTags(&params)) {
    if (pdfix->GetErrorType() == kErrorOperationCancelled) {
      progress_control->set_text(L"Process cancelled ...");
    } else {
      throw PdfixException();
    }
  }

  progress_control.reset();  // cleanup the progress control before closing the document

  doc->Close();
}

}  // namespace ProcessControl
