////////////////////////////////////////////////////////////////////////////////////////////////////
// OpedDocumentFromStream.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage OpedDocumentFromStream_cpp
*/
/*!
\page OpedDocumentFromStream_cpp Open PDF from stream Sample
Example how to open a PDF from memory.
\snippet /OpedDocumentFromStream.hpp OpedDocumentFromStream_cpp
*/

#include "pdfixsdksamples/OpedDocumentFromStream.h"

//! [OpedDocumentFromStream_cpp]
#include <string>
#include <iostream>
#include <algorithm>
#include "Pdfix.h"

using namespace PDFixSDK;

// Iterates all documents bookmars.
void OpedDocumentFromStream(
  const std::wstring& open_path                        // source PDF document
  ) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME))
    throw std::runtime_error("Pdfix initialization fail");

  Pdfix* pdfix = GetPdfix();
  if (!pdfix)
    throw std::runtime_error("GetPdfix fail");
  
  PsStream* file_stm = pdfix->CreateFileStream(open_path.c_str(), kPsReadOnly);
  if (!file_stm)
    throw PdfixException();
  
  PsStream* mem_stm = pdfix->CreateMemStream();
  if (!mem_stm)
    throw PdfixException();
  
  int sz = file_stm->GetSize();
  int pos = 0;
  while (sz) {
    int read = std::min(sz, 4096);
    std::vector<unsigned char> buffer;
    buffer.resize(4096);
    file_stm->Read(pos, &buffer[0], read);
    mem_stm->Write(pos, &buffer[0], read);
    pos += read;
    sz -= read;
  }
  file_stm->Destroy();
  
  PdfDoc* doc = pdfix->OpenDocFromStream(mem_stm, L"");
  if (!doc)
    throw PdfixException();

  doc->Close();    
  mem_stm->Destroy();
  pdfix->Destroy();
}
//! [OpedDocumentFromStream_cpp]
