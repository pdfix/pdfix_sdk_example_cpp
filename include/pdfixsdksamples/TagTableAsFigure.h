////////////////////////////////////////////////////////////////////////////////////////////////////
// TagTableAsFigure.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <iostream>

namespace TagTableAsFigure {
  void Run(const std::wstring& open_path,  // source PDF document
          const std::wstring& save_path   // output PDF document
  );
} // namespace TagTableAsFigure
