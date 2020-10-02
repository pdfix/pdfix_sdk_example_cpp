#pragma once

//system
#include <string>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//project
#include "Utils.h"
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace AcroFormImport {
void Run(
    const std::wstring& open_path,         // source PDF document
    const std::wstring& save_path,         // destination PDF document
    const std::wstring& json_path          // path to JSON to import
    );
  }
