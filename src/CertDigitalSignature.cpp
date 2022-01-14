////////////////////////////////////////////////////////////////////////////////////////////////////
// CertDigitalSignature.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/CertDigitalSignature.h"

#include <string>
#include <iostream>
#ifdef _WIN32
#include <Cryptuiapi.h>
#include <WinCrypt.h>
#include <Cryptdlg.h>
#endif
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

#ifdef WIN32
static UINT_PTR CALLBACK CertHookCallback(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  return 1;
}

static BOOL CALLBACK  CertFilterCallback(PCCERT_CONTEXT pCertContext,
  DWORD lCustData, DWORD dwFlags, DWORD dwDisplayWell) {
  // would be good to check if certificate is suitable for signing
  // if contains private key for example
  return true;
}
#endif
  
// Applies digital signature.
void CertDigitalSignature(
  const std::wstring& email,                           // authorization email   
  const std::wstring& license_key,                     // authorization license key
  const std::wstring& open_path,                       // source PDF document
  const std::wstring& save_path,                       // signed PDF document
  const std::wstring& time_stamp_server                // time stamp server
) {
#ifdef WIN32 
  HCERTSTORE cert_store = NULL;
  PCCERT_CONTEXT cert_context = NULL;
  // open a certificate store.
  if (!(cert_store = CertOpenSystemStore(NULL, TEXT("MY"))))
    throw std::runtime_error("Open retificate store failed!");
  HMODULE hLib = LoadLibraryA("cryptdlg.dll");
  BOOL(WINAPI *pCertSelectCertificate)(PCERT_SELECT_STRUCT) = NULL;
  pCertSelectCertificate = (BOOL(WINAPI *)(PCERT_SELECT_STRUCT))GetProcAddress(hLib, "CertSelectCertificateW");
  CERT_SELECT_STRUCT certSelect;
  memset(&certSelect, 0, sizeof(CERT_SELECT_STRUCT));
  certSelect.dwFlags = CSS_ENABLEHOOK;
  certSelect.dwSize = sizeof(CERT_SELECT_STRUCT);
  certSelect.hwndParent = ::GetForegroundWindow();
  certSelect.pfnHook = (PFNCMHOOKPROC)CertHookCallback;
  certSelect.pfnFilter = (PFNCMFILTERPROC)CertFilterCallback;
  certSelect.cCertStore = 1;
  certSelect.arrayCertStore = &cert_store;
  certSelect.cCertContext = 1;
  certSelect.arrayCertContext = &cert_context;
  if (pCertSelectCertificate(&certSelect) == false) {
    if (cert_store)
      CertCloseStore(cert_store, 0);
    throw std::runtime_error("Select cetificate store failed!");
  }

  auto pdfix = PdfixEngine::Get();
  if (!pdfix->Authorize(email.c_str(), license_key.c_str()))
    throw std::runtime_error(pdfix->GetError());
    
  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw std::runtime_error(pdfix->GetError());
    
  // prepare platform dependent Windows digital signature
  PdfCertDigSig* dig_sig = pdfix->CreateCertDigSig();
  if (!dig_sig)
    throw std::runtime_error(pdfix->GetError());
  dig_sig->SetReason(L"Testing PDFix API");
  dig_sig->SetLocation(L"Location");
  dig_sig->SetContactInfo(L"info@pdfix.net");
  if (!dig_sig->SetCertContext((void*)cert_context))
    throw std::runtime_error(pdfix->GetError());
  if (!time_stamp_server.empty())
    if (dig_sig->SetTimeStampServer(time_stamp_server.c_str(), L"", L"") == false)
      throw std::runtime_error(pdfix->GetError());

  if (!dig_sig->SignDoc(doc, save_path.c_str()))
    throw std::runtime_error(pdfix->GetError());
  dig_sig->Destroy();
  doc->Close();

  if (cert_context)
    CertFreeCertificateContext(cert_context);
  if (cert_store)
    CertCloseStore(cert_store, 0);
#endif
}