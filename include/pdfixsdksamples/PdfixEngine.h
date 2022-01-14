#pragma once


#include "Pdfix.h"
using namespace PDFixSDK;

class PdfixEngine {

  private:
    static Pdfix* _pdfix;

  public:
    static void Init();

    static void Terminate();

    static Pdfix* Get() { 
      return _pdfix;
    }

};
