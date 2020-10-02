#pragma once

//system
#include <string>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//project
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace AcroFormExport {
void ProcessWidget(PdfDoc* doc, PdsDictionary* widget_obj, ptree& json);
void ProcessFormField(PdfDoc* doc, PdfFormField* field, ptree& json, bool widgets);
void Run(
    const std::wstring& open_path,         // source PDF document
    std::ostream& output,                  // output JSON document
    bool widgets                           // include widget annots
    );
}
