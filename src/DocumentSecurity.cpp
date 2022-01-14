////////////////////////////////////////////////////////////////////////////////////////////////////
// DocumentSecurity.cpp
// Copyright (c) 2021 Pdfix. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pdfixsdksamples/DocumentSecurity.h"

// project
#include "Pdfix.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#define UNUSED(x) (void)(x)

using namespace PDFixSDK;

namespace DocumentSecurity {

////////////////////////////////////////////////////////////////////////////////////////////////////
// helper structures
////////////////////////////////////////////////////////////////////////////////////////////////////

  class XorSecurityHandler {
   public:
    static const wchar_t* kFilterName;
   private:
    uint8_t m_cipher_key;
    static const wchar_t* kSecret;
  
    template<typename T>
    void crypt_data(const T* data, int data_size, T* dest) {
      for (int i = 0; i < data_size; i++) {
        dest[i] = data[i] ^ m_cipher_key;
      }
    }

    std::wstring to_hex(const wchar_t* data, size_t length) {
      std::wstringstream stream;
      stream << std::hex;
      for (size_t i = 0; i < length; i++) {
        stream << std::setw(2) << std::setfill(L'0') << (uint8_t)(data[i] & 0xff);
      }
      return stream.str();
    }

    std::wstring from_hex(const wchar_t* data, size_t length) {
      assert(length % 2 == 0);

      std::wstring result;

      auto steps = length >> 1;
      result.resize(steps);

      for (size_t i = 0; i < steps; i++) {
        unsigned int x;

        std::wstringstream stream;
        stream << std::hex;
        stream << data[2*i] << data[2*i + 1];
        stream >> x;

        result[i] = (wchar_t)x;
      }
      return result;
    }
   public:
    XorSecurityHandler() : m_cipher_key(0) {}
    XorSecurityHandler(uint8_t cipher_key) : m_cipher_key(cipher_key) {}

    void SetKey(uint8_t key) {
      m_cipher_key = key;
    }

    bool OnInit(const PdsDictionary* trailer) {
      auto nonconst_trailer = const_cast<PdsDictionary*>(trailer);
      auto encrypt_dict = nonconst_trailer->GetDictionary(L"Encrypt");
      if (!encrypt_dict) {
        return false;
      }
      auto filter_name = encrypt_dict->GetText(L"Filter");
      if (filter_name != kFilterName) {
        return false;
      }

      // check the cipher key be encrypting secret
      auto secret = encrypt_dict->GetText(L"Secret");
      secret = from_hex(secret.c_str(), secret.length());
      crypt_data(secret.c_str(), secret.length(), secret.data());

      if (secret != kSecret) {
        return false;
      }

      return true;
    }

    uint32_t GetPermissions() {
      return -1;
    }

    bool IsMetadataEncrypted() {
      return false;
    }

    void UpdateEncryptDict(PdsDictionary* encrypt_dict, const PdsArray* id_array) {
      UNUSED(id_array);
      encrypt_dict->PutName(L"Filter", kFilterName);

      std::wstring secret = kSecret;
      crypt_data(secret.c_str(), secret.length(), secret.data());    
      secret = to_hex(secret.c_str(), secret.length());

      encrypt_dict->PutString(L"Secret", secret.c_str());
    }

    int GetCryptSize(const uint8_t* data, int size) {
      UNUSED(data);
      return size;
    }

    int CryptData(
      const uint8_t* data, int data_size,
      uint8_t* dest, int dest_size) {

      assert(dest_size >= data_size);
      crypt_data(data, data_size, dest);

      return data_size;
    }
  };

  const wchar_t* XorSecurityHandler::kFilterName = L"XorCipher";
  const wchar_t* XorSecurityHandler::kSecret = L"hello world";

