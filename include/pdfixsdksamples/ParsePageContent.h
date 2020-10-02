#pragma once

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ParsePageContent {
// ProcessObject gets the value of the object.
void ProcessPageObject(PdsPageObject* obj, std::ostream& ss, std::string indent);

// Iterates all documents bookmars.
void Run(
    const std::wstring& open_path,              // source PDF document
    std::ostream& output,                       // output document
    int page_num
    );
}
