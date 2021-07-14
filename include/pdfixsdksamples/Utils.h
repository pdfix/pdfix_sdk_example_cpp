#pragma once

#include <string>
#include "Pdfix.h"

using namespace PDFixSDK;

#define kPi 3.1415926535897932384626433832795f

bool DirectoryExists(const std::wstring& path, bool create);
std::wstring FromUtf8(const std::string& str);
std::string ToUtf8(const std::wstring& str);
std::string PsStreamEncodeBase64(PsStream *stream);
void PdfMatrixTransform(PdfMatrix &m, PdfPoint &p);
void PdfMatrixConcat(PdfMatrix& m, PdfMatrix& m1, bool prepend);
void PdfMatrixRotate(PdfMatrix& m, float radian, bool prepend);
void PdfMatrixScale(PdfMatrix& m, float sx, float sy, bool prepend);
void PdfMatrixTranslate(PdfMatrix& m, float x, float y, bool prepend);
void PdfMatrixInverse(PdfMatrix& m, PdfMatrix& m1);