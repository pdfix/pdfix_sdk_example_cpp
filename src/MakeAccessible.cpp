////////////////////////////////////////////////////////////////////////////////////////////////////
// MakeAccessible.h
// Copyright (c) 2018 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/MakeAccessible.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include "Pdfix.h"
#include "pdfixsdksamples/PdfixEngine.h"

using namespace PDFixSDK;

// returns the "name" of a JSON command stored in the stream, or an empty string
static std::string GetCommandName(PsStream* stm) {
  std::string json_content(stm->GetSize(), '\0');
  stm->Read(0, reinterpret_cast<uint8_t*>(json_content.data()), json_content.size());

  boost::property_tree::ptree json;
  std::istringstream json_stream(json_content);
  boost::property_tree::read_json(json_stream, json);
  return json.get<std::string>("name", "");
}

void MakeAccessible(
    const std::wstring& open_path,     // source PDF document
    const std::wstring& save_path,     // output PDF/UA document
    const std::wstring& command_path,  // command configuration file
    const std::wstring& language,  // document language to set and overide the command configuration
    const std::wstring& title      // document title to set and overide the command configuration
) {
  auto pdfix = PdfixEngine::Get();

  PdfDoc* doc = pdfix->OpenDoc(open_path.c_str(), L"");
  if (!doc)
    throw PdfixException();

  auto command = doc->GetCommand();

  // if command path is empty, use the default command from the SDK, othervise use the provided path
  // to JSON

  PsStream* cmd_stm = nullptr;

  if (command_path.empty()) {
    // find the make-accessible command in all embedded commands
    auto cmd_count = command->GetNumCustomActions();

    for (int i = 0; i < cmd_count; i++) {
      cmd_stm = pdfix->CreateMemStream();
      if (!cmd_stm) {
        throw PdfixException();
      }

      if (!command->SaveCustomActionToStream(i, cmd_stm, kDataFormatJson, kSaveFull)) {
        throw PdfixException();
      }

      // find command with name 'make_accessible'
      std::string name;
      try {
        name = GetCommandName(cmd_stm);
      } catch (std::exception& ex) {
        std::cout << "[WARNING]: Loading JSON command failed. [" << std::to_string(i) << "] "
                  << ex.what() << std::endl;
      }

      if (name == "make_accessible") {
        break;
      }
      cmd_stm->Destroy();
      cmd_stm = nullptr;
    }
  } else {
    cmd_stm = pdfix->CreateFileStream(command_path.c_str(), kPsReadOnly);
    if (!cmd_stm) {
      throw PdfixException();
    }
  }

  if (cmd_stm != nullptr) {
    if (!command->LoadParamsFromStream(cmd_stm, kDataFormatJson)) {
      throw PdfixException();
    }
    cmd_stm->Destroy();
  }

  if (!command->Run()) {
    throw PdfixException();
  }

  if ((!language.empty()) && (!doc->SetLang(language.c_str()))) {
    throw PdfixException();
  }

  if ((!title.empty()) && (!doc->SetInfo(L"Title", title.c_str()))) {
    throw PdfixException();
  }

  if (!doc->Save(save_path.c_str(), kSaveFull))
    throw PdfixException();

  doc->Close();
}
