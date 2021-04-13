#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

void FlattenAnnots(
    const std::wstring& open_path,               // source PDF document
    const std::wstring& save_path                // output PDF doucment
    );
