
#include "pdfixsdksamples/SearchText.h"

#include <cassert>
#include <algorithm>
#include <array>
#include <functional>

#include <locale>
#include <codecvt>
#include <iomanip>
#include <iostream>

#include "Pdfix.h"

using namespace PDFixSDK;

namespace SearchText {

  void DrawQuad(PdfDoc* doc, PdsContent* content, const PdfQuad& quad) {
    auto rgb_colorspace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceRGB);

    PdfMatrix matrix;
    auto path_obj = content->AddNewPath(-1, &matrix);
    if (!path_obj)
      throw PdfixException();

    path_obj->MoveTo(&quad.tl);
    path_obj->LineTo(&quad.tr);
    path_obj->LineTo(&quad.br);
    path_obj->LineTo(&quad.bl);
    path_obj->ClosePath();

    path_obj->SetStroke(true);
    path_obj->SetFillType(kFillRuleEvenOdd);

    PdfGraphicState gs;
    
    // red color
    auto color = rgb_colorspace->CreateColor();
    color->SetValue(0, 0.99f);
    color->SetValue(1, 0.33f);
    color->SetValue(2, 0.33f);

    // stroke
    gs.color_state.stroke_color = color;
    gs.color_state.stroke_opacity = 255;
    gs.color_state.stroke_type = kFillTypeSolid;

    // fill
    gs.color_state.fill_color = color;
    gs.color_state.fill_opacity = 50;
    gs.color_state.fill_type = kFillTypeSolid;
    
    gs.line_width = 1.0;
    gs.miter_limit = 1.0;
    gs.line_cap = kPdfLineCapRound;
    gs.line_join = kPdfLineJoinRound;
    gs.blend_mode = kBlendModeNormal;

    gs.matrix = matrix;

    path_obj->SetGState(&gs);
  }

  bool lower_test(wchar_t l, wchar_t r) {
    return std::tolower(l) == std::tolower(r);
  }

  static void SearchText(
    PdfPage* page,
    const std::wstring& query,
    std::function<void(PdsWord*)> process_word
  ) {

    auto word_list = page->AcquireWordList(kWordFinderAlgLatest);
    int word_count = word_list->GetNumWords();
    for (int i = 0; i < word_count; i++) {
      auto word = word_list->GetWord(i);

  #if defined(_DEBUG) && 0
      std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
      std::string u8word = conv.to_bytes(word->GetText());
      std::cout << u8word << " = ";
      auto num_textrun = word->GetNumTextRuns();
      for (int j = 0; j < num_textrun; j++) {
        auto textrun = word->GetTextRunAt(j);
        auto text_obj = textrun->GetTextObject();
        
        std::string u8str = conv.to_bytes(text_obj->GetText());
        auto from =  textrun->GetFirstCharIndex();
        auto to = textrun->GetLastCharIndex();
        if (j > 0) {
        std::cout << " + ";
        }
        std::cout << u8str.substr(from, to-from);
      }
      std::cout << std::endl;
  #endif

      auto text = word->GetText();
      auto it = std::search(text.begin(), text.end(), query.begin(), query.end(), lower_test);

  #if defined(_DEBUG) && 0
      std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
      std::string u8str = conv.to_bytes(text);
      std::cout << u8str << std::endl;
  #endif
      // found
      if (it != text.end()) {
        process_word(word);
      }
    }
    word_list->Release();
  }

  void Run(
    const std::wstring& open_path,  // source PDF document
    const std::wstring& save_path,  // destination PDF document
    const std::wstring& query,      // text to search in pdf file
    int page_num                    // number of the page where to search, -1 for all pages
  ) {
    
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();

    PdfPage* page = nullptr;

    auto process_word = [&](PdsWord* word) {
      auto content = page->GetContent();
      auto quad = word->GetQuad();      
      DrawQuad(doc, content, quad);
    };

    if (page_num < 0) {
      auto page_count = doc->GetNumPages();
      for (int i = 0; i < page_count; i++) {
        page = doc->AcquirePage(i);
        SearchText(page, query, process_word);

        page->SetContent();
        page->Release();
      }
    } else {
      page = doc->AcquirePage(page_num);
      SearchText(page, query, process_word);

      page->SetContent();
      page->Release();
    }

    doc->Save(save_path.c_str(), kSaveFull);

    doc->Close();
    pdfix->Destroy();
  }
} // namespace SearchText
