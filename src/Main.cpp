///////////////////////////////////////////////////////////////////////////////
// Main.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////
 
#include <exception>
#include <iostream>
#include "Pdfix.h"
#include "PdfToHtml.h"
#include "OcrTesseract.h"

// samples
#include "AddComment.hpp"
#include "AddTags.hpp"
#include "AddWatermark.hpp"
#include "ConvertToHtml.hpp"
#include "DigitalSignature.hpp"
#include "DocumentMetadata.hpp"
#include "EmbedFonts.hpp"
#include "ExportFormFieldValues.hpp"
#include "ExtractText.hpp"
#include "ExtractImages.hpp"
#include "ExtractTables.hpp"
#include "FlattenAnnots.hpp"
#include "GetBookmarks.hpp" 
#include "GetWhitespace.hpp"
#include "Initialization.hpp"
#include "MakeAccessible.hpp"
#include "OcrWithTesseract.hpp"
#include "PrintPage.hpp"
#include "RegexSearch.hpp"
#include "RegexSetPattern.hpp"
#include "RegisterEvent.hpp"
#include "RemoveComments.hpp"
#include "RenderPage.hpp"
#include "SetFieldFlags.hpp"
#include "SetFormFieldValue.hpp"

extern std::wstring GetAbsolutePath(const std::wstring& path);

int main()
{
  std::wstring email = L"YOUR_EMAIL";                                     // authorization email   
  std::wstring key = L"LICENSE_KEY";                                      // authorization license key
  std::wstring open_path = GetAbsolutePath(L"resources/test.pdf");        // source PDF document
  std::wstring config_path = GetAbsolutePath(L"resources/config.json");   // configuration file

  try {
    Initialization(email, key);
    AddComment(email, key, open_path, GetAbsolutePath(L"output/AddComment.pdf"));
    AddTags(email, key, open_path, GetAbsolutePath(L"output/AddTags.pdf"), config_path);
    AddWatermark(email, key, open_path, GetAbsolutePath(L"output/AddWatermark.pdf"), 
      GetAbsolutePath(L"resources/watermark.png"), PdfWatermarkParams());
    ConvertToHtml(email, key, open_path, GetAbsolutePath(L"output/index.html"), config_path, 
      PdfHtmlParams());
    DigitalSignature(email, key, open_path, GetAbsolutePath(L"output/DigitalSignature.pdf"), 
      GetAbsolutePath(L"resources/cert.pfx"), L"PASSWORD");
    DocumentMetadata(email, key, open_path, GetAbsolutePath(L"output/DocumentMetadata.pdf"), 
      L"ouput/metadata.xml");
    EmbedFonts(email, key, open_path, GetAbsolutePath(L"output/EmbedFonts.pdf"));
    ExportFormFieldValues(email, key, open_path, 
      GetAbsolutePath(L"output/ExportFormFieldValues.txt"));
    ExtractImages(email, key, open_path, GetAbsolutePath(L"output/ExtractImages/"), 800, 
      PdfImageParams());
    ExtractTables(email, key, open_path, GetAbsolutePath(L"output/ExtracTables/"));
    ExtractText(email, key, open_path, GetAbsolutePath(L"output/ExtractText.txt"), config_path);
    FlattenAnnots(email, key, open_path, GetAbsolutePath(L"output/FlattenAnnots.pdf"), 
      PdfFlattenAnnotsParams());
    GetBookmarks(email, key, open_path);
    GetWhitespace(email, key, open_path);
    MakeAccessible(email, key, open_path, GetAbsolutePath(L"output/MakeAccessible.pdf"),
      GetAbsolutePath(L"resources/tessdata/"), "eng", L"en-US");
    OcrWithTesseract(email, key, open_path, GetAbsolutePath(L"output/OcrTesseract.pdf"), 
      GetAbsolutePath(L"resources/tessdata/"), "en", OcrTesseractParams());
    PrintPage(email, key, open_path);
    RegexSearch(email, key, open_path, L"(\\d{4}[- ]){3}\\d{4}");
    RegexSetPattern(email, key, open_path);
    RegisterEvent(email, key, open_path);
    RemoveComments(email, key, open_path, GetAbsolutePath(L"output/RemoveComments.pdf"));
    RenderPage(email, key, open_path, GetAbsolutePath(L"output/RenderPage/"), 2.0, kRotate0);
    SetFieldFlags(email, key, open_path, GetAbsolutePath(L"output/SetFieldFlags.pdf"));
    SetFormFieldValue(email, key, open_path, GetAbsolutePath(L"output/SetFormFieldValue.pdf"));
  }
  catch (std::exception& ex) {
    std::cout << ex.what();
	  return 1;
  }
  return 0;
}

