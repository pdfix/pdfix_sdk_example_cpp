#pragma once

// system
#include <string>
#include <iostream>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "Utils.h"
#include "Pdfix.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace BookmarksToJson {
static std::set<PdsObject*> processed_bookmarks;

void ProcessAction(PdfAction* action, PdfDoc* doc, ptree& json);

// ProcessBookmark gets the title of the bookmark if it's not a root.
void ProcessBookmark(PdfBookmark* bmk, PdfDoc* doc, ptree& json);

// Extract all documents bookmars into json.
void Run(
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& password,                        // open document password
    std::ostream& output                                 // output stream
    );
}
