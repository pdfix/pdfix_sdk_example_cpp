#pragma once

#include <string>

namespace StandardLicenseCreateOfflineActivationFile {
void Run(const std::wstring& license_key,  // authorization license key
         const std::wstring& request_file  // activation request path
);
}  // namespace StandardLicenseCreateOfflineActivationFile
