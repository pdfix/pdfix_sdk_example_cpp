#pragma once

// system
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace NamedDestsToJson {
bool ProcessViewDestination(PdfViewDestination* view_dest, PdfDoc* doc, ptree& json);
void ProcessNamedDest(PdsObject* name, PdsObject* value, PdfDoc* doc, ptree& json);
void ProcessNameTreeObject(PdsObject* obj, PdfDoc* doc, ptree& json);
// Extract all documents bookmars into json.
void Run(
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& password,                        // open document password
    std::ostream& output                                 // output stream
    );
}
