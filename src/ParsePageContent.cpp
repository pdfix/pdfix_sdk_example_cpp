////////////////////////////////////////////////////////////////////////////////////////////////////
// ParsePageContent.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/ParsePageContent.h"

// system
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// project
#include "pdfixsdksamples/Utils.h"
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

namespace ParsePageContent {

  // ProcessObject gets the value of the object.
  void ProcessPageObject(PdsPageObject* obj, std::ostream& ss, std::string indent) {
    
    if (!obj) throw PdfixException();
    indent += "  ";
    ss << indent << "type: ";
    switch (obj->GetObjectType()) {
      case kPdsPageUnknown:
        ss << "unknown" << std::endl; break;
      case kPdsPageText: {
        ss << "text" << std::endl;
        auto text = ((PdsText*)obj);
        auto str = text->GetText();
        ss << indent << "text: " << ToUtf8(str) << std::endl;
        break;
      }
      case kPdsPageImage:{
        auto image = (PdsImage*)obj;
        auto stream = image->GetDataStm();
        ss << "image" << std::endl; 
        auto sz = stream->GetSize();
        ss << indent << "image_stream_size: " << sz << std::endl;
        //std::vector<unsigned char> data;
        //data.resize(sz);
        //if (!stream->Read(0, &data[0], sz))
        //  throw PdfixException();
        //auto str = std::string(begin(data), begin(data) + std::min(sz, 100));
        //ss << indent << "image_stream: " << str << std::endl ;
        break;
        }
      case kPdsPagePath:{
        auto path = (PdsPath*)obj;
        auto points_num = path->GetNumPathPoints();
        ss << "path" << std::endl;
        ss << indent << "points_num: " << points_num << std::endl;
        break;
        }
        
      case kPdsPageShading:{
        auto shading = (PdsShading*)obj;
        auto id = shading->GetId();
        ss << "shading" << std::endl;
        ss << "blend_mode: " << shading->GetGState().blend_mode << std::endl;
        break;
        }
      case kPdsPageForm: {
        ss << "form" << std::endl;
        auto form = (PdsForm*)obj;
        auto content = form->GetContent();
        auto objects_num = content->GetNumObjects();
        if (objects_num > 0) {
          ss << indent << "/kids" << std::endl;
          indent += "  ";
          for (int i = 0; i < content->GetNumObjects(); i++) {
            ss << indent << "/" << i << std::endl;
            auto kid = content->GetObject(i);
            ProcessPageObject(kid, ss, indent );
          }
          indent = indent.substr(0, indent.length() - 2);
        }
        break;
      }
    }

    PdfRect bbox = obj->GetBBox();
    ss << indent << "/bbox" << std::endl;
    indent += "  ";
    ss << indent << "left: " << (float)bbox.left << std::endl;
    ss << indent << "top: " << (float)bbox.top << std::endl;
    ss << indent << "right: " << (float)bbox.right << std::endl;
    ss << indent << "bottom: " << (float)bbox.bottom << std::endl;
    indent = indent.substr(0,indent.length()-2);
    
    auto content_mark = obj->GetContentMark();
    if (content_mark) {
      for (int i = 0; i < content_mark->GetNumTags(); i++) {
        ss << indent << "/" << ToUtf8(content_mark->GetTagName(i)) << " ";
        auto dict = content_mark->GetTagObject(i);
        if (dict) {
          ss << "<<";
          for (int j = 0; j < dict->GetNumKeys(); j++) {
            auto key = dict->GetKey(j);
            ss << "/" << ToUtf8(key) << "(" << ToUtf8(dict->GetText(key.c_str())) << ") ";
          }
          ss << ">>";
        }
        ss << std::endl;
      }
    }
  }

  // Iterates the content on a page
  void Run(
    const std::wstring& open_path,              // source PDF document
    std::ostream& output,                       // output document
    int page_num
    ) {
    auto pdfix = PdfixEngine::Get();

    PdfDoc* doc = nullptr;
    doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw PdfixException();
    
    PdfPage* page = doc->AcquirePage(page_num);
    if (!page)
      throw PdfixException();
      
    std::string indent = "  ";
    // get page content flags
    auto flags = page->GetContentFlags();
    output << "page_num " << page_num << std::endl;
    output << "/content_flags" << std::endl;
    output << indent << "text: " << (((flags & kContentText) != 0) ? "true" : "false") << std::endl;
    output << indent << "image: " << (((flags & kContentImage) != 0) ? "true" : "false") << std::endl;
    output << indent << "path: " << (((flags & kContentPath) != 0) ? "true" : "false") << std::endl;
    output << indent << "form: " << (((flags & kContentForm) != 0) ? "true" : "false") << std::endl;
    output << indent << "shading: " << (((flags & kContentShading) != 0) ? "true" : "false") << std::endl;
    output << indent << "text_transparent: " << (((flags & kContentTextTransparent) != 0) ? "true" : "false") << std::endl;
    output << indent << "text_fill: " << (((flags & kContentTextFill) != 0) ? "true" : "false") << std::endl;
    output << indent << "text_stroke: " << (((flags & kContentTextStroke) != 0) ? "true" : "false") << std::endl;
   
    output << "/root" << std::endl;
    output << indent << "/kids" << std::endl;
    indent += "  ";
    auto content = page->GetContent();
    auto count = content->GetNumObjects();
    for (int i = 0; i < count; i++) {
      output << indent << "/" << i << std::endl;
      ProcessPageObject(content->GetObject(i), output, indent);
    }

    page->Release();

    doc->Close();
  }
}