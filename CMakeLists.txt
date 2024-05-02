cmake_minimum_required(VERSION 3.10)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

project(pdfixsdksample)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADERS
  include/pdfixsdksamples/Utils.h
  include/pdfixsdksamples/ExtractText.h
  include/pdfixsdksamples/AcroFormExport.h
  include/pdfixsdksamples/AcroFormImport.h
  include/pdfixsdksamples/AddComment.h
  include/pdfixsdksamples/AddTags.h
  include/pdfixsdksamples/AddWatermark.h
  include/pdfixsdksamples/ApplyRedaction.h
  include/pdfixsdksamples/BookmarksToJson.h
  # include/pdfixsdksamples/CertDigitalSignature.h
  # include/pdfixsdksamples/ConvertTaggedPdf.h
  include/pdfixsdksamples/ConvertRGBToCMYK.h
  include/pdfixsdksamples/ConvertToHtml.h
  include/pdfixsdksamples/ConvertToHtmlEx.h
  include/pdfixsdksamples/ConvertToTiff.h
  include/pdfixsdksamples/CopyAnnots.h
  include/pdfixsdksamples/CreateNewDocument.h
  include/pdfixsdksamples/CreateNewDocuments.h
  include/pdfixsdksamples/CreatePage.h
  include/pdfixsdksamples/DeletePages.h
  include/pdfixsdksamples/DigitalSignature.h
  include/pdfixsdksamples/DocumentMetadata.h
  include/pdfixsdksamples/DocumentSecurity.h
  include/pdfixsdksamples/EditContent.h
  include/pdfixsdksamples/EmbedFonts.h
  include/pdfixsdksamples/ExportFormFieldValues.h
  include/pdfixsdksamples/ExtractData.h
  include/pdfixsdksamples/ExtractHighlightedText.h
  include/pdfixsdksamples/ExtractImages.h
  include/pdfixsdksamples/ExtractTables.h
  include/pdfixsdksamples/ExtractText.h
  include/pdfixsdksamples/FillForm.h
  include/pdfixsdksamples/FlattenAnnots.h
  include/pdfixsdksamples/GetWhitespace.h
  include/pdfixsdksamples/ImportFormData.h
  include/pdfixsdksamples/Imposition.h
  include/pdfixsdksamples/Initialization.h
  include/pdfixsdksamples/InsertPages.h
  include/pdfixsdksamples/LicenseReset.h
  include/pdfixsdksamples/LicenseStatus.h
  include/pdfixsdksamples/MakeAccessible.h
  include/pdfixsdksamples/MovePage.h
  include/pdfixsdksamples/NamedDestsToJson.h
  include/pdfixsdksamples/OcrPageImagesWithTesseract.h
  include/pdfixsdksamples/OcrWithTesseract.h
  include/pdfixsdksamples/OpedDocumentFromStream.h
  include/pdfixsdksamples/PagesToJson.h
  include/pdfixsdksamples/ParsePageContent.h
  include/pdfixsdksamples/ParsePdsObjects.h
  include/pdfixsdksamples/PreflightDocTemplate.h
  include/pdfixsdksamples/PrintPage.h
  include/pdfixsdksamples/ReadOCGLayers.h
  include/pdfixsdksamples/RegexSearch.h
  include/pdfixsdksamples/RegexSetPattern.h
  include/pdfixsdksamples/RegisterEvent.h
  include/pdfixsdksamples/RemoveComments.h
  include/pdfixsdksamples/RemoveTags.h
  include/pdfixsdksamples/RenderPage.h
  include/pdfixsdksamples/RenderPages.h
  include/pdfixsdksamples/SearchText.h
  include/pdfixsdksamples/SetAnnotationAppearance.h
  include/pdfixsdksamples/SetFieldFlags.h
  include/pdfixsdksamples/SetFormFieldValue.h
  include/pdfixsdksamples/StandardLicenseActivate.h
  include/pdfixsdksamples/StandardLicenseCreateOfflineActivationFile.h
  include/pdfixsdksamples/StandardLicenseActivateOffline.h
  include/pdfixsdksamples/StandardLicenseDeactivate.h
  include/pdfixsdksamples/StandardLicenseDeactivateOffline.h
  include/pdfixsdksamples/StandardLicenseUpdate.h
  include/pdfixsdksamples/samples.h
  include/pdfixsdksamples/CreateRedactionMark.h
  include/pdfixsdksamples/ProcessControl.h
  include/pdfixsdksamples/EditPageObjectMCID.h
  )

