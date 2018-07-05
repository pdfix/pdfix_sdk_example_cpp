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
#include "ExtractHighlightedText.hpp"
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

  std::wstring resources_dir = GetAbsolutePath(L"../resources");
  std::wstring output_dir = GetAbsolutePath(L"../output");

  std::wstring open_path = resources_dir + L"/test.pdf";        // source PDF document
  std::wstring config_path = resources_dir + L"/config.json";   // configuration file

  try {
    std::cout << "Initialization" << std::endl;
    Initialization(email, key);

    std::cout << "AddComment" << std::endl;
    AddComment(email, key, open_path, output_dir + L"/AddComment.pdf");

    std::cout << "AddTags" << std::endl;
    AddTags(email, key, open_path, output_dir + L"/AddTags.pdf", config_path);

    std::cout << "AddWatermark" << std::endl;
    PdfWatermarkParams watermark_params;
    AddWatermark(email, key, open_path, output_dir + L"/AddWatermark.pdf", 
      resources_dir + L"/watermark.png", watermark_params);

    std::cout << "ConvertToHtml" << std::endl;
    PdfHtmlParams html_params;
    ConvertToHtml(email, key, open_path, output_dir + L"/index.html", config_path, 
      html_params);

    std::cout << "DigitalSignature" << std::endl;
    DigitalSignature(email, key, open_path, output_dir + L"/DigitalSignature.pdf",
      resources_dir + L"/test.pfx", L"TEST_PASSWORD");

    std::cout << "DocumentMetadata" << std::endl;
    DocumentMetadata(email, key, open_path, output_dir + L"/DocumentMetadata.pdf",
      output_dir + L"/metadata.xml");

    std::cout << "EmbedFonts" << std::endl;
    EmbedFonts(email, key, open_path, output_dir + L"/EmbedFonts.pdf");

    std::cout << "ExportFormFieldValues" << std::endl;
    ExportFormFieldValues(email, key, open_path,
      output_dir + L"/ExportFormFieldValues.txt");

    std::cout << "ExtractHighlightedText" << std::endl;
    ExtractHighlightedText(email, key, open_path, output_dir + L"/ExtractHighlightedText.txt",
      config_path);

    std::cout << "ExtractImages" << std::endl;
    PdfImageParams image_params;
    ExtractImages(email, key, open_path, output_dir + L"/", 800, 
      image_params);

    std::cout << "ExtractTables" << std::endl;
    ExtractTables(email, key, open_path, output_dir + L"/");
    
    std::cout << "ExtractText" << std::endl;
    ExtractText(email, key, open_path, output_dir + L"/ExtractText.txt", config_path);

    std::cout << "FlattenAnnots" << std::endl;
    PdfFlattenAnnotsParams flatten_annots_params;
    FlattenAnnots(email, key, open_path, output_dir + L"/FlattenAnnots.pdf", 
      flatten_annots_params);

    std::cout << "GetBookmarks" << std::endl;
    GetBookmarks(email, key, open_path);
    
    std::cout << "GetWhitespace" << std::endl;
    GetWhitespace(email, key, open_path);
    
    std::cout << "MakeAccessible" << std::endl;
    MakeAccessible(email, key, open_path, output_dir + L"/MakeAccessible.pdf",
      L"en", L"Document Title", config_path);

    std::cout << "OcrWithTesseract" << std::endl;
    OcrTesseractParams ocr_params;
    OcrWithTesseract(email, key, open_path, output_dir + L"/OcrTesseract.pdf", 
      resources_dir, L"eng", ocr_params);

    std::cout << "PrintPage" << std::endl;
    PrintPage(email, key, open_path);

    std::cout << "RegexSearch" << std::endl;
    RegexSearch(email, key, open_path, L"(\\d{4}[- ]){3}\\d{4}");

    std::cout << "RegexSetPattern" << std::endl;
    RegexSetPattern(email, key, open_path);

    std::cout << "RegisterEvent" << std::endl;
    RegisterEvent(email, key, open_path);

    std::cout << "RemoveComments" << std::endl;
    RemoveComments(email, key, open_path, output_dir + L"/RemoveComments.pdf");

    std::cout << "RenderPage" << std::endl;
    RenderPage(email, key, open_path, output_dir + L"/RenderPage.jpg", 2.0, kRotate0);
    
    std::cout << "SetFieldFlags" << std::endl;
    SetFieldFlags(email, key, open_path, output_dir + L"/SetFieldFlags.pdf");
    
    std::cout << "SetFormFieldValue" << std::endl;
    SetFormFieldValue(email, key, open_path, output_dir + L"/SetFormFieldValue.pdf");
  }
  catch (std::exception& ex) {
    std::cout << ex.what();
	  return 1;
  }
  return 0;
}

