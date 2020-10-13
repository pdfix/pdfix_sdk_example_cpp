#pragma once

#include <string>

// TODO: redaction rectangle

// Creates redaction mark and saves it to the new document
void CreateReactionMark(
  const std::wstring& open_file,                // source PDF document
  const std::wstring& save_file,                // file path where to save PDF docuemnt
  int page_num                                   // index of page where to create redaction mark
);
