
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

inline  std::array<PdfPoint, 4> GetRectPoints(const PdfRect& rect) {
  return {
    PdfPoint{rect.left, rect.top},
    PdfPoint{rect.right, rect.top},
    PdfPoint{rect.right, rect.bottom},
    PdfPoint{rect.left, rect.bottom}
  };
}

void DrawRecangle(PdfDoc* doc, PdsContent* content, const PdfRect& rect) {
  auto rgb_colorspace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceRGB);

  PdfMatrix matrix;
  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = 0;
  matrix.f = 0;

  auto path_obj = content->AddNewPath(-1, &matrix);
  if (!path_obj)
    throw PdfixException();

  auto pts = GetRectPoints(rect);
  path_obj->MoveTo(&pts[0]);
  path_obj->LineTo(&pts[1]);
  path_obj->LineTo(&pts[2]);
  path_obj->LineTo(&pts[3]);
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
  PdsWordFinder* finder,
  const std::wstring& query,
  int page_num,
  std::function<void(PdsWord*)> process_word
) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

  auto word_list = finder->AcquireWordList(page_num);
  int word_count = word_list->GetWordCount();
  for (int i = 0; i < word_count; i++) {
    auto word = word_list->GetWord(i);
    auto text = word->GetText();
    auto it = std::search(text.begin(), text.end(), query.begin(), query.end(), lower_test);
//#ifdef DEBUG && 1
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::string u8str = conv.to_bytes(text);
    std::cout << u8str << std::endl;
//#endif
    // found
    if (it != text.end()) {
      process_word(word);
    }
  }
  finder->ReleaseWordList(page_num);
}

void SearchText(
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
    //std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    //std::string u8str = conv.to_bytes(word->GetText());
    //std::cout << u8str << std::endl;

    auto content = page->GetContent();
    auto bbox = word->GetBBox();
    
    DrawRecangle(doc, content, bbox);

    page->SetContent();
  };

  auto finder = doc->CreateWordFinder(0);
  if (page_num < 0) {
    auto page_count = doc->GetNumPages();
    for (int i = 0; i < page_count; i++) {
      page = doc->AcquirePage(i);
      SearchText(finder, query, i, process_word);
    }
  } else {
    page = doc->AcquirePage(page_num);
    SearchText(finder, query, page_num, process_word);
  }

  doc->Save(save_path.c_str(), kSaveFull);

  finder->Destroy();
  doc->Close();
  pdfix->Destroy();
}

