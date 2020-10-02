#pragma once

#include <string>

// Creates new documents
void CreateNewDocuments(
    const std::wstring& save_path,                 // directory where to save PDF docuemnts
    size_t document_count,                         // count of documents to be created in the directory
    size_t thread_count                            // maximal number of threads to be used
    );