  PdfCustomSecurityHandler* CreateXorSecurityHandler(Pdfix* pdfix, XorSecurityHandler* xor_handler) {
    auto security_handler = pdfix->CreateCustomSecurityHandler(XorSecurityHandler::kFilterName, xor_handler);
    security_handler->SetDestroyProc([](void* client_data) {
      auto handler = static_cast<XorSecurityHandler*>(client_data);
      if (handler)
        delete handler;
    });
    security_handler->SetSetAuthoziationDataProc([](void* data, void* client_data) -> bool {
      auto key = *static_cast<uint8_t*>(data);
      static_cast<XorSecurityHandler*>(client_data)->SetKey(key);
      return true;
    });
    security_handler->SetOnInitProc([](const PdsDictionary* trailer, void* client_data) {
      return static_cast<XorSecurityHandler*>(client_data)->OnInit(trailer);
    });
    security_handler->SetGetPermissionsProc([](void* client_data) {
      return (int)static_cast<XorSecurityHandler*>(client_data)->GetPermissions();
    });
    security_handler->SetIsMetadataEncryptedProc([](void* client_data) {
      return static_cast<XorSecurityHandler*>(client_data)->IsMetadataEncrypted();
    });
    security_handler->SetUpdateEncryptDictProc([](PdsDictionary* encrypt_dict, const PdsArray* id_array, void* client_data) {
      static_cast<XorSecurityHandler*>(client_data)->UpdateEncryptDict(encrypt_dict, id_array);
    });
    security_handler->SetGetDecryptSizeProc([](const void* data, int size, void* client_data) {
      return static_cast<XorSecurityHandler*>(client_data)->GetCryptSize(static_cast<const uint8_t*>(data), size);
    });
    security_handler->SetDecryptContentProc(
      [](int objnum, int gennum,
         const void* data, int data_size,
         void* dest, int dest_size,
         void* client_data) {
      UNUSED(objnum);
      UNUSED(gennum);
      return static_cast<XorSecurityHandler*>(client_data)->CryptData(
        static_cast<const uint8_t*>(data), data_size,
        static_cast<uint8_t*>(dest), dest_size
      );
    });
    security_handler->SetGetEncryptSizeProc([](const void* data, int size, void* client_data) -> int {
      return static_cast<XorSecurityHandler*>(client_data)->GetCryptSize(static_cast<const uint8_t*>(data), size);
    });
    security_handler->SetEncryptContentProc(
      [](int objnum, int gennum,
         const void* data, int data_size,
         void* dest, int dest_size,
         void* client_data) {
      UNUSED(objnum);
      UNUSED(gennum);
      return static_cast<XorSecurityHandler*>(client_data)->CryptData(
        static_cast<const uint8_t*>(data), data_size,
        static_cast<uint8_t*>(dest), dest_size
      );
    });
    return security_handler;
  }

  PdfSecurityHandler* CreateXorSecurityHandler(void* client_data) {
    auto pdfix = static_cast<Pdfix*>(client_data);
    auto xor_handler = new XorSecurityHandler();
    return CreateXorSecurityHandler(pdfix, xor_handler);
  }

  Pdfix* InitPdfix() {
        // initialize Pdfix
    if (!Pdfix_init(Pdfix_MODULE_NAME))
      throw std::runtime_error("Pdfix initialization fail");

    Pdfix* pdfix = GetPdfix();
    if (!pdfix)
      throw std::runtime_error("GetPdfix fail");

    if (pdfix->GetVersionMajor() != PDFIX_VERSION_MAJOR || 
      pdfix->GetVersionMinor() != PDFIX_VERSION_MINOR ||
      pdfix->GetVersionPatch() != PDFIX_VERSION_PATCH)
      throw std::runtime_error("Incompatible version");
    
    return pdfix;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
// actual examples
////////////////////////////////////////////////////////////////////////////////////////////////////

  void RemoveSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      const std::wstring& password    // source PDF document password
  ) {
    auto pdfix = InitPdfix();
    auto doc = pdfix->OpenDoc(open_path.c_str(), password.c_str());
    if (!doc)
      throw std::runtime_error(pdfix->GetError());

    // remove document security by setting security handler to null
    doc->SetSecurityHandler(nullptr);

    if (!doc->Save(save_path.c_str(), kSaveFull) )
      throw std::runtime_error(pdfix->GetError());

    doc->Close();
    pdfix->Destroy();
  }

