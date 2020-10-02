#pragma once

#include <string>
#include <map>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ReadOCGLayers {
std::map<std::wstring, int> ReadLayerNames(PdsDictionary* root_obj);
void Run(
    const std::wstring& open_file                 // source PDF document
    );
} //namespace ReadOCGLayers
