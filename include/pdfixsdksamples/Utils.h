#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

#define kPi 3.1415926535897932384626433832795f

std::wstring FromUtf8(const std::string& str);
std::string ToUtf8(const std::wstring& str);
void PdfMatrixTransform(PdfMatrix& m, PdfPoint& p);
void PdfMatrixConcat(PdfMatrix& m, PdfMatrix& m1, bool prepend);
void PdfMatrixRotate(PdfMatrix& m, double radian, bool prepend);
void PdfMatrixScale(PdfMatrix& m, double sx, double sy, bool prepend);
void PdfMatrixTranslate(PdfMatrix& m, double x, double y, bool prepend);
