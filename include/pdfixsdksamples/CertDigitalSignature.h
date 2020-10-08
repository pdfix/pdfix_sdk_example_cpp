#pragma once

#include <string>
#ifdef _WIN32
#include <windows.h>
#include <Cryptuiapi.h>
#include <WinCrypt.h>
#include <Cryptdlg.h>
#endif


#ifdef WIN32
static UINT_PTR CALLBACK CertHookCallback(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static BOOL CALLBACK  CertFilterCallback(PCCERT_CONTEXT pCertContext,DWORD lCustData, DWORD dwFlags, DWORD dwDisplayWell);
#endif
  
// Applies digital signature.
void CertDigitalSignature(
    const std::wstring& email,                           // authorization email
    const std::wstring& license_key,                     // authorization license key
    const std::wstring& open_path,                       // source PDF document
    const std::wstring& save_path,                       // signed PDF document
    const std::wstring& time_stamp_server                // time stamp server
    );
