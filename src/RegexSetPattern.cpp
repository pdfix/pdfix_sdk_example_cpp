////////////////////////////////////////////////////////////////////////////////////////////////////
// RegexSetPattern.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/RegexSetPattern.h"

#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

  // Finds a first occurence of the pattern in an input text.
void RegexSetPattern(
  const std::wstring& text                       // text where to search the pattern
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");

  PsRegex* regex = pdfix->CreateRegex();
  if (!regex)
    throw std::runtime_error("Regex not created!");
  std::wstring pattern[10];
  // All major credit cards regex
  pattern[0] = L"^(?:4[0-9]{12}(?:[0-9]{3})?|5[1-5][0-9]{14}|6011[0-9]{12}|622((12[6-9]|"
    "1[3-9][0-9])|([2-8][0-9][0-9])|(9(([0-1][0-9])|(2[0-5]))))[0-9]{10}|64[4-9][0-9]{13}|"
    "65[0-9]{14}|3(?:0[0-5]|[68][0-9])[0-9]{11}|3[47][0-9]{13})*$";
  // American Express Credit Card
  pattern[1] = L"^(3[47][0-9]{13})*$";
  // MasterCard Credit Card
  pattern[2] = L"^(5[1-5][0-9]{14})*$";
  // Visa Credit Card
  pattern[3] = L"^(4[0-9]{12}(?:[0-9]{3})?)*$";
  // Phone Numbers(North American)
  pattern[4] = L"^((([0-9]{1})*[- .(]*([0-9]{3})[- .)]*[0-9]{3}[- .]*[0-9]{4})+)*$";
  // Social Security Numbers
  pattern[5] = L"^([0-9]{3}[-]*[0-9]{2}[-]*[0-9]{4})*$";
  // UK Postal Codes
  pattern[6] = L"^([A-Z]{1,2}[0-9][A-Z0-9]? [0-9][ABD-HJLNP-UW-Z]{2})*$";
  // URLs
  pattern[7] = L"^((http|https|ftp)://)?([[a-zA-Z0-9]\\-\\.])+(\\.)([[a-zA-Z0-9]]){2,4}"
    "([[a-zA-Z0-9]/+=%&_\\.~?\\-]*)$";
  // Emails
  pattern[8] = L"^[a-zA-Z0-9._%-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}$";
  // Passwords
  pattern[9] = L"(?=^.{6,}$)((?=.*[A-Za-z0-9])(?=.*[A-Z])(?=.*[a-z]))^.*";

  for (int i = 0; i < 10; i++) {
    regex->SetPattern(pattern[i].c_str());
    if (regex->Search(text.c_str(), 0)) {
      int pos = regex->GetPosition();
      int len = regex->GetLength();
      auto match_text = regex->GetText();
      std::wcout << match_text << std::endl;
    }
  }
  regex->Destroy();
  pdfix->Destroy();
}