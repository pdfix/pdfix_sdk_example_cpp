////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadingOrder.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <iostream>

namespace TagsReadingOrder {

  void Run(const std::wstring& open_path,  // source PDF document
           const std::wstring& save_path   // output PDF document
  );
} // namespace TagsReadingOrder
