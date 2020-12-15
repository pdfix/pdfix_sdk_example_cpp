
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

inline std::array<float, 4> CMYKToValues(PdfCMYK& cmyk_color) {
  const float divisor = 1.0f / 255.0f;
  std::array<float, 4> result;
  result[0] = (float)(cmyk_color.c) * divisor;
  result[1] = (float)(cmyk_color.m) * divisor;
  result[2] = (float)(cmyk_color.y) * divisor;
  result[3] = (float)(cmyk_color.k) * divisor;
  return result;
}

const PdfCMYK kDefaultCMYKColor = MakeCMYK(0, 100, 0, 0);
PdfColorSpace* gCMYKColorSpace = nullptr;

static void ConvertRGBToCMYK(PdfColor* color, const ColorMap& color_map) {
  assert(gCMYKColorSpace != nullptr);

  auto color_space = color->GetColorSpace();
  if (color_space->GetFamily() == kColorSpaceDeviceRGB) {

    // find color mapping
    auto rgb = color->GetRGB();
    auto color_pair = color_map.find(rgb);
    PdfCMYK cmyk_color = kDefaultCMYKColor;
    if (color_pair != color_map.end()) {
      cmyk_color = color_pair->second;
    }

    // set cmyk color space
    color->SetColorSpace(gCMYKColorSpace);
    
    // set cmyk color values
    auto values = CMYKToValues(cmyk_color);
    for (int i = 0; i < values.size(); i++) {
      color->SetValue(i, values[i]);
    }
  }
}

static void ConvertRGBToCMYK(PdfColorState& color_state, const ColorMap& color_map) {
  ConvertRGBToCMYK(color_state.fill_color, color_map);
  ConvertRGBToCMYK(color_state.stroke_color, color_map);
}

static void ConvertRGBToCMYK(PdfPage* page, const ColorMap& color_map) {
  auto content = page->GetContent();
  auto num_objects = content->GetNumObjects();
  for (int i = 0; i < num_objects; i++) {
    auto page_object = content->GetObject(i);
    auto g_state = page_object->GetGState();
    auto& color_state = g_state.color_state;
    ConvertRGBToCMYK(color_state, color_map);
    page_object->SetGState(&g_state);

    if (page_object->GetObjectType() == PdfPageObjectType::kPdsPageText) {
      auto text_object = (PdsText*)page_object;
      auto text_state = text_object->GetTextState(page->GetDoc());
      auto& t_color_state = text_state.color_state;
      ConvertRGBToCMYK(t_color_state, color_map);
      // TODO: text_object->SetTextState(&g_state);
    }
  }
}

static void ConvertRGBToCMYK(PdfDoc* doc, const ColorMap& color_map) {
  auto num_pages = doc->GetNumPages();
  for (int i = 0; i < num_pages; i++) {
    auto page = doc->AcquirePage(i);
    ConvertRGBToCMYK(page, color_map);
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
  color_map[MakeRGB(21, 125, 71)] = MakeCMYK(93, 0, 100, 3);
  color_map[MakeRGB(35, 170, 70)] = MakeCMYK(77, 0, 88, 0);
  color_map[MakeRGB(15, 123, 63)] = MakeCMYK(90, 70, 96, 28);
  color_map[MakeRGB(22, 159, 80)] = MakeCMYK(88, 0, 94, 0);

  ConvertRGBToCMYK(doc, color_map);

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}

