////////////////////////////////////////////////////////////////////////////////////////////////////
// TagsReadStructTree.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\page CPP_Samples C++ Samples
- \subpage GetTags_cpp
*/
/*!
\page GetTags_cpp Get Tags Sample
Example how to automatically get tags from the document.
\snippet /GetTags.hpp GetTags_cpp
*/

#pragma once

//! [GetTags_cpp]
#include <string>
#include <iostream>

void TagsReadStructTree(
  const std::wstring& open_path,        // source PDF document
  std::ostream& output                  // output stream
);
