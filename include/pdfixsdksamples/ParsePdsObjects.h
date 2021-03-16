#pragma once

#include <string>
#include <map>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace ParsePdsObjects {

  // ProcessObject gets the value of the object.
  void ProcessObject(PdsObject* obj, std::ostream& ss, std::string indent, std::map<PdsObject*, int>& mapped);

  // Iterates all documents bookmars.
  void Run(
    const std::wstring& open_path,           // source PDF document
    const std::wstring& password,            // source PDF document
    std::ostream& output                     // output document
        );
}
