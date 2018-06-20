////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016 PDFix. All Rights Reserved.
// This file was generated automatically
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _OcrTesseract_h
#define _OcrTesseract_h

#include <stdint.h>

#define OCRTESSERACT_VERSION_MAJOR 1
#define OCRTESSERACT_VERSION_MINOR 0
#define OCRTESSERACT_VERSION_PATCH 1
#define _in_
#define _out_
#define _callback_

struct OcrTesseract;
struct TesseractDoc;

typedef int OcrTesseractFlags;
typedef OcrTesseract* OcrTesseractP;
typedef TesseractDoc* TesseractDocP;

enum {
  kErrorOcrTesseract = 2000,
  kErrorOcrTesseractInitialization = 2001,
  kErrorOcrTesseractMissingData = 2002,
  kErrorOcrTesseractMissingLanguage = 2003,
  kErrorOcrTesseractMissingPageImage = 2004,
  kErrorOcrTesseractProcessDoc = 2005,
  kErrorOcrTesseractProcessPage = 2006,
} ;

enum {
  kOcrNone = 0x00,
  kOcrImages = 0x0001,
} ;

typedef enum {
  kOcrSegOSDOnly = 0,
  kOcrSegAutoOSD = 1,
  kOcrSegAutoOnly = 2,
  kOcrSegAuto = 3,
  kOcrSegSingleColumn = 4,
  kOcrSegSingleBlockVertText = 5,
  kOcrSegSingleBlock = 6,
  kOcrSegSingleLine = 7,
  kOcrSegSingleWord = 8,
  kOcrSegCircleWord = 9,
  kOcrSegSingleChar = 10,
  kOcrSegSparseText = 11,
  kOcrSegSparseTextOSD = 12,
  kOcrSegRawLine = 13,
} OcrTesseractPageSegType;

typedef enum {
  kOcrTesseractOnly = 0,
  kOcrTesseractLSTMOnlly = 1,
  kOcrTesseractLSTMCombined = 2,
  kOcrTesseractDefault = 3,
} OcrTesseractEngineType;


typedef struct _OcrTesseractParams {
  OcrTesseractFlags flags;
  PdfDevRect clip_rect;
  OcrTesseractPageSegType page_seg;
  OcrTesseractEngineType engine;
  double zoom;
  PdfRotate rotate;
  _OcrTesseractParams() {
    flags = 0;
    zoom = 1;
    rotate = kRotate0;
    page_seg = kOcrSegAutoOSD;
    engine = kOcrTesseractDefault;
  }
} OcrTesseractParams;


struct OcrTesseract : PdfixPlugin {
  virtual bool SetLanguage(const wchar_t* lang) = 0;
  virtual bool SetData(const wchar_t* path) = 0;
  virtual TesseractDoc* OpenOcrDoc(PdfDoc* pdDoc) = 0;
};

struct TesseractDoc {
  virtual bool Close() = 0;
  virtual bool Save(const wchar_t* path, OcrTesseractParams* params, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
};

///////////////////////////////////////////////////////////////////////////////
// OcrTesseract initialization

#ifdef PDFIX_STATIC_LIB
OcrTesseract* GetOcrTesseract();
#else

#ifdef _WIN32
#include <Windows.h>
#define DLL_HANDLE HMODULE
#define PdfixLoadLibrary LoadLibraryA
#define PdfixFreeLibrary FreeLibrary
#define PdfixGetProcAddress GetProcAddress
#else //UNIX
#include <dlfcn.h>
#define DLL_HANDLE void*
#define PdfixLoadLibrary(name) dlopen(name, RTLD_NOW)
#define PdfixFreeLibrary dlclose
#define PdfixGetProcAddress dlsym
#endif //  _WIN32

// method prototypes
typedef OcrTesseract* (*GetOcrTesseractProcType)( );

// initialization
extern DLL_HANDLE* OcrTesseract_init(const char* path);
extern void OcrTesseract_destroy();
// static declarations
extern DLL_HANDLE g_OcrTesseract_handle;
extern GetOcrTesseractProcType GetOcrTesseract;

// static definitions OcrTesseract (use in the source file)
#define OcrTesseract_statics \
GetOcrTesseractProcType GetOcrTesseract = nullptr;\
DLL_HANDLE g_OcrTesseract_handle = 0;\
void OcrTesseract_destroy() {\
  if (g_OcrTesseract_handle) PdfixFreeLibrary(g_OcrTesseract_handle);\
  g_OcrTesseract_handle = nullptr;\
  GetOcrTesseract = 0;\
}\
DLL_HANDLE* OcrTesseract_init(const char* path) {\
  g_OcrTesseract_handle = PdfixLoadLibrary(path);   if (!g_OcrTesseract_handle) return nullptr;\
  GetOcrTesseract = (GetOcrTesseractProcType)PdfixGetProcAddress(g_OcrTesseract_handle, "GetOcrTesseract");\
  if (GetOcrTesseract == nullptr) { OcrTesseract_destroy(); return nullptr; } return &g_OcrTesseract_handle; }

#ifndef OcrTesseract_MODULE_NAME
#if defined _WIN32
#if defined _WIN64
#define OcrTesseract_MODULE_NAME "ocr_tesseract64.dll"
#else
#define OcrTesseract_MODULE_NAME "ocr_tesseract.dll"
#endif
#elif defined __linux__
#if defined __x86_64__
#define OcrTesseract_MODULE_NAME "libocr_tesseract64.so"
#else
#define OcrTesseract_MODULE_NAME "libocr_tesseract.so"
#endif
#elif defined __APPLE__
#if defined __x86_64__
#define OcrTesseract_MODULE_NAME "libocr_tesseract64.dylib"
#else
#define OcrTesseract_MODULE_NAME "libocr_tesseract.dylib"
#endif
#else
#error unknown platform
#endif
#endif //OcrTesseract_MODULE_NAME

#endif // PDFIX_STATIC_LIB
#endif //_OcrTesseract_h
