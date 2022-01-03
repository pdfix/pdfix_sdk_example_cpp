////////////////////////////////////////////////////////////////////////////////////////////////////
// Utils.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/Utils.h"
#include <string>
#include <iostream>
#include <locale.h>
#include <codecvt>
#include <math.h>
#ifdef _WIN32
#include <Windows.h>
#include <Shlobj.h>
extern HINSTANCE ghInstance;
#elif defined __GNUC__
#include <cstring>
#include <limits.h>
#include <locale>
#include <sys/stat.h>
#endif
#include "Pdfix.h"
#include "OcrTesseract.h"

using namespace PDFixSDK;

Pdfix_statics;
OcrTesseract_statics;

////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateDirectory
////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateDirectory(const std::string& dir) {
#if defined _MSC_VER 
  return SHCreateDirectoryExA(NULL, dir.c_str(), NULL);
//  return _mkdir(dir.data());
#elif defined __GNUC__
  return mkdir(dir.data(), 0777);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DirectoryExists
////////////////////////////////////////////////////////////////////////////////////////////////////
bool DirectoryExists(const std::string& path, bool create) {
  struct stat s;
  if (stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR))
    return true;
  if (create && !CreateDirectory(path.c_str())) return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DirectoryExists
////////////////////////////////////////////////////////////////////////////////////////////////////
bool DirectoryExists(const std::wstring& path, bool create) {
  return DirectoryExists(ToUtf8(path), create);
}

// convert UTF-8 string to wstring
std::wstring FromUtf8(const std::string& str) {
  std::wstring result;
  try {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    result = myconv.from_bytes(str);
  }
  catch (std::range_error& e) {
  }
  return result;
}

// convert wstring to UTF-8 string
std::string ToUtf8(const std::wstring& str) {
  std::string result;
  try {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    result = myconv.to_bytes(str);
  }
  catch (std::range_error& e) {
  }
  return result;
}

std::string GetAbsolutePath(const std::string& path) {
  std::string result;
#ifndef _WIN32
  if (path.length() && path.front() == '/') {
    result = path;
  }
  else {
    result.resize(PATH_MAX);
    realpath(path.c_str(), (char*)result.c_str());
  }  
#else
  std::string dir;
  dir.resize(_MAX_PATH);
  GetModuleFileNameA(NULL, (char*)dir.c_str(), _MAX_PATH);
  dir.erase(dir.begin() + dir.find_last_of("\\/") + 1, dir.end());
  SetCurrentDirectoryA(dir.c_str());
  result.resize(_MAX_PATH);
  GetFullPathNameA(path.c_str(), _MAX_PATH, (char*)result.c_str(), NULL);
#endif
  result.resize(strlen(result.c_str()));
  return result;
}

std::wstring GetAbsolutePath(const std::wstring& path) {
  return FromUtf8(GetAbsolutePath(ToUtf8(path)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Base64Encode
std::string Base64Encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }
  return ret;
}

std::string PsStreamEncodeBase64(PsStream* stream) {
  auto len = stream->GetSize();
  std::vector<unsigned char> buffer;
  buffer.resize(len);
  stream->Read(0, &buffer[0], len);
  return Base64Encode((unsigned char*)&buffer[0], len);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PdfMatrix utils
////////////////////////////////////////////////////////////////////////////////////////////////////
void PdfMatrixTransform(PdfMatrix& m, PdfPoint& p) {
  PdfPoint ret;
  ret.x = m.a * p.x + m.c * p.y + m.e;
  ret.y = m.b * p.x + m.d * p.y + m.f;
  p.x = ret.x;
  p.y = ret.y;
}

void PdfMatrixConcat(PdfMatrix& m, PdfMatrix& m1, bool prepend) {
  PdfMatrix left(m), right(m1);
  if (prepend)
    std::swap(left, right);
  m.a = left.a * right.a + left.b * right.c;
  m.b = left.a * right.b + left.b * right.d;
  m.c = left.c * right.a + left.d * right.c;
  m.d = left.c * right.b + left.d * right.d;
  m.e = left.e * right.a + left.f * right.c + right.e;
  m.f = left.e * right.b + left.f * right.d + right.f;
}

void PdfMatrixRotate(PdfMatrix& m, float radian, bool prepend) {
  float cosValue = cos(radian);
  float sinValue = sin(radian);
  PdfMatrix m1;
  m1.a = cosValue;
  m1.b = sinValue;
  m1.c = -sinValue;
  m1.d = cosValue;
  PdfMatrixConcat(m, m1, prepend);
}

void PdfMatrixScale(PdfMatrix& m, float sx, float sy, bool prepend) {
  m.a *= sx;
  m.d *= sy;
  if (prepend) {
    m.b *= sx;
    m.c *= sy;
  }
  m.b *= sy;
  m.c *= sx;
  m.e *= sx;
  m.f *= sy;
}

void PdfMatrixTranslate(PdfMatrix& m, float x, float y, bool prepend) {
  if (prepend) {
    m.e += x * m.a + y + m.c;
    m.f += y * m.d + x * m.b;
  }
  m.e += x;
  m.f += y; 
}

void PdfMatrixInverse(PdfMatrix& m, PdfMatrix& m1) {
  PdfMatrix inverse(m), orig(m1);
  float i = orig.a * orig.d - orig.b * orig.c;
  if (fabs(i) == 0)
    return;

  float j = -i;
  inverse.a = orig.d / i;
  inverse.b = orig.b / j;
  inverse.c = orig.c / j;
  inverse.d = orig.a / i;
  inverse.e = (orig.c * orig.f - orig.d * orig.e) / i;
  inverse.f = (orig.a * orig.f - orig.b * orig.e) / j;
}
