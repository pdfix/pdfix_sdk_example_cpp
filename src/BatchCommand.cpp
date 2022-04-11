////////////////////////////////////////////////////////////////////////////////////////////////////
// AddComment.cpp
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/BatchCommand.h"

#include <string>
#include <iostream>
#include <filesystem>
// JSON parser
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Pdfix.h"

#include "pdfixsdksamples/PdfixEngine.h"
#include "pdfixsdksamples/ExtractData.h"

using namespace PDFixSDK;
using namespace boost::property_tree;

extern std::wstring FromUtf8(const std::string& str);
extern std::string ToUtf8(const std::wstring& wstr);

namespace BatchCommand {

  // command configuration json example:
  // {
  //   "commands": [
  //     {
  //       "name" : "make-accessible",
  //       "description": "Make PDF accessible",
  //       "params": [
  //         "config-path": "<template_config_path.json>" 
  //       ]
  //     },
  //     {
  //       "name" : "set-lang",
  //       "description": "Set document language",
  //       "params": [
  //         "lang": "en-US" 
  //       ]
  //     },
  //     {
  //       "name" : "set-title",
  //       "description": "Set document title",
  //       "params": [
  //         "title": "Document title" 
  //       ]
  //     },
  //     {
  //       "name":" "doc-info"
  //       "description": "Get the document information",
  //     }
  //   ]
  // }

  void make_accessible(PdfDoc* doc, bool preflight, const std::wstring& config_path){
    auto pdfix = PdfixEngine::Get();
    auto doc_template = doc->GetTemplate();
    if (!doc_template)
      throw PdfixException();

    doc_template->SetDefaults();

    if (!config_path.empty()) {
      PsFileStream* stm = pdfix->CreateFileStream(config_path.c_str(), kPsReadOnly);
      if (stm) {
        if (!doc_template->LoadFromStream(stm, kDataFormatJson))
          throw PdfixException();
        stm->Destroy();
      }
    }

    if (preflight) {
      // add reference pages for preflight
      for (auto i = 0; i < doc->GetNumPages(); i++) {
        if (!doc_template->AddPage(i, nullptr, nullptr))
          throw PdfixException();
      }
        
      // run document preflight
      if (!doc_template->Update(nullptr, nullptr))
        throw PdfixException();
    }

    // convert to PDF/UA
    PdfAccessibleParams params;
    params.embed_fonts = 1;
    params.subset_fonts = 1;
    //params.accept_tags = 1;
  
    if (!doc->MakeAccessible(&params, nullptr, nullptr))
      throw PdfixException();
  }

  void process_command(PdfDoc* doc, ptree& cmd, ptree& cmd_result) {

    auto name = cmd.get_optional<std::string>("name");
    if (!name)
      throw std::runtime_error("invalid command config");
    auto params = cmd.get_child_optional("params");

    if (name.get() == "make-accessible") {
      std::wstring config_path;
      bool preflight = false;
      if (params) {
        auto config_path_value = params->get_optional<std::string>("config-path");
        config_path = config_path_value ? FromUtf8(config_path_value.get()) : L"";

        auto preflight_node = params->get_optional<bool>("preflight");
        preflight = preflight_node ? preflight_node.get() : false;
      }

      make_accessible(doc, preflight, config_path.c_str());
    }
    else if (name.get() == "set-lang") {
      std::wstring lang;
      if (params) {
        auto lang_value = params->get_optional<std::string>("lang");
        lang = lang_value ? FromUtf8(lang_value.get()) : L"";
      }
      doc->SetLang(lang.c_str());
    }
    else if (name.get() == "set-title") {
      std::wstring title;
      if (params) {
        auto title_value = params->get_optional<std::string>("title");
        title = title_value ? FromUtf8(title_value.get()) : L"";
      }
      doc->SetInfo(L"Title", title.c_str());
    }
    else if (name.get() == "doc-info") {
      ptree cmd_output;
      ExtractData::DataType data_type;
      data_type.doc_info = true;
      ExtractData::ExtractDocumentInfo(doc, cmd_output, data_type);
      cmd_result.put_child("output", cmd_output);
    } else {
      cmd_result.put("error", "command not found");
    }

    cmd_result.put("name", name.get());
    if (params)
      cmd_result.put_child("params", params.get());
  }

  // Execute multiple commands in a batch.
  void Run(
      const std::wstring& open_file,                // source PDF document
      const std::wstring& password,                 // open password
      const std::wstring& save_file,                // path to save PDF docuemnt
      const std::wstring& batch_config,             // command configuration JSON
      std::ostream& output                          // command output (json)
      ) {
    auto pdfix = PdfixEngine::Get();

    if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
      pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
      pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
      throw std::runtime_error("Incompatible version");

    PdfDoc* doc = pdfix->OpenDoc(open_file.c_str(), password.c_str());
    if (!doc)
      throw PdfixException();

    // process the command
    ptree commands_json;
    read_json(ToUtf8(batch_config), commands_json);

    ptree results;
    auto commands = commands_json.get_child_optional("commands");
    if (commands) {
      for (auto& cmd : commands.get()) {
        ptree cmd_result;
        process_command(doc, cmd.second, cmd_result);
        results.push_back(std::make_pair("", cmd_result));
      }
    }

    ptree json;
    json.add_child("output", results);

    write_json(output, json);

    // save only if modified
    if (doc->GetFlags() & kDocIsModified) {
      // save changes if modified
      if (!doc->Save(save_file.c_str(), kSaveFull))
        throw PdfixException();
    }
    else if (!save_file.empty()) {
      // no changes - copy original file
      std::filesystem::copy(open_file, save_file);
    }
    doc->Close();
  }
}