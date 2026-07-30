#pragma once

#include <iostream>
#include <string>

namespace TemplateQueryTestPageObject {

void Run(
    const std::wstring& open_path,  // source PDF document
    std::ostream& output            // summary of TestPageObject results
);

}  // namespace TemplateQueryTestPageObject