set(SOURCES
  #src/RenderPageWithoutText.cpp
  src/TagAsArtifact.cpp
  #src/ExtractTextFromOCGLayer.cpp
  src/AcroFormExport.cpp
  src/AcroFormImport.cpp
  src/AddComment.cpp
  src/AddTags.cpp
  src/AddWatermark.cpp
  src/ApplyRedaction.cpp
  src/BookmarksToJson.cpp
  # src/CertDigitalSignature.cpp
  #src/ConvertTaggedPdf.cpp
  src/ConvertRGBToCMYK.cpp
  src/ConvertToHtml.cpp
  src/ConvertToHtmlEx.cpp
  src/ConvertToTiff.cpp
  src/CopyAnnots.cpp
  src/CreateNewDocument.cpp
  src/CreateNewDocuments.cpp
  src/CreatePage.cpp
  src/DeletePages.cpp
  src/DigitalSignature.cpp
  src/DocumentMetadata.cpp
  src/DocumentSecurity.cpp
  src/EditContent.cpp
  src/EmbedFonts.cpp
  src/ExtractAnnot.cpp
  src/ExportFormFieldValues.cpp
  src/ExtractHighlightedText.cpp
  src/ExtractImages.cpp
  src/ExtractPageData.cpp
  src/ExtractPageContent.cpp
  src/ExtractPageMap.cpp
  src/ExtractPdfData.cpp
  src/ExtractPdfUtils.cpp
  src/ExtractStructTree.cpp
  src/ExtractTables.cpp
  src/ExtractText.cpp
  src/FillForm.cpp
  src/FlattenAnnots.cpp
  src/GetWhitespace.cpp
  src/ImportFormData.cpp
  src/Imposition.cpp
  src/Initialization.cpp
  src/InsertPages.cpp
  src/LicenseReset.cpp
  src/LicenseStatus.cpp
  src/MakeAccessible.cpp
  src/MovePage.cpp
  src/NamedDestsToJson.cpp
  src/OcrPageImagesWithTesseract.cpp
  src/OcrWithTesseract.cpp
  src/OpedDocumentFromStream.cpp
  src/PagesToJson.cpp
  src/ParsePageContent.cpp
  src/ParsePdsObjects.cpp
  src/PdfixEngine.cpp
  src/PreflightDocTemplate.cpp
  src/PrintPage.cpp
  src/ReadOCGLayers.cpp
  src/RegexSearch.cpp
  src/RegexSetPattern.cpp
  src/RegisterEvent.cpp
  src/RemoveComments.cpp
  src/RemoveTags.cpp
  src/RenderPage.cpp
  src/RenderPages.cpp
  src/SearchText.cpp
  src/SetAnnotationAppearance.cpp
  src/SetFieldFlags.cpp
  src/SetFormFieldValue.cpp
  src/StandardLicenseActivate.cpp
  src/StandardLicenseCreateOfflineActivationFile.cpp
  src/StandardLicenseActivateOffline.cpp
  src/StandardLicenseDeactivate.cpp
  src/StandardLicenseUpdate.cpp
  src/TagAnnotation.cpp
  src/TagHeadings.cpp
  src/TagTableAsFigure.cpp
  src/TagsEditStructTree.cpp
  src/TagsReadStructTree.cpp
  src/TagsReadingOrder.cpp
  src/Utils.cpp
  src/CreateRedactionMark.cpp
  src/ProcessControl.cpp
  src/EditPageObjectMCID.cpp
  )

add_library(pdfixsdksample
  ${HEADERS}
  ${SOURCES}
  include/pdfixsdksamples/StandardLicenseUpdateOffline.h
  src/StandardLicenseDeactivateOffline.cpp
  src/StandardLicenseUpdateOffline.cpp
  )

# idendify project architecture
if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
  set (PROJECTNAME_ARCHITECTURE "x64")
else ()
  set (PROJECTNAME_ARCHITECTURE "x86")
endif ()

# set output directory
if(MSVC)
  if (CMAKE_CL_64 OR ${PROJECTNAME_ARCHITECTURE} EQUAL "x64")  # -- 64-bit builds.
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/x64")
  else()
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/x86")
  endif()
  set(DOWNLOAD_PLATFORM windows.zip)
elseif(APPLE)
  if("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "arm64")
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/arm64")
  elseif("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "x86_64")
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/x64")
  endif()
  set(DOWNLOAD_PLATFORM macos.zip)
elseif(UNIX)
  set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/x64")
  set(DOWNLOAD_PLATFORM linux.tar.gz)
else()
endif()

include(FetchContent)

FetchContent_Declare(
  pdfixdist  
  URL "https://github.com/pdfix/pdfix_sdk_builds/releases/download/8.0.0/pdfix_sdk_8.0.0_ede0c5cf_${DOWNLOAD_PLATFORM}"
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/pdfix_sdk
  )

if(NOT TARGET pdfix_app)
  message("Downloading from https://github.com/pdfix/pdfix_sdk_builds")
  FetchContent_Populate(pdfixdist)
  file(COPY ${pdfixdist_SOURCE_DIR}/bin DESTINATION  ${CMAKE_SOURCE_DIR})
endif()

set_target_properties(pdfixsdksample
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED TRUE
  RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}
  )

if(APPLE)
    set(BUILD_APPLE_COMPILER_DEFINITIONS _LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
endif()


target_compile_definitions(pdfixsdksample
  PUBLIC
  NOMINMAX 
  UNICODE
  _UNICODE
  "$<$<CONFIG:DEBUG>:_DEBUG>"
  BOOST_BIND_GLOBAL_PLACEHOLDERS
  ${BUILD_APPLE_COMPILER_DEFINITIONS}
  )  

target_include_directories(pdfixsdksample
  PUBLIC
  "$<IF:$<TARGET_EXISTS:pdfix_app>,${CMAKE_SOURCE_DIR}/libs/boost/include,${CMAKE_SOURCE_DIR}/external>"
  "$<IF:$<TARGET_EXISTS:pdfix_app>,${CMAKE_SOURCE_DIR}/libs/boost_1_78_0,${CMAKE_SOURCE_DIR}/external>"
  "$<IF:$<TARGET_EXISTS:pdfix_app>,${CMAKE_SOURCE_DIR}/include/c++,${pdfixdist_SOURCE_DIR}/include/c++>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  )

set_target_properties(pdfixsdksample 
  PROPERTIES
  PUBLIC_HEADER "include/pdfixsdksamples/samples.h"
  )

if(UNIX)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
    "-ldl"
    "-lpthread"
  )
endif()
 

add_subdirectory(example)
