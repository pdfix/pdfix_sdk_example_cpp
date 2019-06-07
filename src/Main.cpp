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
#include "BookmarksToJson.hpp"
#include "ConvertToHtml.hpp"
#include "ConvertToHtmlEx.hpp"
#include "ConvertToXml.hpp"
#include "DigitalSignature.hpp"
#include "DocumentMetadata.hpp"
#include "EmbedFonts.hpp"
#include "ExportFormFieldValues.hpp"
#include "ExtractText.hpp"
#include "ExtractHighlightedText.hpp"
#include "ExtractImages.hpp"
#include "ExtractTables.hpp"
#include "FillForm.hpp"
#include "FlattenAnnots.hpp"
#include "GetBookmarks.hpp"
#include "GetWhitespace.hpp"
#include "Initialization.hpp"
#include "MakeAccessible.hpp"
#include "OcrPageImagesWithTesseract.hpp"
#include "OcrWithTesseract.hpp"
#include "ParsePageContent.hpp"
#include "ParsePdsObjects.hpp"
#include "PrintPage.hpp"
#include "RegexSearch.hpp"
#include "RegexSetPattern.hpp"
#include "RegisterEvent.hpp"
#include "RemoveComments.hpp"
#include "RenderPage.hpp"
#include "RenderPageWithoutText.hpp"
#include "SetFieldFlags.hpp"
#include "SetFormFieldValue.hpp"
#include "TagAnnotation.hpp"
#include "TagAsArtifact.hpp"
#include "TagHeadings.hpp"
#include "TagsEditStructTree.hpp"
#include "TagsReadingOrder.hpp"
#include "TagsReadStructTree.hpp"
#include "TagTableAsFigure.hpp"

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
    Initialization(email, key);
    
    // Markup & Comment
    AddComment(email, key, open_path, output_dir + L"/AddComment.pdf");
    RemoveComments(email, key, open_path, output_dir + L"/RemoveComments.pdf");
    
    // Convert to HTML
    PdfHtmlParams html_params;
    html_params.flags |= (kHtmlNoExternalCSS | kHtmlNoExternalIMG | kHtmlNoExternalJS);
    ConvertToHtml(email, key, open_path, output_dir + L"/fixed.html", config_path,
                  html_params);
    html_params.type = kPdfHtmlResponsive;
    ConvertToHtml(email, key, open_path, output_dir + L"/responsive.html", config_path,
                  html_params);
    PdfHtmlParams html_params_ex;
    html_params.flags |= (kHtmlNoExternalCSS | kHtmlNoExternalIMG | kHtmlNoExternalJS);
    
    // Convert to HTML by parts
    ConvertToHtmlEx(email, key, open_path, output_dir + L"/ConvertToHtmlPage_script.js", config_path,
                    html_params_ex, L"js", L"");
    ConvertToHtmlEx(email, key, open_path, output_dir + L"/ConvertToHtmlPage_style.css", config_path,
                    html_params_ex, L"css", L"");
    ConvertToHtmlEx(email, key, open_path, output_dir + L"/ConvertToHtmlPage_doc.html", config_path,
                    html_params_ex, L"document", L"");
    ConvertToHtmlEx(email, key, open_path, output_dir + L"/ConvertToHtmlPage_page_1.html", config_path,
                    html_params_ex, L"page", L"1");
    
    //PdfTaggedParams tagged_params;
    //ConvertTaggedPdf(email, key, open_path, GetAbsolutePath(L"output/fixed.html"), config_path,
    //  tagged_params);
    DigitalSignature(email, key, open_path, output_dir + L"/DigitalSignature.pdf",
                     resources_dir + L"/test.pfx", L"TEST_PASSWORD");
    DocumentMetadata(email, key, open_path, output_dir + L"/DocumentMetadata.pdf",
                     output_dir + L"/metadata.xml");
    EmbedFonts(email, key, open_path, output_dir + L"/EmbedFonts.pdf");
    
    // Data Extraction
    ExtractHighlightedText(email, key, open_path,
                           output_dir + L"/ExtractHighlightedText.txt", config_path);
    PdfImageParams image_params;
    ExtractImages(email, key, open_path, output_dir + L"/", 800, image_params);
    ExtractTables(email, key, open_path, output_dir + L"/");
    ExtractText(email, key, open_path, output_dir + L"/ExtractText.txt", config_path);
    ConvertToXml(email, key, open_path, output_dir + L"/ConvertToXml.xml", config_path);
    
    // OCR Tesseract
    OcrWithTesseract(email, key, open_path, output_dir + L"/OcrTesseract.pdf",
                     resources_dir + L"/tessdata", L"eng", 2., kRotate0);
    OcrPageImagesWithTesseract(email, key, open_path, output_dir + L"/OcrPageImagesWithTesseract.pdf",
                               resources_dir + L"/tessdata", L"eng", 2., kRotate0);
    
    // Regex
    RegexSearch(email, key, open_path, L"(\\d{4}[- ]){3}\\d{4}");
    RegexSetPattern(email, key, open_path);
    
    RegisterEvent(email, key, open_path);
    
    // Render & Print
    RenderPage(email, key, open_path, output_dir + L"/RenderPage.jpg", 2.0, kRotate0);
    RenderPageWithoutText(email, key, open_path, output_dir + L"/RenderPageWithoutText.jpg", 2.0, kRotate0);
    //PrintPage(email, key, open_path);
    
    // Tags & Accessibility
    AddTags(email, key, open_path, output_dir + L"/AddTags.pdf", config_path);
    TagsReadStructTree(email, key, open_path, output_dir + L"/TagsReadStructTree.txt", config_path);
    TagTableAsFigure::Run(email, key, open_path, output_dir + L"/TagTableAsFigure.pdf");
    TagsReadingOrder::Run(email, key, open_path, output_dir + L"/TagsReadingOrder.pdf");
    TagAnnotation::Run(email, key, open_path, output_dir + L"/TagAnnotation.pdf");
    TagHeadings::Run(email, key, open_path, output_dir + L"/TagHeadings.pdf");
    TagAsArtifact::Run(email, key, open_path, output_dir + L"/TagAsArtifact.pdf");
    TagsEditStructTree(email, key, open_path, output_dir + L"/TagsEditStructTree.pdf");
    MakeAccessible(email, key, open_path, output_dir + L"/MakeAccessible.pdf", L"", L"", config_path);
    
    // Form fields
    ExportFormFieldValues(email, key, open_path,
                          output_dir + L"/ExportFormFieldValues.txt");
    SetFieldFlags(email, key, open_path, output_dir + L"/SetFieldFlags.pdf");
    SetFormFieldValue(email, key, open_path, output_dir + L"/SetFormFieldValue.pdf");
    
    // Misc
    PdfFlattenAnnotsParams flatten_annots_params;
    FlattenAnnots(email, key, open_path, output_dir + L"/FlattenAnnots.pdf",
                  flatten_annots_params);
    BookmarksToJson(email, key, open_path, output_dir + L"/BookmarksToJson.json");
    GetWhitespace(email, key, open_path);
    PdfWatermarkParams watermark_params;
    AddWatermark(email, key, open_path, output_dir + L"/AddWatermark.pdf",
                 resources_dir + L"/watermark.png", watermark_params);
    //      OpedDocumentFromStream(email, key, open_path);
    ParsePdsObjects(email, key, open_path, output_dir + L"/ParsePdsObjects.txt");
    ParsePageContent(email, key, open_path, output_dir + L"/ParsePageContent.txt");
  }
  catch (std::exception& ex) {
    std::cout << "Error: " << ex.what() << std::endl;
	  return 1;
  }
  return 0;
}

