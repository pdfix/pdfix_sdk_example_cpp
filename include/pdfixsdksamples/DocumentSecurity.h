#pragma once

#include <string>

namespace DocumentSecurity {

  void RemoveSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      const std::wstring& password    // source PDF document passowrd
  );

  void AddSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      const std::wstring& password    // output PDF document passowrd
  );

  void AddCustomSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      uint8_t key                     // key to lock PDF document
  );

  void RemoveCustomSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      uint8_t key                     // key to unlock PDF document
  );

  void PostponedDocumentAuthorization(
    const std::wstring& open_path,  // source PDF document
    const std::wstring& password    // source PDF document password
  );
}
