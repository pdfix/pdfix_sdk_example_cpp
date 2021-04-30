#pragma once

#include <string>

namespace SearchText {
  void Run(
    const std::wstring& open_path,  // source PDF document
    const std::wstring& save_path,  // destynation PDF document
    const std::wstring& query,      // text to search in pdf file
    int page_num = -1               // number of the page where to search, -1 for all pages
  );
}
