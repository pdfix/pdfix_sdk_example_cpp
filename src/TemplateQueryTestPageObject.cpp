////////////////////////////////////////////////////////////////////////////////////////////////////
// TemplateQueryTestPageObject.cpp
// Copyright (c) 2026 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/TemplateQueryTestPageObject.h"

#include <string>

#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

namespace TemplateQueryTestPageObject {

namespace {

struct EnumContext {
  PdfTemplateQuery* query = nullptr;
  int object_count = 0;
  int matched_count = 0;
};

PdfEnumResultType EnumPageObjectProc(PdsPageObject* object, int /*index*/, void* client_data) {
  auto* ctx = static_cast<EnumContext*>(client_data);
  if (!object || !ctx || !ctx->query) {
    return kEnumResultContinue;
  }

  ++ctx->object_count;
  if (ctx->query->TestPageObject(object)) {
    ++ctx->matched_count;
  }

  return kEnumResultContinue;
}

}  // namespace

void Run(const std::wstring& open_path, std::ostream& output) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc) {
    throw PdfixException();
  }

  PdfTemplateQuery* query = doc->CreateTemplateQuery();
  if (!query) {
    doc->Close();
    throw PdfixException();
  }

  if (!query->LoadFromRegex(L".*")) {
    doc->Close();
    throw PdfixException();
  }

  EnumContext ctx;
  ctx.query = query;

  for (int page_index = 0; page_index < doc->GetNumPages(); ++page_index) {
    PdfPage* page = doc->AcquirePage(page_index);
    if (!page) {
      throw PdfixException();
    }

    PdsContent* content = page->GetContent();
    if (!content) {
      page->Release();
      throw PdfixException();
    }

    doc->EnumPageObjects(content, nullptr, kEnumForms, EnumPageObjectProc, &ctx);
    page->Release();
  }

  output << "page_objects: " << ctx.object_count << std::endl;
  output << "matched: " << ctx.matched_count << std::endl;

  doc->Close();
}

}  // namespace TemplateQueryTestPageObject
