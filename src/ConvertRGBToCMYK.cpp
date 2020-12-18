
#include "pdfixsdksamples/ConvertRGBToCMYK.h"

#include "Pdfix.h"

#include <boost/property_tree/json_parser.hpp>
#include <map>

using namespace PDFixSDK;

struct RGBComparator {
    bool operator()(const PdfRGB& a, const PdfRGB& b) const {
        return (a.r < b.r) || (a.g < b.g) || (a.b < b.b);
    }
};

typedef std::map<PdfRGB, PdfCMYK, RGBComparator> ColorMap;

inline PdfCMYK MakeCMYK(int c, int m, int y, int k) {
  PdfCMYK value;
  value.c = c;
  value.m = m;
  value.y = y;
  value.k = k;
  return value;
}

inline PdfRGB MakeRGB(int r, int g, int b) {
  PdfRGB value;
  value.r = r;
  value.g = g;
  value.b = b;
  return value;
}

inline std::array<double, 4> CMYKToValues(PdfCMYK& cmyk_color) {
  const double divisor = 1.0 / 100.0;
  std::array<double, 4> result;
  result[0] = (double)(cmyk_color.c) * divisor;
  result[1] = (double)(cmyk_color.m) * divisor;
  result[2] = (double)(cmyk_color.y) * divisor;
  result[3] = (double)(cmyk_color.k) * divisor;
  return result;
}

PdfColorSpace* gCMYKColorSpace = nullptr;

static PdfColor* ConvertRGBToCMYK(PdfColor* color, const ColorMap& color_map) {
  assert(gCMYKColorSpace != nullptr);

  PdfColor* result = color;

  auto color_space = color->GetColorSpace();
  if (color_space && (color_space->GetFamilyType() == kColorSpaceDeviceRGB)) {

    // find color mapping
    PdfRGB rgb = color->GetRGB();
    auto color_pair = color_map.find(rgb);
    if (color_pair != color_map.end()) {
      auto cmyk_color = color_pair->second;
      // create color in cmyk color space
      result = gCMYKColorSpace->CreateColor();
      // set cmyk color values
      auto values = CMYKToValues(cmyk_color);
      for (int i = 0; i < values.size(); i++) {
        result->SetValue(i, values[i]);
      }
    }
  }

  return result;
}

static void ConvertRGBToCMYK(PdfColorState& color_state, const ColorMap& color_map) {
  if (color_state.stroke_type != kFillTypeNone) {
    color_state.stroke_color = ConvertRGBToCMYK(color_state.stroke_color, color_map);
  }
  if (color_state.fill_type != kFillTypeNone) {
    color_state.fill_color = ConvertRGBToCMYK(color_state.fill_color, color_map);
  }
}

static void ConvertRGBToCMYK(PdfPage* page, const ColorMap& color_map) {
  auto content = page->GetContent();
  auto num_objects = content->GetNumObjects();
  for (int i = 0; i < num_objects; i++) {
    auto page_object = content->GetObject(i);
    if (page_object->GetObjectType() != PdfPageObjectType::kPdsPageText) {
      auto g_state = page_object->GetGState();
      auto& color_state = g_state.color_state;
      ConvertRGBToCMYK(color_state, color_map);
      page_object->SetGState(&g_state);
      if (color_state.stroke_color)
        color_state.stroke_color->Destroy();
      if (color_state.fill_color)
        color_state.fill_color->Destroy();
    } else {
      auto text_object = (PdsText*)page_object;
      auto t_state = text_object->GetTextState(page->GetDoc());
      auto& color_state = t_state.color_state;
      ConvertRGBToCMYK(color_state, color_map);
      text_object->SetTextState(&t_state);
      if (color_state.stroke_color)
        color_state.stroke_color->Destroy();
      if (color_state.fill_color)
        color_state.fill_color->Destroy();
    }
  }
}

static void ConvertRGBToCMYK(PdfDoc* doc, const ColorMap& color_map) {
  auto num_pages = doc->GetNumPages();
  for (int i = 0; i < num_pages; i++) {
    auto page = doc->AcquirePage(i);
    ConvertRGBToCMYK(page, color_map);
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

  //ptree template_tree;
  //read_json(ss, template_tree);
  // TODO: load colormap
  // Black: RGB 0, 0, 0 - Replace with CMYK 0, 0, 0, 100
  // Dark green: RGB: 0, 85, 34 - Replace with CMYK 92, 14, 98, 55
  // Mid Green: RGB 21, 152, 71 - Replace with CMYK 93, 0, 100, 3
  // Highlight Green: RGB 35, 170, 77 - Replace with CMYK  77, 0, 88, 0
  // Dark green: RGB: 15, 123, 63 - Replace with CMYK 90, 7, 96, 28
  // Highlight Green: RGB 22, 159, 80 - Replace with CMYK  88, 0, 94, 0
  ColorMap color_map;
  color_map[MakeRGB(0, 0, 0)] = MakeCMYK(0, 0, 0, 100);
  color_map[MakeRGB(0, 85, 34)] = MakeCMYK(92, 14, 98, 55);
  color_map[MakeRGB(21, 152, 71)] = MakeCMYK(93, 0, 100, 3);
  color_map[MakeRGB(35, 170, 77)] = MakeCMYK(77, 0, 88, 0);
  color_map[MakeRGB(15, 123, 63)] = MakeCMYK(90, 70, 96, 28);
  color_map[MakeRGB(22, 159, 80)] = MakeCMYK(88, 0, 94, 0);

  ConvertRGBToCMYK(doc, color_map);

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}

