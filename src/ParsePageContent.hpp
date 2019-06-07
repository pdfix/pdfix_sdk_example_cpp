////////////////////////////////////////////////////////////////////////////////////////////////////
// ParsePageContent.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage ParsePageContent_cpp
*/
/*!
\page ParsePageContent_cpp Parse PDF page content
Example how to read all page content objects.
\snippet /ParsePageContent.hpp ParsePageContent_cpp
*/

#pragma once

//! [ParsePageContent_cpp]
#include <string>
#include <iostream>
#include "Pdfix.h"

// ProcessObject gets the value of the object.
void ProcessPageObject(PdsPageObject* obj, std::ostream& ss, std::string indent) {
  indent += " ";
  ss << indent;
  switch (obj->GetObjectType()) {
    case kPdsPageUnknown:
      ss << "Unknown: " << std::endl;
      break;
    case kPdsPageText: {
      std::wstring text;
      text.resize(((PdsText*)obj)->GetText(nullptr, 0));
      ((PdsText*)obj)->GetText((wchar_t*)text.c_str(), (int)text.size());

      ss << "Text: " << ToUtf8(text) << std::endl;
      break;
    }
    case kPdsPageImage:
      ss << "Image: " << std::endl;
      break;
    case kPdsPagePath:
      ss << "Path: " << std::endl;
      break;
    case kPdsPageShading:
      ss << "Shading: " << std::endl;
      break;
    case kPdsPageForm: {
      ss << "Form: " << std::endl;
      auto form = (PdsForm*)obj;
      for (int i = 0; i < form->GetNumPageObjects(); i++) {
        auto kid = form->GetPageObject(i);
        ProcessPageObject(kid, ss, indent);
      }
      break;
    }
  }
}

// Iterates all documents bookmars.
void ParsePageContent(
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
    
  PdfPage* page = doc->AcquirePage(0);
    
  std::ofstream ofs(ToUtf8(save_path));
  if (!ofs.is_open())
    throw std::runtime_error("Failed to open file " + ToUtf8(save_path) + "; " + std::to_string(errno));
    
  // get page content flags
  // TODO:
  auto flags = page->GetContentFlags();
    
  ofs << "Page content: ";
  if (flags & kContentText) ofs << "Text, ";
  if (flags & kContentImage) ofs << "Image, ";
  if (flags & kContentPath) ofs << "Path, ";
  if (flags & kContentForm) ofs << "Form, ";
  if (flags & kContentShading) ofs << "Shading, ";
  if (flags & kContentTextTransparent) ofs << "Transparent text, ";
  if (flags & kContentTextFill) ofs << "Text Fill, ";
  if (flags & kContentTextStroke) ofs << "Text Stroke, ";
  ofs << std::endl << std::endl;

  for (int i = 0; i < page->GetNumPageObjects(); i++ ) {
    ProcessPageObject(page->GetPageObject(i), ofs, "");
  }
  ofs.close();
    
  page->Release();

  doc->Close();
  pdfix->Destroy();
}
//! [ParsePageContent_cpp]
