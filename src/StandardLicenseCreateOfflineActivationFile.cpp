////////////////////////////////////////////////////////////////////////////////////////////////////
// StandardLicenseActivate.cpp
// Copyright (c) 2019 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

// system
#include <string>
// other libraries
#include "Pdfix.h"

using namespace PDFixSDK;

namespace StandardLicenseCreateOfflineActivationFile {
void Run(const std::wstring& license_key,  // authorization license key
         const std::wstring& request_file  // activation request path
) {
  // initialize Pdfix
  if (!Pdfix_init(Pdfix_MODULE_NAME)) {
    throw std::runtime_error("Pdfix initialization fail");
  }

  Pdfix* pdfix = GetPdfix();
  if (!pdfix) {
    throw std::runtime_error("GetPdfix fail");
  }

  auto authorization = pdfix->GetStandardAuthorization();
  if (!authorization) {
    throw PdfixException();
  }

  if (!authorization->CreateOfflineActivationFile(license_key.c_str(), request_file.c_str())) {
    throw PdfixException();
  }

  pdfix->Destroy();
}
}  // namespace StandardLicenseCreateOfflineActivationFile
