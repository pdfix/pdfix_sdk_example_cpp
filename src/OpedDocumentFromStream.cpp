////////////////////////////////////////////////////////////////////////////////////////////////////
// OpedDocumentFromStream.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/OpedDocumentFromStream.h"

#include <string>
#include <iostream>
#include <algorithm>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace OpedDocumentFromStream {

  // Iterates all documents bookmars.
  void Run(
    const std::wstring& open_path                        // source PDF document
    ) {
    // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");
    
    // open document from file stream
    PsStream* file_stm = pdfix->CreateFileStream(open_path.c_str(), kPsReadOnly);
    if (!file_stm)
      throw PdfixException();
    PdfDoc* doc0 = pdfix->OpenDocFromStream(file_stm, L"");
    if (!doc0)
      throw PdfixException();
    std::cout << "Document from file stream: " << doc0->GetNumPages() << " pages" << std::endl;
    doc0->Close();    
  
    // open PDF from memory stream
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
    
    PdfDoc* doc1 = pdfix->OpenDocFromStream(mem_stm, L"");
    if (!doc1)
      throw PdfixException();
    std::cout << "Document from memory stream: " << doc1->GetNumPages() << " pages" << std::endl;
    doc1->Close();    
    mem_stm->Destroy();

    // open from custom stream - define Read and GetSize procedure
    auto custom_stm = pdfix->CreateCustomStream(
      [](int offset, void* buffer, int size, void* data) -> int {
        return ((PsStream*)data)->Read(offset, (uint8_t*)buffer, size) ? size : 0;
      }, file_stm);
    if (!custom_stm)
      throw PdfixException();

    custom_stm->SetGetSizeProc(
      [](void* data) -> int { return ((PsStream*)data)->GetSize(); });

    PdfDoc* doc2 = pdfix->OpenDocFromStream(custom_stm, L"");
    if (!doc2)
      throw PdfixException();
    std::cout << "Document from custom stream: " << doc2->GetNumPages() << " pages" << std::endl;
    doc2->Close();    

    custom_stm->Destroy();

    file_stm->Destroy();
    pdfix->Destroy();
  }
}