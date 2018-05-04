////////////////////////////////////////////////////////////////////////////////////////////////////
// Utils.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <locale.h>
#include <codecvt>
#ifdef _WIN32
#include <Windows.h>
extern HINSTANCE ghInstance;
#elif defined __linux__
#include <cstring>
#include <limits.h>
#include <locale>
#endif
#include "Pdfix.h"
#include "PdfToHtml.h"
#include "OcrTesseract.h"

Pdfix_statics;

// convert UTF-8 string to wstring
std::wstring FromUtf8(const std::string& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string ToUtf8(const std::wstring& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str);
}
std::string GetAbsolutePath(const std::string& path) {
  std::string result;
#ifndef _WIN32
  result.resize(PATH_MAX);
  realpath(path.c_str(), (char*)result.c_str());
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