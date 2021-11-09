#include "pdfixsdksamples/PdfixEngine.h"


Pdfix* PdfixEngine::_pdfix = nullptr;


void PdfixEngine::Init() {
  if (!_pdfix) {
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    _pdfix = GetPdfix();

    if (_pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR ||
        _pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
        _pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
      throw std::runtime_error("Incompatible version");
  }
}

void PdfixEngine::Terminate() {
  if (_pdfix)
    _pdfix->Destroy();
}

