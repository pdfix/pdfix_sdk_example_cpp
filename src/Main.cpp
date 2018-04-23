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
  std::wstring email = L"YOUR_EMAIL";                           // authorization email   
  std::wstring key = L"LICENSE_KEY";                            // authorization license key
  std::wstring open_path = GetAbsolutePath(L"test.pdf");        // source PDF document
  std::wstring config_path = GetAbsolutePath(L"config.json");   // configuration file

  try {
    AddComment(email, key, open_path, GetAbsolutePath(L"AddComment.pdf"));
    AddTags(email, key, open_path, GetAbsolutePath(L"AddTags.pdf"), config_path);
    AddWatermark(email, key, open_path, GetAbsolutePath(L"AddWatermark.pdf"), 
      GetAbsolutePath(L"watermark.bmp"), PdfWatermarkParams());
    ConvertToHtml(email, key, open_path, GetAbsolutePath(L"index.html"), config_path, 
      PdfHtmlParams());
    DigitalSignature(email, key, open_path, GetAbsolutePath(L"DigitalSignature.pdf"), 
      GetAbsolutePath(L"cert.pfx"), L"PASSWORD");
    DocumentMetadata(email, key, open_path, GetAbsolutePath(L"DocumentMetadata.pdf"), 
      L"metadata.xml");
    EmbedFonts(email, key, open_path, GetAbsolutePath(L"EmbedFonts.pdf"));
    ExportFormFieldValues(email, key, open_path, GetAbsolutePath(L"ExportFormFieldValues.txt"));
    ExtractImages(email, key, open_path, GetAbsolutePath(L"ExtractImages/"), 800, 
      PdfImageParams());
    ExtractTables(email, key, open_path, GetAbsolutePath(L"ExtractTables/"));
    ExtractText(email, key, open_path, GetAbsolutePath(L"ExtractText.txt"), config_path);
    FlattenAnnots(email, key, open_path, GetAbsolutePath(L"FlattenAnnots.pdf"), 
      PdfFlattenAnnotsParams());
    GetBookmarks(email, key, open_path);
    GetWhitespace(email, key, open_path);
    MakeAccessible(email, key, open_path, GetAbsolutePath(L"MakeAccessible.pdf"),
      GetAbsolutePath(L"tessdata/"), "eng", L"en-US");
    OcrWithTesseract(email, key, open_path, GetAbsolutePath(L"OcrTesseract.pdf"), 
      GetAbsolutePath(L"tessdata/"), "en", OcrTesseractParams());
    PrintPage(email, key, open_path);
    RegexSearch(email, key, open_path, L"(\\d{4}[- ]){3}\\d{4}");
    RegexSetPattern(email, key, open_path);
    RegisterEvent(email, key, open_path);
    RemoveComments(email, key, open_path, GetAbsolutePath(L"RemoveComments.pdf"));
    RenderPage(email, key, open_path, GetAbsolutePath(L"RenderPage/"), 2.0, kRotate0);
    SetFieldFlags(email, key, open_path, GetAbsolutePath(L"SetFieldFlags.pdf"));
    SetFormFieldValue(email, key, open_path, GetAbsolutePath(L"SetFormFieldValue.pdf"));
  }
  catch (std::exception& ex) {
    std::cout << ex.what();
	  return 1;
  }
  return 0;
}

