#pragma once

#include <memory>
#include "Pdfix.h"
using namespace PDFixSDK;


class PdfixEngine {
  private:
    static Pdfix* _pdfix;

  public:
    static Pdfix* Init();
    static void Terminate();
    static Pdfix* Get() { return _pdfix;}
};

// Pdfix unique_ptr class can be used as a pdfix singleton initialization
// example: PdfixPtr pdfix(PdfixEngine::Init(), pdfix_deleter);
static auto pdfix_deleter = [](Pdfix* pdfix) { pdfix->Destroy(); };
typedef std::unique_ptr<Pdfix, decltype(pdfix_deleter)> PdfixPtr;
