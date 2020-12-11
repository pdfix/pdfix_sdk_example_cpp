///////////////////////////////////////////////////////////////////////////////
// Main.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/samples.h"
 
#include "pdfixsdksamples/ConvertRGBToCMYK.h"

extern std::wstring GetAbsolutePath(const std::wstring& path);

int main()
{
  std::wstring resources_dir = GetAbsolutePath(L"../../resources");
  std::wstring output_dir = GetAbsolutePath(L"../../output");

  std::wstring open_path = resources_dir + L"/test.pdf";        // source PDF document
  std::wstring password = L"";                                  // open password
  //std::wstring save_path = output_dir + L"/test.pdf";        // source PDF document
  std::wstring config_path = resources_dir + L"/config.json";   // configuration file

  try {
    Initialization();

    ConvertRGBToCMYK(open_path, output_dir + L"RGB2CMYK.pdf");
    
    /*
    // Accessibility and PDF Tagging samples
    MakeAccessible(open_path, output_dir + L"/MakeAccessible.pdf", 
      std::make_pair(false, L""), std::make_pair(true, L"Document title"), 
      config_path,
      false);

    AddTags(open_path, output_dir + L"/AddTags.pdf", config_path, true);

    // TagsReadStructTree(open_path, output_dir + L"/TagsReadStructTree.txt", config_path);
    // TagTableAsFigure::Run(open_path, output_dir + L"/TagTableAsFigure.pdf");
    // TagsReadingOrder::Run(open_path, output_dir + L"/TagsReadingOrder.pdf");
    // TagAnnotation::Run(open_path, output_dir + L"/TagAnnotation.pdf");
    // TagHeadings::Run(open_path, output_dir + L"/TagHeadings.pdf");
    // TagAsArtifact::Run(open_path, output_dir + L"/TagAsArtifact.pdf");
    // TagsEditStructTree(open_path, output_dir + L"/TagsEditStructTree.pdf");

    // Data Extraction samples
    ExtractData::DataType extract_data; 
    extract_data.doc_info = true;       // extract document info
    extract_data.page_map = true;       // extract page map data for data scraping
    extract_data.extract_text = true;   // extract text
    ExtractData::Run(open_path, password, config_path, std::cout, extract_data, true, kDataFormatJson);

    PdfImageParams image_params;
    ExtractImages(open_path, output_dir + L"/", 800, image_params);
    ExtractTables(open_path, output_dir + L"/");
    ExtractHighlightedText(open_path, output_dir + L"/ExtractHighlightedText.txt", config_path);

    // PDF to HTML samples
    PdfHtmlParams html_params;
    html_params.flags |= (kHtmlNoExternalCSS | kHtmlNoExternalIMG | kHtmlNoExternalJS);
    ConvertToHtml::Run(open_path, password, output_dir + L"/fixed.html", config_path, html_params, true);
    html_params.type = kPdfHtmlResponsive;
    ConvertToHtml::Run(open_path, password, output_dir + L"/responsive.html", config_path, html_params, true);

    PdfHtmlParams html_params_ex;
    html_params_ex.flags |= (kHtmlNoExternalCSS | kHtmlNoExternalIMG | kHtmlNoExternalJS);
    ConvertToHtmlEx::Run(open_path, password, output_dir + L"/ConvertToHtmlPage_script.js", config_path,
                   html_params_ex, L"js", L"");
    ConvertToHtmlEx::Run(open_path, password, output_dir + L"/ConvertToHtmlPage_style.css", config_path,
                   html_params_ex, L"css", L"");
    ConvertToHtmlEx::Run(open_path, password, output_dir + L"/ConvertToHtmlPage_doc.html", config_path,
                   html_params_ex, L"document", L"");
    ConvertToHtmlEx::Run(open_path, password, output_dir + L"/ConvertToHtmlPage_page_1.html", config_path,
                   html_params_ex, L"page", L"1");

    // Markup & Comment
    AddComment(open_path, output_dir + L"/AddComment.pdf");
    
    RemoveComments(open_path, output_dir + L"/RemoveComments.pdf");

    PdfFlattenAnnotsParams flatten_annots_params;
    FlattenAnnots(open_path, output_dir + L"/FlattenAnnots.pdf", flatten_annots_params);

    // Render & Print
    PdfDevRect clip_area;
    RenderPage::Run(open_path, password, output_dir + L"/RenderPage.jpg", image_params, 1, 1.0, kRotate0, clip_area);

    // Signing and form-filling
    DigitalSignature(open_path, output_dir + L"/DigitalSignature.pdf", resources_dir + L"/test.pfx", L"TEST_PASSWORD");
    PdfWatermarkParams watermark_params;
    AddWatermark(open_path, output_dir + L"/AddWatermark.pdf", resources_dir + L"/watermark.png", watermark_params);
    ExportFormFieldValues(open_path, output_dir + L"/ExportFormFieldValues.txt");
    SetFormFieldValue(open_path, output_dir + L"/SetFormFieldValue.pdf");
    SetFieldFlags(open_path, output_dir + L"/SetFieldFlags.pdf");

    // OCR Tesseract
    OcrWithTesseract(open_path, output_dir + L"/OcrTesseract.pdf", resources_dir + L"/tessdata", L"eng", 2., kRotate0);
    OcrPageImagesWithTesseract(open_path, output_dir + L"/OcrPageImagesWithTesseract.pdf", resources_dir + L"/tessdata", L"eng", 2., kRotate0);

    // Miscelaneous
    BookmarksToJson::Run(open_path, L"", std::cout);
    GetWhitespace::Run(open_path);
    OpedDocumentFromStream::Run(open_path);
    ParsePdsObjects::Run(open_path, std::cout);
    ParsePageContent::Run(open_path, std::cout, 0);
    DocumentMetadata::Run(open_path, output_dir + L"/DocumentMetadata.pdf", output_dir + L"/metadata.xml");
    EmbedFonts::Run(open_path, output_dir + L"/EmbedFonts.pdf");
    RegisterEvent(open_path);

    // Regex
    RegexSearch(open_path, L"(\\d{4}[- ]){3}\\d{4}");
    RegexSetPattern(open_path);
    */
  }
  catch (std::exception& ex) {
    std::cout << "Error: " << ex.what() << std::endl;
	  return 1;
  }
  return 0;
}

