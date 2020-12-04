////////////////////////////////////////////////////////////////////////////////////////////////////
// TagAsArtifact.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


#include <string>
#include <iostream>
#include "Pdfix.h"

using namespace PDFixSDK;

namespace TagAsArtifact {
  
  // find any non-tagged objects in the page content and mark them as artifact
  void MarkUntaggedObjectsAsArtifact(PdfPage* page);
  
  // re-tag the struct element to heading based on font properties
  void RemoveParagraph(PdsStructElement* struct_elem);

  // re-tag P tags to H1 or H2 tag based on predefined font properties (size, font name)
  void Run(
    const std::wstring& open_path,        // source PDF document
    const std::wstring& save_path         // output PDF document
  );
} //  namespace TagAsArtifact

