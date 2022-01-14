
#include "pdfixsdksamples/ConvertRGBToCMYK.h"
#include  <cassert>
#include <algorithm>
#include "Pdfix.h"

using namespace PDFixSDK;

PdfColorSpace* gCMYKColorSpace = nullptr;

static PdfColor* ConvertRGBToCMYK(PdfColor* color) {
  assert(gCMYKColorSpace != nullptr);

  PdfColor* result = color;

  auto color_space = color->GetColorSpace();
  if (color_space && (color_space->GetFamilyType() == kColorSpaceDeviceRGB)) {
    // create color in cmyk color space
    result = gCMYKColorSpace->CreateColor();
    // set cmyk color values
    // see: https://www.rapidtables.com/convert/color/rgb-to-cmyk.html
    auto r = color->GetValue(0);
    auto g = color->GetValue(1);
    auto b = color->GetValue(2);

    auto k = 1.0 - std::max(r, std::max(g, b));
    auto k_inv_recip = (k == 1.0) ? 0.0 : (1.0 / (1.0 - k));
    auto c = (1.0 - r - k) * k_inv_recip;
    auto m = (1.0 - g - k) * k_inv_recip;
    auto y = (1.0 - b - k) * k_inv_recip;

    result->SetValue(0, c);
    result->SetValue(1, m);
    result->SetValue(2, y);
    result->SetValue(3, k);
  }

  return result;
}

static void ConvertRGBToCMYK(PdfColorState& color_state) {
  if (color_state.stroke_type != kFillTypeNone) {
    color_state.stroke_color = ConvertRGBToCMYK(color_state.stroke_color);
  }
  if (color_state.fill_type != kFillTypeNone) {
    color_state.fill_color = ConvertRGBToCMYK(color_state.fill_color);
  }
}

static void ConvertRGBToCMYK(PdfPage* page) {
  auto content = page->GetContent();
  auto num_objects = content->GetNumObjects();
  for (int i = 0; i < num_objects; i++) {
    auto page_object = content->GetObject(i);
    if (page_object->GetObjectType() != PdfPageObjectType::kPdsPageText) {
      auto g_state = page_object->GetGState();
      auto& color_state = g_state.color_state;
      ConvertRGBToCMYK(color_state);
      page_object->SetGState(&g_state);
      if (color_state.stroke_color)
        color_state.stroke_color->Destroy();
      if (color_state.fill_color)
        color_state.fill_color->Destroy();
    } else {
      auto text_object = (PdsText*)page_object;
      auto t_state = text_object->GetTextState();
      auto& color_state = t_state.color_state;
      ConvertRGBToCMYK(color_state);
      text_object->SetTextState(&t_state);
      if (color_state.stroke_color)
        color_state.stroke_color->Destroy();
      if (color_state.fill_color)
        color_state.fill_color->Destroy();
    }
  }
}

static void ConvertRGBToCMYK(PdfDoc* doc) {
  auto num_pages = doc->GetNumPages();
  for (int i = 0; i < num_pages; i++) {
    auto page = doc->AcquirePage(i);
    ConvertRGBToCMYK(page);
    page->SetContent();
    page->Release();
  }
}

void ConvertRGBToCMYK(
  const std::wstring& open_path,      // source PDF document
  const std::wstring& save_path       // output PDF document
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

  gCMYKColorSpace = doc->CreateColorSpace(PdfColorSpaceFamily::kColorSpaceDeviceCMYK);

  ConvertRGBToCMYK(doc);

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}

