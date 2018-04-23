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
#endif
#include "Pdfix.h"
#include "PdfToHtml.h"
#include "OcrTesseract.h"

Pdfix_statics;

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
  std::wstring result;
#ifndef _WIN32
  result.resize(PATH_MAX);
  realpath((char*)path.c_str(), (char*)result.c_str());
#else
  std::wstring dir;
  dir.resize(_MAX_PATH);
  GetModuleFileNameW(NULL, (wchar_t*)dir.c_str(), _MAX_PATH);
  dir.erase(dir.begin() + dir.find_last_of(L"\\/") + 1, dir.end());
  SetCurrentDirectoryW(dir.c_str());
  result.resize(_MAX_PATH);
  GetFullPathNameW(path.c_str(), _MAX_PATH, (wchar_t*)result.c_str(), NULL);
#endif
  return result;
}

std::string ToUtf8(const wchar_t unicode) {
  std::string out;
  if ((unsigned int)unicode < 0x80) {
    out.push_back((char)unicode);
  }
  else {
    if ((unsigned int)unicode >= 0x80000000) {
      return out;
    }
    int nbytes = 0;
    if ((unsigned int)unicode < 0x800) {
      nbytes = 2;
    }
    else if ((unsigned int)unicode < 0x10000) {
      nbytes = 3;
    }
    else if ((unsigned int)unicode < 0x200000) {
      nbytes = 4;
    }
    else if ((unsigned int)unicode < 0x4000000) {
      nbytes = 5;
    }
    else {
      nbytes = 6;
    }
    static uint8_t prefix[] = { 0xc0, 0xe0, 0xf0, 0xf8, 0xfc };
    int order = 1 << ((nbytes - 1) * 6);
    int code = unicode;
    out.push_back(prefix[nbytes - 2] | (code / order));
    for (int i = 0; i < nbytes - 1; i++) {
      code = code % order;
      order >>= 6;
      out.push_back(0x80 | (code / order));
    }
  }
  return out;
}

#if defined _WIN32
template <typename T>
std::string wide2utf8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source) {
  std::string result;
  std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
  result = convertor.to_bytes(source);
  return result;
}
#endif

std::string ToUtf8(std::wstring& wstr) {
#ifndef _WIN32
  std::string result;
  for (auto c : wstr)
    result.append(ToUtf8(c));
  return result;
#else
  return wide2utf8(wstr);
#endif
}