////////////////////////////////////////////////////////////////////////////////////////////////////
// ParsePdsObjects.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ParsePdsObjects_cpp
*/
/*!
\page ParsePdsObjects_cpp Parse PDF document objects
Example how to read all documents objects in the document catalog.
\snippet /ParsePdsObjects.hpp ParsePdsObjects_cpp
*/

#pragma once

//! [ParsePdsObjects_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

#ifdef GetObject
#undef GetObject
#endif

// ProcessObject gets the value of the object.
void ProcessObject(PdsObject* obj, std::ostream& ss, std::string indent, 
  std::map<PdsObject*, int>& mapped) {
//  indent += " ";

//  ss << "{" << obj->GetObjectNumber() << "} ";

  auto found = mapped.find(obj);
  if (found != mapped.end()) {
    found->second++;
    ss << "[already mapped]";
    return;
  }
  
  ss << "==== Obj {" << obj->GetId() << "} " << std::endl;
  
  mapped.insert(std::make_pair(obj, 1));
  switch (obj->GetObjectType()) {
  case kPdsBoolean:{
    PdsBoolean* boolObj = (PdsBoolean*)obj;
    ss << (boolObj->GetValue() ? "true" : "false") << std::endl;
  } break;
  case kPdsNumber: {
    PdsNumber* numberObj = (PdsNumber*)obj;
    if (numberObj->IsIntegerValue())
      ss << numberObj->GetIntegerValue() << std::endl;
    else
      ss << (float)numberObj->GetValue() << std::endl;
  } break;
  case kPdsName: {
    PdsName* nameObj = (PdsName*)obj;
    std::wstring str;
    str.resize(nameObj->GetText(nullptr, 0));
    nameObj->GetText((wchar_t*)str.c_str(), (int)str.size());
    ss << ToUtf8(str) << std::endl;
  } break;
  case kPdsString: {
    PdsString* strObj = (PdsString*)obj;
    std::wstring str;
    str.resize(strObj->GetText(nullptr, 0));
    strObj->GetText((wchar_t*)str.c_str(), (int)str.size());
    ss << ToUtf8(str) << std::endl;
  } break;
  case kPdsStream: {
    PdsStream* streamObj = (PdsStream*)obj;
    ss << "Stream (" << streamObj->GetRawDataSize() << ")" << std::endl;
    auto sz = streamObj->GetSize();
    std::vector<unsigned char> data;
    if (sz) {
      data.resize(sz);
      if (!streamObj->Read(0, &data[0], sz))
        throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));
    }
    ss << "stream" << std::endl << std::string(begin(data), end(data)) << std::endl << "endstream" << std::endl;
    ProcessObject(streamObj->GetStreamDict(), ss, indent, mapped);
  } break;
  case kPdsArray: {
    PdsArray* arrObj = (PdsArray*)obj;
    for (int i = 0; i < (arrObj->GetNumObjects()); i++) {
      ss << std::endl << indent << "[" << i << "] ";
      ProcessObject(arrObj->Get(i), ss, indent, mapped);
    }
  } break;
  case kPdsDictionary: {
    PdsDictionary* dictObj = (PdsDictionary*)obj;
    for (int i = 0; i < (dictObj->GetNumKeys()); i++) {
      std::wstring key;
      key.resize(dictObj->GetKey(i, nullptr, 0));
      dictObj->GetKey(i, (wchar_t*)key.c_str(), (int)key.size());

      ss << std::endl << indent << "[" << ToUtf8(key) << "] ";
      
      std::string str;
      str.resize(dictObj->GetString(key.c_str(), nullptr, 0));
      dictObj->GetString(key.c_str(), (char*)str.c_str(), (int)str.size());

      std::wstring txt;
      txt.resize(dictObj->GetText(key.c_str(), nullptr, 0));
      dictObj->GetText(key.c_str(), (wchar_t*)txt.c_str(), (int)txt.size());

      int i_value = dictObj->GetInteger(key.c_str(), -1);
      double d_value = dictObj->GetNumber(key.c_str());
      
      ProcessObject(dictObj->Get(key.c_str()), ss, indent, mapped);
    }
  } break;
  default:
    int x = 0;
  }
}

// Iterates all documents bookmars.
void ParsePdsObjects(
  const std::wstring& email,                           // authorization email   
  const std::wstring& license_key,                     // authorization license key
  const std::wstring& open_path,                       // source PDF document
  const std::wstring& save_path                        // output document
  ) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdfDoc* doc = nullptr;
  doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  PdsObject* root = doc->GetRootObject();
  if (!root)
    throw std::runtime_error(std::to_string(GetPdfix()->GetErrorType()));

  std::map<PdsObject*, int> mapped;

  std::ofstream ofs;
  ofs.open(ToUtf8(save_path));
  ProcessObject(root, ofs, "", mapped);
  ofs.close();

  doc->Close();
  pdfix->Destroy();
}
//! [ParsePdsObjects_cpp]
