////////////////////////////////////////////////////////////////////////////////////////////////////
// CopyAnnots.cpp
// Copyright (c) 2021 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CopyAnnots.h"

// project
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

#include <cassert>

using namespace PDFixSDK;

#define UNUSED(x) (void)(x)

namespace CopyAnnots {

    void CopyAnnots(
      Pdfix* pdfix,
      const std::wstring& src_path,     // source PDF document
      const std::wstring& dest_path,    // destination PDF doucment
      const std::wstring& save_path,     // output PDF doucment
      int src_page_index,
      int dest_page_index
  ) {
    auto source_doc = pdfix->OpenDoc(src_path.c_str(), L"");
    if (!source_doc)
      throw PdfixException();

    auto dest_doc = pdfix->OpenDoc(dest_path.c_str(), L"");
    if (!dest_doc)
      throw PdfixException();

    auto src_page = source_doc->AcquirePage(src_page_index);
    if (!src_page)
      throw PdfixException();

    auto dest_page = dest_doc->AcquirePage(dest_page_index);
    if (!dest_page)
      throw PdfixException();

    auto num_annots = src_page->GetNumAnnots();
    for (int i = 0; i < num_annots; i++) {
      auto annot = src_page->GetAnnot(i);
      if (annot->CanCopy()) {
        auto copy_data = annot->Copy();
        if (annot->CanPaste(dest_page, nullptr, copy_data)) {
          auto new_annot = annot->Paste(dest_page, nullptr, copy_data);
          if (!new_annot) {
            throw PdfixException();
          }
        }
      }
    }

    dest_doc->Save(save_path.c_str(), kSaveFull);
    dest_doc->Close();
    source_doc->Close();
  }

  void CopyAnnots(
      const std::wstring& src_path,     // source PDF document
      const std::wstring& dest_path,    // destination PDF doucment
      const std::wstring& save_path,     // output PDF doucment
      int src_page_index,
      int dest_page_index
  ) {
    auto pdfix = PdfixEngine::Get();

    CopyAnnots(pdfix, src_path, dest_path, save_path, src_page_index, dest_page_index);
  }

  // we define custom annot handler for circle annotations that copies
  // circle annotation and paste it as square
  void CopyCustomAnnots(
      const std::wstring& src_path,     // source PDF document
      const std::wstring& dest_path,    // destination PDF doucment
      const std::wstring& save_path,     // output PDF doucment
      int src_page_index,
      int dest_page_index
  ) {
    auto pdfix = PdfixEngine::Get();

    std::wstring custom_annot_name = L"Circle";
    auto annot_handler = pdfix->RegisterAnnotHandler(custom_annot_name.c_str());

    annot_handler->SetCanCopyProc([](PdfAnnot* annot) -> bool {
      UNUSED(annot);
      return true;
    });

    annot_handler->SetCopyProc([](PdfAnnot* annot) -> void* {
      auto rect = annot->GetBBox();
      auto result = new PdfRect();
      result->bottom = rect.bottom;
      result->left = rect.left;
      result->right = rect.right;
      result->top = rect.top;
      return result;
    });

    annot_handler->SetCanPasteProc([](PdfPage* dest_page, const PdfPoint* center, void* data) -> bool {
      UNUSED(dest_page);
      UNUSED(center);
      UNUSED(data);
      return true;
    });

    annot_handler->SetPasteProc([](PdfPage* dest_page, const PdfPoint* center, void* data) -> PdfAnnot* {
      UNUSED(center);
      PdfRect* rect = static_cast<PdfRect*>(data);
      auto annot = dest_page->CreateAnnot(PdfAnnotSubtype::kAnnotSquare, rect);
      dest_page->AddAnnot(-1, annot);
      return annot;
    });

    annot_handler->SetDestroyDataProc([](void* data){
      if (data) {
        PdfRect* rect = static_cast<PdfRect*>(data);
        delete rect;
      }
      return true;
    });

    CopyAnnots(pdfix, src_path, dest_path, save_path, src_page_index, dest_page_index);
  }
}
