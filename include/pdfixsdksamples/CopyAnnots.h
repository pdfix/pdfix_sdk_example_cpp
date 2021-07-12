#pragma once

#include <string>

namespace CopyAnnots {

  void CopyAnnots(
      const std::wstring& src_path,     // source PDF document
      const std::wstring& dest_path,    // destination PDF doucment
      const std::wstring& save_path,     // output PDF doucment
      int src_page,
      int dest_page
  );

  void CopyCircleAnnotsAsSquares(
      const std::wstring& src_path,     // source PDF document
      const std::wstring& dest_path,    // destination PDF doucment
      const std::wstring& save_path,     // output PDF doucment
      int src_page,
      int dest_page
  );
}
