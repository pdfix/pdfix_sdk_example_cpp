#include "pdfixsdksamples/PdfixEngine.h"


Pdfix* PdfixEngine::_pdfix = nullptr;


Pdfix* PdfixEngine::Init() {
  if (!_pdfix) {
    // current working dirextory must be where the pdfix shared library is stored
    // the module name contains only relative file path name
    
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail. Unable to load pdf.dylib");

    _pdfix = GetPdfix();

    if (_pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR ||
        _pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
        _pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
      throw std::runtime_error("Incompatible version");
  }
  return _pdfix;
}

void PdfixEngine::Terminate() {
  if (_pdfix)
    _pdfix->Destroy();
}