  void AddSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      const std::wstring& password    // output PDF document password
  ) {
    auto pdfix = InitPdfix();
    auto doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw std::runtime_error(pdfix->GetError());

    PdfStandardSecurityParams encryption_params;
    auto security_handler = pdfix->CreateStandardSecurityHandler(password.c_str(), &encryption_params);

    // new security handler will be used when saving the document
    doc->SetSecurityHandler(security_handler);

    if (!doc->Save(save_path.c_str(), kSaveFull) )
      throw std::runtime_error(pdfix->GetError());

    doc->Close();
    pdfix->Destroy();
  }

  void AddCustomSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      uint8_t key                     // key to lock PDF document
  ) {
    auto pdfix = InitPdfix();
    auto doc = pdfix->OpenDoc(open_path.c_str(), L"");
    if (!doc)
      throw std::runtime_error(pdfix->GetError());

    auto xor_handler = new XorSecurityHandler(key);
    auto security_handler = CreateXorSecurityHandler(pdfix, xor_handler);

    // new security handler will be used when saving the document
    doc->SetSecurityHandler(security_handler);

    if (!doc->Save(save_path.c_str(), kSaveFull) )
      throw std::runtime_error(pdfix->GetError());

    doc->Close();
    pdfix->Destroy();
  }

  void RemoveCustomSecurity(
      const std::wstring& open_path,  // source PDF document
      const std::wstring& save_path,  // output PDF doucment
      uint8_t key                     // key to unlock PDF document
  ) {
    auto pdfix = InitPdfix();
    auto doc = pdfix->OpenDoc(open_path.c_str(), nullptr);
    if (!doc)
      throw std::runtime_error(pdfix->GetError());

    // new security handler will be used when saving the document
    pdfix->RegisterSecurityHandler(CreateXorSecurityHandler, XorSecurityHandler::kFilterName, pdfix);

    auto get_auth_data = [](PdfDoc* doc, PdfSecurityHandler* handler, void* data) -> bool {
      UNUSED(doc);
      auto filter = handler->GetFilter();
      if (filter == XorSecurityHandler::kFilterName) {
        auto custom_handler = static_cast<PdfCustomSecurityHandler*>(handler);
        custom_handler->SetAuthorizationData(data);
        return true;
      }

      return false;
    };

    auto cipher_key = key;
    if (!doc->Authorize(get_auth_data, &cipher_key)) {
      throw std::runtime_error(pdfix->GetError());
    }

    // do something with the document
    auto num_objects = 0;
    auto num_pages = doc->GetNumPages();
    for (int i = 0; i < num_pages; i++) {
      auto page = doc->AcquirePage(i);
      num_objects += page->GetContent()->GetNumObjects();
      page->Release();
    }

    std::cout << "Total object count: " << num_objects << std::endl;

    doc->SetSecurityHandler(nullptr);

    if (!doc->Save(save_path.c_str(), kSaveFull) )
      throw std::runtime_error(pdfix->GetError());

    doc->Close();
    pdfix->Destroy();
  }

  void PostponedDocumentAuthorization(
    const std::wstring& open_path,  // source PDF document
    const std::wstring& password    // source PDF document password
  ) {
    auto pdfix = InitPdfix();
    auto doc = pdfix->OpenDoc(open_path.c_str(), nullptr);
    if (!doc)
      throw std::runtime_error(pdfix->GetError());

    auto get_auth_data = [](PdfDoc* doc, PdfSecurityHandler* handler, void* data) -> bool {
      UNUSED(doc);
      auto filter = handler->GetFilter();
      if (filter == L"Standard") {
        auto std_handler = static_cast<PdfStandardSecurityHandler*>(handler);
        const wchar_t* password = static_cast<const wchar_t*>(data);
        std_handler->SetPassword(password);
        return true;
      }

      return false;
    };

    std::wstring pass = password;
    if (doc->IsSecured() && !doc->Authorize(get_auth_data, pass.data())) {
      throw std::runtime_error(pdfix->GetError());
    }

    // do something with the document
    auto num_objects = 0;
    auto num_pages = doc->GetNumPages();
    for (int i = 0; i < num_pages; i++) {
      auto page = doc->AcquirePage(i);
      num_objects += page->GetContent()->GetNumObjects();
      page->Release();
    }

    std::cout << "Total object count: " << num_objects << std::endl;

    doc->Close();
    pdfix->Destroy();
  }
};

