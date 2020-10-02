#pragma once

#include <string>
#include <sstream>
#include "Pdfix.h"

using namespace PDFixSDK;

// HasHighlight rerturns true if there is an highlight annotation over the char_rect
bool HasHighlight(PdfPage* page, PdfRect& char_rect);
// GetHighlightedText processes each element recursively.
// If the element is a highlighted text, saves it to the output stream.
void GetHighlightedText(PdfPage* page, PdeElement* element, std::stringstream& ss);
// Extracts texts from the document and saves them to TXT format.
void ExtractHighlightedText(
    const std::wstring& open_path,      // source PDF document
    const std::wstring& save_path,      // output TXT file
    const std::wstring& config_path     // configuration file
    );
