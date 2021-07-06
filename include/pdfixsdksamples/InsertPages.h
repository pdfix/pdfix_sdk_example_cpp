#pragma once

#include <string>

// Insert pages from one document to another.
void InsertPages(
  const std::wstring& source_doc_file,          // source PDF document
  const std::wstring& dest_doc_file,            // destination PDF document
  const std::wstring& save_file,                // file where to save PDF docuemnt
  int where,                                    // index where to insert pages
  int from,                                     // from page
  int to                                        // to oage
  );

