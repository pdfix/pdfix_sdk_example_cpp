
#include "pdfixsdksamples/ConvertRGBToCMYK.h"

#include "Pdfix.h"

#include <boost/property_tree/json_parser.hpp>
#include <map>

using namespace PDFixSDK;

typedef std::map<uint32_t, PdfCMYK> ColorMap;

inline PdfCMYK MakeCMYK(int c, int m, int y, int k) {
  PdfCMYK value;
  value.c = c;
  value.m = m;
  value.y = y;
  value.k = k;
  return value;
}

static void ConvertRGBToCMYK(PdfDoc* doc, const ColorMap& color_map) {
  // TODO:
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
  color_map[0x00000000] = MakeCMYK(0, 0, 0, 100);
  color_map[0x00005522] = MakeCMYK(92, 14, 98, 55);
  color_map[0x00159847] = MakeCMYK(93, 0, 100, 3);
  color_map[0x0023aa4d] = MakeCMYK(77, 0, 88, 0);
  color_map[0x000f7b3f] = MakeCMYK(90, 70, 96, 28);
  color_map[0x00169f50] = MakeCMYK(88, 0, 94, 0);

  ConvertRGBToCMYK(doc, color_map);

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
  pdfix->Destroy();
}

