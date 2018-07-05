////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016 PDFix. All Rights Reserved.
// This file was generated automatically
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _Pdfix_h
#define _Pdfix_h

#include <stdint.h>

#define PDFIX_VERSION_MAJOR 4
#define PDFIX_VERSION_MINOR 0
#define PDFIX_VERSION_PATCH 4
#define MAX_INT 2147483647
#define MIN_INT -2147483647
#define kTemplateFlatAnnots "flat-annots"
#define kTemplateAcceptTags "accept-tags"
#define kTemplateNoJustifiedText "no-justified-text"
#define kTemplateMaxWordSpacing "max-word-spacing"
#define kTemplateHeaderRatio "header-ratio"
#define kTemplateFooterRatio "footer-ratio"
#define kTemplateObjectIntersection "object-intersection"
#define kTemplateIsolatedIntersectionH "isolated-intersection-h"
#define kTemplateIsolatedIntersectionV "isolated-intersection-v"
#define kTemplateIgnoreTextTables "ignore-text-tables"
#define kTemplateIgnoreBgTables "ignore-background-tables"
#define kTemplateIsolatedImage "isolated-image"
#define kTemplateTextOnly "text-only"
#define kTrTextHeight "kTrTextHeight"
#define kTrBBoxExpansion "kTrBBoxExpansion"
#define kTrChartBackgroundRatio "kTrChartBackgroundRatio"
#define kTrRectIsLine "kTrRectIsLine"
#define kTrAngleDeviation "kTrAngleDeviation"
#define kLineIntersection "kLineIntersection"
#define kTrIsolatedText "kTrIsolatedText"
#define kTrIsolatedElement "kTrIsolatedElement"
#define kTrTableChartRatio "kTrTableChartRatio"
#define kTrTableAlignmentH "kTrTableAlignmentH"
#define kTrTableAlignmentV "kTrTableAlignmentV"
#define kTrSameFontSize "kTrSameFontSize"
#define kTrSameTextVertPos "kTrSameTextVertPos"
#define kTrLineSpacingTextSplit "kTrLineSpacingTextSplit"
#define kTrLineSpacing "kTrLineSpacing"
#define kMinDropCapSize "kMinDropCapSize"
#define kMinSpaceWidthRatio "kMinSpaceWidthRatio"
#define kTrSplitterRatio "kTrSplitterRatio"
#define kTrBaselineTextRatio "kTrBaselineTextRatio"
#define kTrBaselineTableRation "kTrBaselineTableRation"
#define kTrBaselineBulletRation "kTrBaselineBulletRation"
#define kTrPathObjectMax "kTrPathObjectMax"
#define kTrPathObjectMin "kTrPathObjectMin"
#define kTemplatePagemap "pagemap"
#define kTemplateOuery "query"
#define kTemplateObjectType "object-type"
#define kTemplateMinFontSize "min-font-size"
#define kTemplateMaxFontSize "max-font-size"
#define kTemplateFontName "font-name"
#define kTemplateTextColor "text-color"
#define kTemplateRegex "regex"
#define kTemplateIgnore "ignore"
#define kTemplateNoJoin "no-join"
#define kTemplateNoSplit "no-split"
#define kTemplateStartPage "start-page"
#define kTemplateEndPage "end-page"
#define kTemplateTagArtifact "tag-artifact"
#define kTemplateHeading "heading"
#define kTemplateHeadingStyle "style"
#define kTemplateTagHeader "tag-header"
#define kTemplateFirstInstanceOnly "first-instance-only"
#define kTemplateTagAlternate "tag-alternate"
#define kTemplateAltText "alt-text"
#define kTemplateTagAttribute "tag-attribute"
#define kTemplateList "list"
#define kTemplateTable "table"
#define kTemplateTableLayout "table_layout"
#define kTemplateCellLayout "cell_layout"
#define kTemplateRectLayout "rect_layout"
#define kTemplateTextLine "text-line"
#define kTemplateReflow "reflow"
#define kTemplateCreateTextBlock "create-text-block"
#define kTemplateTextWord "text-word"
#define kTemplateNumColls "num-colls"
#define kTemplateSplitterSizeRatio "splitter-size-ratio"
#define kTemplateSplitterDeflateRatio "splitter-deflate-ratio"
#define kTemplateSplitterMaxDeflate "splitter-max-deflate"
#define _in_
#define _out_
#define _callback_

struct PdsObject;
struct PdsBoolean;
struct PdsNumber;
struct PdsString;
struct PdsName;
struct PdsArray;
struct PdsDictionary;
struct PdsStream;
struct PdsNull;
struct PdsReference;
struct PdeElement;
struct PdeContainer;
struct PdeList;
struct PdeToc;
struct PdeTag;
struct PdeAnnot;
struct PdeFormField;
struct PdeImage;
struct PdeLine;
struct PdeRect;
struct PdeHeader;
struct PdeFooter;
struct PdeCell;
struct PdeTable;
struct PdeWord;
struct PdeTextLine;
struct PdeText;
struct PdfAction;
struct PdfAnnot;
struct PdfLinkAnnot;
struct PdfMarkupAnnot;
struct PdfTextAnnot;
struct PdfTextMarkupAnnot;
struct PdfWidgetAnnot;
struct PdfBaseDigSig;
struct PdfDigSig;
struct PdfCertDigSig;
struct PdfCustomDigSig;
struct PdsStructTree;
struct PdfDoc;
struct PdfDocTemplate;
struct PdfAlternate;
struct PdfHtmlAlternate;
struct PdfFont;
struct PdfFormField;
struct PsImage;
struct PdfPage;
struct PdePageMap;
struct PdfPageView;
struct PdfBookmark;
struct PsRegex;
struct PsStream;
struct PsFileStream;
struct PsMemoryStream;
struct PsProcStream;
struct PdfStructElement;
struct PsMetadata;
struct Pdfix;
struct PdfixPlugin;

typedef int PdfErrorType;
typedef int PdfAnnotFlags;
typedef int PdfRemoveAnnotFlags;
typedef int PdfTextStateFlag;
typedef int PdfFieldFlags;
typedef int PdfRenderFlags;
typedef int PdfFontFlags;
typedef int PdfWordFlags;
typedef int PdfTextLineFlags;
typedef void* PsPlatformFile;
typedef void* PsFolderIterator;
typedef void* PsStreamData;
typedef PdsObject* PdfObjectP;
typedef PdsBoolean* PdfBooleanP;
typedef PdsNumber* PdfNumberP;
typedef PdsString* PdfStringP;
typedef PdsName* PdfNameP;
typedef PdsArray* PdfArrayP;
typedef PdsDictionary* PdfDictionaryP;
typedef PdsStream* PdfStreamP;
typedef PdsNull* PdfNullP;
typedef PdsReference* PdfReferenceP;
typedef PdeElement* PdeElementP;
typedef PdeContainer* PdeContainerP;
typedef PdeList* PdeListP;
typedef PdeToc* PdeTocP;
typedef PdeTag* PdeTagP;
typedef PdeAnnot* PdeAnnotP;
typedef PdeFormField* PdeFormFieldP;
typedef PdeImage* PdeImageP;
typedef PdeLine* PdeLineP;
typedef PdeRect* PdeRectP;
typedef PdeHeader* PdeHeaderP;
typedef PdeFooter* PdeFooterP;
typedef PdeCell* PdeCellP;
typedef PdeTable* PdeTableP;
typedef PdeWord* PdeWordP;
typedef PdeTextLine* PdeTextLineP;
typedef PdeText* PdeTextP;
typedef PdfAction* PdfActionP;
typedef PdfAnnot* PdfAnnotP;
typedef PdfLinkAnnot* PdfLinkAnnotP;
typedef PdfMarkupAnnot* PdfMarkupAnnotP;
typedef PdfTextAnnot* PdfTextAnnotP;
typedef PdfTextMarkupAnnot* PdfTextMarkupAnnotP;
typedef PdfWidgetAnnot* PdfWidgetAnnotP;
typedef PdfDigSig* PdfDigSigP;
typedef PdfDigSig* PdfDigSigP;
typedef PdfCertDigSig* PdfCertDigSigP;
typedef PdfCustomDigSig* PdfCustomDigSigP;
typedef PdfDoc* PdfDocP;
typedef PdfFont* PdfFontP;
typedef PdfFormField* PdfFormFieldP;
typedef PsImage* PsImageP;
typedef PdfPage* PdfPageP;
typedef PdePageMap* PdePageMapP;
typedef PdfPageView* PdfPageViewP;
typedef PdfBookmark* PdfBookmarkP;
typedef PsRegex* PsRegexP;
typedef PsStream* PsStreamP;
typedef PsFileStream* PsFileStreamP;
typedef PsMemoryStream* PsMemoryStreamP;
typedef PsProcStream* PsProcStreamP;
typedef PdfStructElement* PdsStructElementP;
typedef Pdfix* PdfixP;
typedef PdfixPlugin* PdfixPluginP;

typedef enum {
  kAuthPlatformWin = 0,
  kAuthPlatformMac = 1,
  kAuthPlatformLinux = 2,
  kAuthPlatformAndroid = 3,
  kAuthPlatformiOS = 4,
  kAuthPlatformServer = 5,
} PdfAuthPlatform;

typedef enum {
  kAuthOptionBasic = 0,
  kAuthOptionProfessional = 1,
  kAuthOptionEnterprise = 2,
} PdfAuthOption;

enum {
  kErrorUnknown = 1,
  kErrorOutOfMemory = 2,
  kErrorMalformedInput = 3,
  kErrorMethodNotImplemented = 4,
  kErrorPdfDocInvalid = 5,
  kErrorPdfDocOpen = 6,
  kErrorPdfDocCreate = 7,
  kErrorPdfDocSave = 8,
  kErrorPdfDocXFA = 9,
  kErrorPdfDocInterForm = 10,
  kErrorPdfDocClose = 11,
  kErrorPdfDocInfo = 12,
  kErrorPdfDocStructTreeMissing = 13,
  kErrorPdfDigSigDestroy = 14,
  kErrorPdfDigSigOpenPfxFile = 15,
  kErrorPdfDigSigSaveFile = 16,
  kErrorPdfDigSigReadFile = 17,
  kErrorPdfDigSigCertOpenSystemStore = 18,
  kErrorPdfDigSigPFXImportCertStore = 19,
  kErrorPdfDigSigCertFindInStore = 20,
  kErrorPdfDigSigPFXImportOpenSSL = 21,
  kErrorPdfDigSigPFXParseOpenSSL = 22,
  kErrorPdfDigSigByteRange = 23,
  kErrorPdfDigSigCryptMemAlloc = 24,
  kErrorPdfDigSigCryptSignMessage = 25,
  kErrorPdfDigSigTimeStampMessage = 26,
  kErrorPdfDigSigTimeStampRequest = 27,
  kErrorPdfDigSigCryptHash = 28,
  kErrorPdfDigSigVerifyDetachedMessage = 29,
  kErrorPdfDigSigUnknownType = 30,
  kErrorPdfDigSigCallback = 31,
  kErrorPdfCosObjInvalid = 32,
  kErrorPdfFontSubstFontMissing = 33,
  kErrorPdfPageRelease = 34,
  kErrorPdfPageGetImage = 35,
  kErrorPdfPageInvalidObj = 36,
  kErrorPdfPageInvalidColorSpace = 37,
  kErrorPdfPageOutOfRange = 38,
  kErrorPdfPageMapInvalidObj = 39,
  kErrorPdfPageMapParse = 40,
  kErrorPdfPageMapRangeOutOf = 41,
  kErrorPdfPageMapAddElement = 42,
  kErrorPdfPageMapCantInsertTj = 43,
  kErrorPdfPageMapWhitespaceOutOfRange = 44,
  kErrorPdfPageViewNotFound = 45,
  kErrorPsImageOpenFile = 46,
  kErrorPsImageUnsupportedFormat = 47,
  kErrorPsImageWriteBMP = 48,
  kErrorPsImageWritePNG = 49,
  kErrorPsImageWriteJPG = 50,
  kErrorPsImageInvalidBitmap = 51,
  kErrorPsImageFormat = 52,
  kErrorPdfAnnotMalformed = 53,
  kErrorPdfAnnotInvalidType = 54,
  kErrorPdfAnnotOutOfRange = 55,
  kErrorPdeAnnotMalformed = 56,
  kErrorPdeElementChildrenOutOfRange = 57,
  kErrorPdeElementMalformed = 58,
  kErrorPdeTextRunMalformed = 59,
  kErrorPdeWordMalformed = 60,
  kErrorPdeLineMalformed = 61,
  kErrorPdeListMalformed = 62,
  kErrorPdeTextMalformed = 63,
  kErrorPdeTextRangeOutOf = 64,
  kErrorPdeTextSelectRange = 65,
  kErrorPdeTableMalformed = 66,
  kErrorPdeTableCellRangeOutOf = 67,
  kErrorPdeCellRangeOutOf = 68,
  kErrorPsRegexDestroy = 69,
  kErrorPsRegexPatternMissing = 70,
  kErrorPsRegexPositionOutOfRange = 71,
  kErrorPsEventMalformed = 72,
  kErrorPsEventExists = 73,
  kErrorPsNoEvent = 74,
  kErrorPdfBookmarkMalformed = 75,
  kErrorPdfBookmarkRoot = 76,
  kErrorPdfBookmarkChildrenOutOfRange = 77,
  kErrorPsAuthorizationFailed = 78,
  kErrorPsAuthorizationNeeded = 79,
  kErrorPsAuthorizationCalled = 80,
  kErrorPsAuthorizationEmail = 81,
  kErrorPsAuthorizationWin = 82,
  kErrorPsAuthorizationMac = 83,
  kErrorPsAuthorizationAndroid = 84,
  kErrorPsAuthorizationiOS = 85,
  kErrorPsAuthorizationLinux = 86,
  kErrorPsAuthorizationServer = 87,
  kErrorPsAuthorizationFeature = 88,
  kErrorPsAuthorizationDate = 89,
  kErrorPsAuthorizationVersion = 90,
  kErrorPsAuthorizationNumber = 91,
  kErrorPsAuthorizationOsCheck = 92,
  kErrorPdfFontNotEmbedded = 93,
  kErrorPdfFontSave = 94,
  kErrorPathNotFound = 95,
  kErrorPdfPageMapAddTags = 96,
  kErrorPdfPageMapRemoveTags = 97,
  kErrorPdfAlternateNotFound = 98,
  kErrorPdfAlternateInvalid = 99,
  kErrorPdfAlternateResourceNotFound = 100,
  kErrorPdfHtmlAlternateFont = 101,
  kErrorPdfHtmlAlternateCreateAF = 102,
  kErrorPdfHtmlAlternateWriteAF = 103,
  kErrorPdfHtmlAlternateImage = 104,
  kErrorPsStreamReadProcMissing = 105,
  kErrorPsStreamWriteProcMissing = 106,
  kErrorPsStreamGetSizeProcMissing = 107,
  kErrorPdfPageMapTagAttributes = 108,
  kErrorPdfPageMapTagParentTree = 109,
  kErrorPdeContentWriter = 110,
  kErrorParsingDataFile = 111,
  kErrorPsRegexSearchFail = 112,
  kErrorDocTemplateInvalidQuery = 113,
  kErrorDocTemplateInvalidValue = 114,
  kErrorPdsStructTreeInvalid = 115,
  kErrorPsRegexIndexOutOfBounds = 116,
  kErrorInit = 117,
} ;

typedef enum {
  kEventUnknown = 0,
  kEventDocWillSave = 1,
  kEventDocWillClose = 2,
  kEventDocDidOpen = 3,
  kEventDocDidSave = 4,
  kEventAnnotWillChange = 5,
  kEventAnnotDidChange = 6,
  kEventPageWillAddAnnot = 7,
  kEventPageWillRemoveAnnot = 8,
  kEventPageDidAddAnnot = 9,
  kEventPageDidRemoveAnnot = 10,
  kEventageContentsDidChange = 11,
} PdfEventType;

typedef enum {
  kSaveIncremental = 0,
  kSaveFull = 1,
} PdfSaveFlags;

typedef enum {
  kDigSigBlank = 0,
  kDigSigUnknown = 1,
  kDigSigInvalid = 2,
  kDigSigValid = 3,
  kDigSigDoubleChecked = 4,
  kDigSigValidStateEnumSize = 5,
} PdfDigSigValidState;

typedef enum {
  kAlignmentNone = 0,
  kAlignmentLeft = 1,
  kAlignmentRight = 2,
  kAlignmentJustify = 3,
  kAlignmentTop = 4,
  kAlignmentBottom = 5,
  kAlignmentCenter = 6,
} PdfAlignment;

typedef enum {
  kRotate0 = 0,
  kRotate90 = 90,
  kRotate180 = 180,
  kRotate270 = 270,
} PdfRotate;

typedef enum {
  kPdsUnknown = 0,
  kPdsBoolean = 1,
  kPdsNumber = 2,
  kPdsString = 3,
  kPdsName = 4,
  kPdsArray = 5,
  kPdsDictionary = 6,
  kPdsStream = 7,
  kPdsNull = 8,
  kPdsReference = 9,
} PdfObjectType;

typedef enum {
  kPdeUnknown = 0,
  kPdeText = 1,
  kPdeTextLine = 2,
  kPdeWord = 3,
  kPdeTextRun = 4,
  kPdeImage = 5,
  kPdeContainer = 6,
  kPdeList = 7,
  kPdeLine = 8,
  kPdeRect = 9,
  kPdeTable = 10,
  kPdeCell = 11,
  kPdeToc = 12,
  kPdeFormField = 13,
  kPdeHeader = 14,
  kPdeFooter = 15,
  kPdeTag = 16,
  kPdeColumn = 17,
  kPdeRow = 18,
} PdfElementType;

typedef enum {
  kPdfLineCapButt = 0,
  kPdfLineCapRound = 1,
  kPdfLineCapSquare = 2,
} PdfLineCap;

typedef enum {
  kPdfLineJoinMiter = 0,
  kPdfLineJoinRound = 1,
  kPdfLineJoinBevel = 2,
} PdfLineJoin;

typedef enum {
  kFillTypeNone = 0,
  kFillTypeSolid = 1,
  kFillTypePattern = 2,
} PdfFillType;

typedef enum {
  kTextAlignmentNone = 0,
  kTextAlignmentLeft = 1,
  kTextAlignmentRight = 2,
  kTextAlignmentCenter = 3,
  kTextAlignmentJustify = 4,
} PdfTextAlignment;

typedef enum {
  kAnnotUnknown = 0,
  kAnnotText = 1,
  kAnnotLink = 2,
  kAnnotFreeText = 3,
  kAnnotLine = 4,
  kAnnotSquare = 5,
  kAnnotCircle = 6,
  kAnnotPolygon = 7,
  kAnnotPolyLine = 8,
  kAnnotHighlight = 9,
  kAnnotUnderline = 10,
  kAnnotSquiggly = 11,
  kAnnotStrikeOut = 12,
  kAnnotStamp = 13,
  kAnnotCaret = 14,
  kAnnotInk = 15,
  kAnnotPopup = 16,
  kAnnotFileAttachment = 17,
  kAnnotSound = 18,
  kAnnotMovie = 19,
  kAnnotWidget = 20,
  kAnnotScreen = 21,
  kAnnotPrinterMark = 22,
  kAnnotTrapNet = 23,
  kAnnotWatermark = 24,
  kAnnot3D = 25,
  kAnnotRedact = 26,
} PdfAnnotSubtype;

enum {
  kAnnotFlagNone = 0x0000,
  kAnnotFlagInvisible = 0x0001,
  kAnnotFlagHidden = 0x0002,
  kAnnotFlagPrint = 0x0004,
  kAnnotFlagNoZoom = 0x0008,
  kAnnotFlagNoRotate = 0x0010,
  kAnnotFlagNoView = 0x0020,
  kAnnotFlagReadOnly = 0x0040,
  kAnnotFlagLocked = 0x0080,
  kAnnotFlagToggleNoView = 0x0100,
  kAnnotFlagLockedContents = 0x0200,
} ;

enum {
  kRemoveAnnotSingle = 0x0000,
  kRemoveAnnotPopup = 0x0001,
  kRemoveAnnotReply = 0x0002,
} ;

typedef enum {
  kBorderSolid = 0,
  kBorderDashed = 1,
  kBorderBeveled = 2,
  kBorderInset = 3,
  kBorderUnderline = 4,
} PdfBorderStyle;

enum {
  kTextFlagNone = 0x000,
  kTextFlagUnderline = 0x001,
  kTextFlagStrikeout = 0x002,
  kTextFlagHighlight = 0x004,
  kTextFlagSubscript = 0x008,
  kTextFlagSuperscript = 0x010,
  kTextFlagNoUnicode = 0x020,
  kTextFlagPatternFill = 0x040,
  kTextFlagPatternStroke = 0x080,
  kTextFlagAngle = 0x100,
  kTextFlagWhiteSpace = 0x200,
} ;

enum {
  kFieldFlagNone = 0x00000000,
  kFieldFlagReadOnly = 0x00000001,
  kFieldFlagRequired = 0x00000002,
  kFieldFlagNoExport = 0x00000004,
  kFieldFlagMultiline = 0x00001000,
  kFieldFlagPassword = 0x00002000,
  kFieldFlagNoToggleToOff = 0x00004000,
  kFieldFlagRadio = 0x00008000,
  kFieldFlagPushButton = 0x00010000,
  kFieldFlagCombo = 0x00200000,
  kFieldFlagEdit = 0x00400000,
  kFieldFlagSort = 0x00800000,
  kFieldFlagMultiSelect = 0x00200000,
  kFieldFlagDoNotSpellCheck = 0x00400000,
  kFieldFlagDCommitOnSelChange = 0x04000000,
  kFieldFlagFileSelect = 0x00100000,
  kFieldFlagDoNotScroll = 0x00800000,
  kFieldFlagComb = 0x01000000,
  kFieldFlagRichText = 0x02000000,
  kFieldFlagRadiosInUnison = 0x02000000,
} ;

typedef enum {
  kFieldUnknown = 0,
  kFieldButton = 1,
  kFieldRadio = 2,
  kFieldCheck = 3,
  kFieldText = 4,
  kFieldCombo = 5,
  kFieldList = 6,
  kFieldSignature = 7,
} PdfFieldType;

typedef enum {
  kActionEventAnnotEnter = 0,
  kActionEventAnnotExit = 1,
  kActionEventAnnotMouseDown = 2,
  kActionEventAnnotMouseUp = 3,
  kActionEventAnnotFocus = 4,
  kActionEventAnnotBlur = 5,
  kActionEventAnnotPageOpen = 6,
  kActionEventAnnotPageClose = 7,
  kActionEventAnnotPageVisible = 8,
  kActionEventAnnotPageInvisible = 9,
  kActionEventPageOpen = 10,
  kActionEventPageClose = 11,
  kActionEventFieldKeystroke = 12,
  kActionEventFieldFormat = 13,
  kActionEventFieldValidate = 14,
  kActionEventFieldCalculate = 15,
  kActionEventDocWillClose = 16,
  kActionEventDocWillSave = 17,
  kActionEventDocDidSave = 18,
  kActionEventDocWillPrint = 19,
  kActionEventDocDidPrint = 20,
} PdfActionEventType;

typedef enum {
  kActionUnknown = 0,
  kActionGoTo = 1,
  kActionGoToR = 2,
  kActionGoToE = 3,
  kActionLaunch = 4,
  kActionThread = 5,
  kActionURI = 6,
  kActionSound = 7,
  kActionMovie = 8,
  kActionHide = 9,
  kActionNamed = 10,
  kActionSubmitForm = 11,
  kActionResetForm = 12,
  kActionImportData = 13,
  kActionJavaScript = 14,
  kActionSetOCGState = 15,
  kActionRendition = 16,
  kActionTrans = 17,
  kActionGoTo3DView = 18,
} PdfActionType;

enum {
  kRenderAnnot = 0x001,
  kRenderLCDText = 0x002,
  kRenderNoNativeText = 0x004,
  kRenderGrayscale = 0x008,
  kRenderLimitedCache = 0x010,
  kRenderForceHalftone = 0x020,
  kRenderPrinting = 0x040,
  kRenderNoText = 0x080,
  kRenderNoBackground = 0x100,
} ;

typedef enum {
  kRenderElemNone = 0,
  kRenderElem = 1,
} PdfRenderMode;

typedef enum {
  kImageFormatPng = 0,
  kImageFormatJpg = 1,
  kImageFormatBmp = 2,
  kImageFormatEmf = 3,
} PdfImageFormat;

enum {
  kFontFixedPitch = 0x00001,
  kFontSerif = 0x00002,
  kFontSymbolic = 0x00004,
  kFontScript = 0x00008,
  kFontNotSymbolic = 0x00020,
  kFontItalic = 0x00040,
  kFontAllCap = 0x10000,
  kFontSmallCap = 0x20000,
  kFontForceBold = 0x40000,
} ;

typedef enum {
  kFontAnsiCharset = 0,
  kFontDefaultCharset = 1,
  kFontSymbolCharset = 2,
  kFontUnknownCharset = 3,
  kFontMacintoshCharset = 77,
  kFontShiftJISCharset = 128,
  kFontHangeulCharset = 129,
  kFontKoreanCharset = 130,
  kFontGB2312Charset = 134,
  kFontCHineseBig5Charset = 136,
  kFontGreekCharset = 161,
  kFontTurkishCharset = 162,
  kFontVietnameseCharset = 163,
  kFontHebrewCharset = 177,
  kFontArabicCharset = 178,
  kFontArabicTCharset = 179,
  kFontArabicUCharset = 180,
  kFontHebrewUCharset = 181,
  kFontBalticCharset = 186,
  kFontRussianCharset = 204,
  kFontThaiCharset = 222,
  kFontEastEuropeCharset = 238,
} PdfFontCharset;

typedef enum {
  kAllPages = 0,
  kEvenPagesOnly = 1,
  kOddPagesOnly = 2,
} PdfPageRangeType;

typedef enum {
  kFontUnknownType = 0,
  kFontType1 = 1,
  kFontTrueType = 2,
  kFontType3 = 3,
  kFontCIDFont = 4,
} PdfFontType;

typedef enum {
  kFontFormatTtf = 0,
  kFontFormatWoff = 1,
} PdfFontFormat;

typedef enum {
  kPdfZoomXYZ = 1,
  kPdfZoomFitPage = 2,
  kPdfZoomFitHorz = 3,
  kPdfZoomFitVert = 4,
  kPdfZoomFitRect = 5,
  kPdfZoomFitBbox = 6,
  kPdfZoomFitBHorz = 7,
  kPdfZoomFitBVert = 8,
} PdfDestZoomType;

typedef enum {
  kDigSigOpenSSL = 0,
  kDigSigCert = 1,
  kDigSigCustom = 2,
} PdfDigSigType;

typedef enum {
  kImageFigure = 0,
  kImageImage = 1,
  kImagePath = 2,
  kImageRect = 3,
  kImageShading = 4,
  kImageTable = 5,
} PdfImageType;

typedef enum {
  kListUnordered = 0,
  kListOrdered = 1,
} PdfListType;

enum {
  kWordHyphen = 0x0001,
  kWordBullet = 0x0002,
  kWordFilling = 0x0008,
  kWordNumber = 0x0010,
  kWordImage = 0x10000,
} ;

enum {
  kTextLineNewLine = 0x0001,
  kTextLineBullet = 0x0002,
  kTextLineHyphen = 0x0004,
  kTextLineIndent = 0x0008,
  kTextLineDropCap = 0x0010,
} ;

typedef enum {
  kTextNormal = 0,
  kTextH1 = 1,
  kTextH2 = 2,
  kTextH3 = 3,
  kTextH4 = 4,
  kTextH5 = 5,
  kTextH6 = 6,
  kTextH7 = 7,
  kTextH8 = 8,
  kTextNote = 9,
} PdfTextStyle;

typedef enum {
  kRegexHyphen = 0,
  kRegexBullet = 1,
  kRegexBulletFont = 2,
  kRegexBulletLine = 3,
  kRegexFilling = 4,
  kRegexToc = 5,
  kRegexNumber = 6,
  kRegexAllCaps = 7,
  kRegexFirstCap = 8,
  kRegexCurrency = 9,
  kRegexPercent = 10,
  kRegexTerminal = 11,
  kRegexTableCaption = 12,
  kRegexImageCaption = 13,
  kRegexChartCaption = 14,
  kRegexNoteCaption = 15,
  kRegexNumberedList = 16,
  kRegexNumberedSplit = 17,
  kRegexSentences = 18,
  kRegexAlphaNum = 19,
  kRegexColon = 20,
  kRegexPhoneNumber = 21,
  kRegexDate = 22,
  kRegexPageNumber = 23,
  kRegexLast = 24,
} PdfRegexType;

typedef enum {
  kPsWrite = 0,
  kPsReadOnly = 1,
  kPsTruncate = 2,
} PsFileMode;

typedef enum {
  kAlternatePdf = 0,
  kAlternateHtml = 1,
} PdfAlternateType;

typedef enum {
  kCSSMediaTypeAll = 0,
  kCSSMediaTypePrint = 1,
  kCSSMediaTypeScreen = 2,
  kCSSMediaTypeSpeech = 3,
} PdfMediaType;

typedef enum {
  kImageDIBFormatArgb = 0x220,
} PsImageDIBFormat;

typedef enum {
  kDataFormatJson = 0,
  kDataFormatXml = 1,
} PsDataFormat;

typedef enum {
  kFileStream = 0,
  kMemoryStream = 1,
  kProcStream = 2,
} PdfStreamType;


typedef struct _PdfEventParams {
  PdfEventType type;
  PdfDoc* doc;
  PdfPage* page;
  PdfAnnot* annot;
  _PdfEventParams() {
    type = kEventUnknown;
    doc = nullptr;
    page = nullptr;
    annot = nullptr;
  }
} PdfEventParams;

typedef struct _PdfPageRangeParams {
  int start_page;
  int end_page;
  PdfPageRangeType page_range_spec;
  _PdfPageRangeParams() {
    start_page = 0;
    end_page = -1;
    page_range_spec = kAllPages;
  }
} PdfPageRangeParams;

typedef struct _PdfWatermarkParams {
  PdfPageRangeParams page_range;
  int order_top;
  PdfAlignment h_align;
  PdfAlignment v_align;
  int percentage_vals;
  double h_value;
  double v_value;
  double scale;
  double rotation;
  double opacity;
  _PdfWatermarkParams() {
    order_top = 1;
    percentage_vals = 0;
    h_align = kAlignmentLeft;
    v_align = kAlignmentTop;
    h_value = 0;
    v_value = 0;
    scale = 1;
    rotation = 0;
    opacity = 1;
  }
} PdfWatermarkParams;

typedef struct _PdfPoint {
  double x;
  double y;
} PdfPoint;

typedef struct _PdfDevPoint {
  int x;
  int y;
} PdfDevPoint;

typedef struct _PdfRect {
  double left;
  double top;
  double right;
  double bottom;
  _PdfRect() {
    left = 0.;
    top = 0.;
    right = 0.;
    bottom = 0.;
  }
} PdfRect;

typedef struct _PdfDevRect {
  int left;
  int top;
  int right;
  int bottom;
  _PdfDevRect() {
    left = 0;
    top = 0;
    right = 0;
    bottom = 0;
  }
} PdfDevRect;

typedef struct _PdfQuad {
  PdfPoint tl;
  PdfPoint tr;
  PdfPoint bl;
  PdfPoint br;
} PdfQuad;

typedef struct _PdfDevQuad {
  PdfDevPoint tl;
  PdfDevPoint tr;
  PdfDevPoint bl;
  PdfDevPoint br;
} PdfDevQuad;

typedef struct _PdfMatrix {
  double a;
  double b;
  double c;
  double d;
  double e;
  double f;
  _PdfMatrix() {
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    e = 0;
    f = 0;
  }
} PdfMatrix;

typedef struct _PdfRGB {
  int r;
  int g;
  int b;
  _PdfRGB() {
    r = 0;
    g = 0;
    b = 0;
  }
} PdfRGB;

typedef struct _PdfColorState {
  PdfFillType fill_type;
  PdfFillType stroke_type;
  PdfRGB fill_color;
  PdfRGB stroke_color;
  int fill_opacity;
  int stroke_opacity;
  _PdfColorState() {
    fill_type = kFillTypeNone;
    stroke_type = kFillTypeNone;
    fill_opacity = 255;
    stroke_opacity = 255;
  }
} PdfColorState;

typedef struct _PdfTextState {
  PdfColorState color_state;
  PdfFont* font;
  double font_size;
  double char_spacing;
  double word_spacing;
  PdfTextStateFlag flags;
  _PdfTextState() {
    font = nullptr;
    font_size = 0;
    char_spacing = 0;
    word_spacing = 0;
    flags = 0;
  }
} PdfTextState;

typedef struct _PdfGraphicState {
  PdfColorState color_state;
  double line_width;
  double miter_limit;
  PdfLineCap line_cap;
  PdfLineJoin line_join;
  _PdfGraphicState() {
    line_width = 1;
    miter_limit = 0;
    line_cap = kPdfLineCapButt;
    line_join = kPdfLineJoinMiter;
  }
} PdfGraphicState;

typedef struct _PdfFontState {
  PdfFontType type;
  PdfFontFlags flags;
  PdfRect bbox;
  int ascent;
  int descent;
  int italic;
  int bold;
  int fixed_width;
  int vertical;
  int embedded;
  int height;
  _PdfFontState() {
    type = kFontUnknownType;
    flags = 0;
    ascent = 0;
    descent = 0;
    italic = 0;
    bold = 0;
    fixed_width = 0;
    vertical = 0;
    embedded = 0;
    height = 0;
  }
} PdfFontState;

typedef struct _PdfPageRenderParams {
  void* device;
  PsImage* image;
  PdfMatrix matrix;
  PdfDevRect clip_rect;
  PdfRenderFlags render_flags;
  _PdfPageRenderParams() {
    device = 0;
    image = nullptr;
    render_flags = kRenderAnnot;
  }
} PdfPageRenderParams;

typedef struct _PdfAnnotAppearance {
  PdfRGB fill_color;
  PdfFillType fill_type;
  PdfRGB border_color;
  double border_width;
  PdfBorderStyle border;
  double opacity;
  double font_size;
  PdfTextAlignment text_align;
  _PdfAnnotAppearance() {
    fill_type = kFillTypeNone;
    border_width = 1;
    border = kBorderSolid;
    opacity = 1;
    font_size = 0;
    text_align = kTextAlignmentLeft;
  }
} PdfAnnotAppearance;

typedef struct _PdfBookmarkAppearance {
  PdfRGB color;
  int italic;
  int bold;
  _PdfBookmarkAppearance() {
    italic = 0;
    bold = 0;
  }
} PdfBookmarkAppearance;

typedef struct _PdfWhitespaceParams {
  double width;
  double height;
  _PdfWhitespaceParams() {
    width = 0;
    height = 0;
  }
} PdfWhitespaceParams;

typedef struct _PdfFlattenAnnotsParams {
  PdfPageRangeParams page_range;
  int flags;
  _PdfFlattenAnnotsParams() {
    flags = 0;
  }
} PdfFlattenAnnotsParams;

typedef struct _PdfMediaQueryParams {
  PdfMediaType type;
  int min_width;
  _PdfMediaQueryParams() {
    type = kCSSMediaTypeAll;
    min_width = 1200;
  }
} PdfMediaQueryParams;

typedef struct _PdfImageParams {
  PdfImageFormat format;
  int quality;
  _PdfImageParams() {
    format = kImageFormatPng;
    quality = 100;
  }
} PdfImageParams;

typedef struct _PdfAccessibleParams {
  int accept_tags;
  int embed_fonts;
  int subset_fonts;
  _PdfAccessibleParams() {
    accept_tags = 0;
    embed_fonts = 0;
    subset_fonts = 0;
  }
} PdfAccessibleParams;

typedef int (*PdfCancelProc) (void* data);
typedef void (*PdfEventProc) (PdfEventParams* event, void* data);
typedef unsigned long (*PdfDigestDataProc) (int buffer_count, const unsigned char* buffer_to_sign[], unsigned long buffer_size[], unsigned char* sign_buff, unsigned long sign_buff_size, void* data);
typedef int (*PsStreamProc)(char* buffer, int offset, int size, PsStreamData data);
typedef void (*PsStreamDestroyProc)(PsStreamData data);
typedef int (*PsStreamGetSizeProc)(PsStreamData data);

struct PdsObject {
  virtual PdfObjectType GetType() = 0;
};

struct PdsBoolean : PdsObject {
  virtual bool GetValue() = 0;
  virtual bool SetValue(bool value) = 0;
};

struct PdsNumber : PdsObject {
  virtual int GetIntegerValue() = 0;
  virtual double GetNumberValue() = 0;
  virtual bool SetIntegerValue(int value) = 0;
  virtual bool SetNumberValue(double value) = 0;
};

struct PdsString : PdsObject {
  virtual int GetValue(_out_ wchar_t* buffer, int len) = 0;
  virtual bool SetValue(const wchar_t* buffer) = 0;
};

struct PdsName : PdsObject {
};

struct PdsArray : PdsObject {
};

struct PdsDictionary : PdsObject {
  virtual bool KnownObject(const char* key) = 0;
  virtual PdsObject* GetObject(const char* key) = 0;
  virtual bool SetObject(const char* key, PdsObject* value) = 0;
};

struct PdsStream : PdsObject {
};

struct PdsNull : PdsObject {
};

struct PdsReference : PdsObject {
};

struct PdeElement {
  virtual PdfElementType GetType() = 0;
  virtual void GetBBox(_out_ PdfRect* bbox) = 0;
  virtual int GetId() = 0;
  virtual void GetGraphicState(_out_ PdfGraphicState* g_state) = 0;
  virtual int GetNumChildren() = 0;
  virtual PdeElement* GetChild(int index) = 0;
  virtual PdfAlignment GetAlignment() = 0;
  virtual double GetAngle() = 0;
  virtual void SetRenderMode(PdfRenderMode mode) = 0;
};

struct PdeContainer : PdeElement {
};

struct PdeList : PdeElement {
};

struct PdeToc : PdeElement {
};

struct PdeTag : PdeElement {
  virtual PdfStructElement* GetStructElement() = 0;
};

struct PdeAnnot : PdeElement {
  virtual PdfAnnot* GetAnnot() = 0;
};

struct PdeFormField : PdeAnnot {
};

struct PdeImage : PdeContainer {
  virtual PdfImageType GetImageType() = 0;
  virtual PdeElement* GetCaption() = 0;
};

struct PdeLine : PdeElement {
};

struct PdeRect : PdeContainer {
};

struct PdeHeader : PdeContainer {
};

struct PdeFooter : PdeContainer {
};

struct PdeCell : PdeContainer {
  virtual int GetRowSpan() = 0;
  virtual int GetColSpan() = 0;
  virtual bool HasBorderGraphicState(int index) = 0;
  virtual PdeCell* GetSpanCell() = 0;
};

struct PdeTable : PdeContainer {
  virtual int GetNumRows() = 0;
  virtual int GetNumCols() = 0;
  virtual PdeCell* GetCell(int row, int col) = 0;
  virtual PdfAlignment GetRowAlignment(int row) = 0;
  virtual PdfAlignment GetColAlignment(int col) = 0;
  virtual PdeElement* GetCaption() = 0;
};

struct PdeWord : PdeElement {
  virtual int GetText(_out_ wchar_t* buffer, int len) = 0;
  virtual bool HasTextState() = 0;
  virtual void GetTextState(_out_ PdfTextState* text_state) = 0;
  virtual int GetNumChars() = 0;
  virtual int GetCharText(int index, _out_ wchar_t* buffer, int len) = 0;
  virtual void GetCharTextState(int index, _out_ PdfTextState* text_state) = 0;
  virtual void GetCharBBox(int index, _out_ PdfRect* bbox) = 0;
  virtual int GetFlags() = 0;
  virtual PdeElement* GetBackground() = 0;
  virtual void GetOrigin(_out_ PdfPoint* point) = 0;
};

struct PdeTextLine : PdeElement {
  virtual int GetText(_out_ wchar_t* buffer, int len) = 0;
  virtual bool HasTextState() = 0;
  virtual void GetTextState(_out_ PdfTextState* text_state) = 0;
  virtual int GetNumWords() = 0;
  virtual PdeWord* GetWord(int index) = 0;
  virtual int GetFlags() = 0;
};

struct PdeText : PdeElement {
  virtual int GetText(_out_ wchar_t* buffer, int len) = 0;
  virtual bool HasTextState() = 0;
  virtual void GetTextState(_out_ PdfTextState* text_state) = 0;
  virtual int GetNumTextLines() = 0;
  virtual PdeTextLine* GetTextLine(int index) = 0;
  virtual int GetNumWords() = 0;
  virtual PdeWord* GetWord(int index) = 0;
  virtual double GetLineSpacing() = 0;
  virtual double GetIndent() = 0;
  virtual PdfTextStyle GetTextStyle() = 0;
};

struct PdfAction {
  virtual PdfActionType GetSubtype() = 0;
  virtual int GetJavaScript(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetURI(_out_ wchar_t* buffer, int len) = 0;
};

struct PdfAnnot {
  virtual PdfAnnotSubtype GetSubtype() = 0;
  virtual PdfAnnotFlags GetFlags() = 0;
  virtual void GetAppearance(_out_ PdfAnnotAppearance* appearance) = 0;
  virtual void GetBBox(_out_ PdfRect* bbox) = 0;
  virtual bool PointInAnnot(PdfPoint* point) = 0;
  virtual bool RectInAnnot(PdfRect* rect) = 0;
};

struct PdfLinkAnnot : PdfAnnot {
  virtual int GetNumQuads() = 0;
  virtual void GetQuad(int index, _out_ PdfQuad* quad) = 0;
  virtual bool AddQuad(PdfQuad* quad) = 0;
  virtual bool RemoveQuad(int index) = 0;
  virtual PdfAction* GetAction() = 0;
};

struct PdfMarkupAnnot : PdfAnnot {
  virtual int GetContents(_out_ wchar_t* buffer, int len) = 0;
  virtual bool SetContents(const wchar_t* buffer) = 0;
  virtual int GetAuthor(_out_ wchar_t* buffer, int len) = 0;
  virtual bool SetAuthor(const wchar_t* buffer) = 0;
  virtual int GetNumReplies() = 0;
  virtual PdfAnnot* GetReply(int index) = 0;
  virtual PdfAnnot* AddReply(const wchar_t* author, const wchar_t* text) = 0;
};

struct PdfTextAnnot : PdfMarkupAnnot {
};

struct PdfTextMarkupAnnot : PdfMarkupAnnot {
  virtual int GetNumQuads() = 0;
  virtual void GetQuad(int index, _out_ PdfQuad* quad) = 0;
  virtual bool AddQuad(PdfQuad* quad) = 0;
  virtual bool RemoveQuad(int index) = 0;
};

struct PdfWidgetAnnot : PdfAnnot {
  virtual int GetCaption(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetFontName(_out_ wchar_t* buffer, int len) = 0;
  virtual PdfAction* GetAction() = 0;
  virtual PdfAction* GetAAction(PdfActionEventType event) = 0;
  virtual PdfFormField* GetFormField() = 0;
};

struct PdfBaseDigSig {
  virtual void Destroy() = 0;
  virtual bool SetReason(const wchar_t* reason) = 0;
  virtual bool SetLocation(const wchar_t* location) = 0;
  virtual bool SetContactInfo(const wchar_t* contact) = 0;
  virtual bool SetName(const wchar_t* name) = 0;
  virtual bool SetTimeStampServer(const wchar_t* url, const wchar_t* user_name, const wchar_t* password) = 0;
  virtual bool SignDoc(PdfDoc* doc, const wchar_t* path) = 0;
};

struct PdfDigSig : PdfBaseDigSig {
  virtual bool SetPfxFile(const wchar_t* pfx_file, const wchar_t* pfx_password) = 0;
};

#if defined _WIN32 && defined _MSC_VER 
struct PdfCertDigSig : PdfBaseDigSig {
  virtual bool SetPfxFile(const wchar_t* pfx_file, const wchar_t* pfx_password) = 0;
  virtual bool SetCertContext(void* cert_context) = 0;
};
#endif

struct PdfCustomDigSig : PdfBaseDigSig {
  virtual bool RegisterDigestDataProc(_callback_ PdfDigestDataProc proc, void* data) = 0;
};

struct PdsStructTree {
};

struct PdfDoc {
  virtual bool Save(const wchar_t* path, PdfSaveFlags flags) = 0;
  virtual bool SaveToStream(PsStream* stream, PdfSaveFlags flags) = 0;
  virtual bool Close() = 0;
  virtual bool AddWatermarkFromImage(PdfWatermarkParams* params, const wchar_t* path) = 0;
  virtual int GetNumPages() = 0;
  virtual PdfPage* AcquirePage(int page_num) = 0;
  virtual bool ReleasePage(PdfPage* page) = 0;
  virtual int GetNumDocumentJavaScripts() = 0;
  virtual int GetDocumentJavaScript(int index, _out_ wchar_t* buffer, int len) = 0;
  virtual int GetDocumentJavaScriptName(int index, _out_ wchar_t* buffer, int len) = 0;
  virtual int GetNumCalculatedFormFields() = 0;
  virtual PdfFormField* GetCalculatedFormField(int index) = 0;
  virtual int GetNumFormFields() = 0;
  virtual PdfFormField* GetFormField(int index) = 0;
  virtual PdfFormField* GetFormFieldByName(const wchar_t* buffer) = 0;
  virtual int GetInfo(const wchar_t* key, _out_ wchar_t* buffer, int len) = 0;
  virtual bool SetInfo(const wchar_t* key, const wchar_t* buffer) = 0;
  virtual PdfBookmark* GetBookmarkRoot() = 0;
  virtual bool FlattenAnnots(PdfFlattenAnnotsParams* params) = 0;
  virtual void RemoveStructTree() = 0;
  virtual int GetNumAlternates() = 0;
  virtual PdfAlternate* AcquireAlternate(int index) = 0;
  virtual PdsObject* CreatePdsObject(PdfObjectType type, bool indirect) = 0;
  virtual bool AddTags(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
  virtual PdfDocTemplate* GetDocTemplate() = 0;
  virtual PsMetadata* GetMetadata() = 0;
  virtual int GetLang(_out_ wchar_t* buffer, int len) = 0;
  virtual bool SetLang(const wchar_t* buffer) = 0;
  virtual bool EmbedFonts(bool subset, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
  virtual bool MakeAccessible(PdfAccessibleParams* params, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
};

struct PdfDocTemplate {
  virtual bool PreflightDoc(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
  virtual bool LoadFromStream(PsStream* stream, PsDataFormat format) = 0;
  virtual bool SaveToStream(PsStream* stream, PsDataFormat format) = 0;
  virtual bool SetDefaults() = 0;
};

struct PdfAlternate {
  virtual PdfAlternateType GetSubtype() = 0;
  virtual int GetName(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetDescription(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetFileName(_out_ wchar_t* buffer, int len) = 0;
  virtual bool SaveContent(const wchar_t* path) = 0;
  virtual void Release() = 0;
};

struct PdfHtmlAlternate : PdfAlternate {
  virtual bool SaveResource(const wchar_t* resource_name, const wchar_t* path) = 0;
};

struct PdfFont {
  virtual int GetFontName(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetFaceName(_out_ wchar_t* buffer, int len) = 0;
  virtual void GetFontState(_out_ PdfFontState* font_state) = 0;
  virtual int GetSystemFontName(_out_ wchar_t* buffer, int len) = 0;
  virtual PdfFontCharset GetSystemFontCharset() = 0;
  virtual bool GetSystemFontBold() = 0;
  virtual bool GetSystemFontItalic() = 0;
  virtual bool SaveToStream(PsStream* stream, PdfFontFormat format) = 0;
};

struct PdfFormField {
  virtual PdfFieldType GetType() = 0;
  virtual PdfFieldFlags GetFlags() = 0;
  virtual bool SetFlags(PdfFieldFlags flags) = 0;
  virtual int GetValue(_out_ wchar_t* buffer, int len) = 0;
  virtual bool SetValue(const wchar_t* buffer) = 0;
  virtual int GetDefaultValue(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetFullName(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetTooltip(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetOptionCount() = 0;
  virtual int GetOptionValue(int index, _out_ wchar_t* buffer, int len) = 0;
  virtual int GetOptionCaption(int index, _out_ wchar_t* buffer, int len) = 0;
  virtual PdfAction* GetAction() = 0;
  virtual PdfAction* GetAAction(PdfActionEventType event) = 0;
  virtual int GetMaxLength() = 0;
  virtual int GetWidgetExportValue(PdfAnnot* annot, _out_ wchar_t* buffer, int len) = 0;
};

struct PsImage {
  virtual void Destroy() = 0;
  virtual bool Save(const wchar_t* path, PdfImageParams* params) = 0;
  virtual bool SaveRect(const wchar_t* path, PdfImageParams* params, PdfDevRect* dev_rect) = 0;
  virtual bool SaveToStream(PsStream* stream, PdfImageParams* params) = 0;
  virtual bool SaveRectToStream(PsStream* stream, PdfImageParams* params, PdfDevRect* dev_rect) = 0;
  virtual void GetPointColor(PdfDevPoint* point, _out_ PdfRGB* color) = 0;
};

struct PdfPage {
  virtual void GetCropBox(_out_ PdfRect* crop_box) = 0;
  virtual void GetMediaBox(_out_ PdfRect* media_box) = 0;
  virtual PdfRotate GetRotate() = 0;
  virtual void GetDefaultMatrix(_out_ PdfMatrix* matrix) = 0;
  virtual int GetNumber() = 0;
  virtual PdePageMap* AcquirePageMap(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
  virtual bool ReleasePageMap() = 0;
  virtual PdfPageView* AcquirePageView(double zoom, PdfRotate rotate) = 0;
  virtual bool ReleasePageView(PdfPageView* page_view) = 0;
  virtual int GetNumAnnots() = 0;
  virtual PdfAnnot* GetAnnot(int index) = 0;
  virtual bool RemoveAnnot(int index, PdfRemoveAnnotFlags flags) = 0;
  virtual PdfTextAnnot* AddTextAnnot(int index, PdfRect* rect) = 0;
  virtual PdfLinkAnnot* AddLinkAnnot(int index, PdfRect* rect) = 0;
  virtual PdfTextMarkupAnnot* AddTextMarkupAnnot(int index, PdfRect* rect, PdfAnnotSubtype subtype) = 0;
  virtual int GetNumAnnotsAtPoint(PdfPoint* point) = 0;
  virtual PdfAnnot* GetAnnotAtPoint(PdfPoint* point, int index) = 0;
  virtual int GetNumAnnotsAtRect(PdfRect* rect) = 0;
  virtual PdfAnnot* GetAnnotAtRect(PdfRect* rect, int index) = 0;
  virtual bool DrawContent(PdfPageRenderParams* params, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
};

struct PdePageMap {
  virtual PdeElement* GetElement() = 0;
  virtual bool GetWhitespace(PdfWhitespaceParams* params, int index, _out_ PdfRect* bbox) = 0;
  virtual void GetBBox(_out_ PdfRect* bbox) = 0;
};

struct PdfPageView {
  virtual int GetDeviceWidth() = 0;
  virtual int GetDeviceHeight() = 0;
  virtual void GetDeviceMatrix(_out_ PdfMatrix* matrix) = 0;
  virtual void RectToDevice(PdfRect* rect, _out_ PdfDevRect* dev_rect) = 0;
  virtual void PointToDevice(PdfPoint* point, _out_ PdfDevPoint* dev_point) = 0;
};

struct PdfBookmark {
  virtual int GetTitle(_out_ wchar_t* buffer, int len) = 0;
  virtual void GetAppearance(PdfBookmarkAppearance* appearance) = 0;
  virtual PdfAction* GetAction() = 0;
  virtual int GetNumChildren() = 0;
  virtual PdfBookmark* GetChild(int index) = 0;
  virtual PdfBookmark* GetParent() = 0;
};

struct PsRegex {
  virtual void Destroy() = 0;
  virtual bool SetPattern(const wchar_t* pattern) = 0;
  virtual bool Search(const wchar_t* text, int position) = 0;
  virtual int GetText(_out_ wchar_t* buffer, int len) = 0;
  virtual int GetPosition() = 0;
  virtual int GetLength() = 0;
  virtual int GetNumMatches() = 0;
  virtual int GetMatchText(int index, _out_ wchar_t* buffer, int len) = 0;
};

struct PsStream {
  virtual void Destroy() = 0;
  virtual bool IsEof() = 0;
  virtual int GetSize() = 0;
  virtual bool Read(int offset, _out_ uint8_t* buffer, int size) = 0;
  virtual bool Write(int offset, _in_ const uint8_t* buffer, int size) = 0;
  virtual int GetPos() = 0;
  virtual bool Flush() = 0;
  virtual void* GetStream() = 0;
  virtual PdfStreamType GetType() = 0;
};

struct PsFileStream : PsStream {
};

struct PsMemoryStream : PsStream {
  virtual bool Resize(int size) = 0;
};

struct PsProcStream : PsStream {
  virtual void SetReadProc(_callback_ PsStreamProc proc) = 0;
  virtual void SetWriteProc(_callback_ PsStreamProc proc) = 0;
  virtual void SetDestroyProc(_callback_ PsStreamDestroyProc proc) = 0;
  virtual void SetGetSizeProc(_callback_ PsStreamGetSizeProc proc) = 0;
};

struct PdfStructElement {
  virtual const char* GetType() = 0;
  virtual const char* GetSubtype() = 0;
  virtual int GetActualText(_out_ wchar_t* buffer, int len) = 0;
};

struct PsMetadata {
  virtual bool SaveToStream(PsStream* stream) = 0;
  virtual bool LoadFromStream(PsStream* stream) = 0;
};

struct Pdfix {
  virtual void Destroy() = 0;
  virtual bool Authorize(const wchar_t* email, const wchar_t* serial_number) = 0;
  virtual bool IsAuthorized() = 0;
  virtual bool IsAuthorizedPlatform(PdfAuthPlatform platform)  = 0;
  virtual bool IsAuthorizedOption(PdfAuthOption option) = 0;
  virtual int GetErrorType() = 0;
  virtual const char* GetError() = 0;
  virtual void SetError(int type, const char* error) = 0;
  virtual int GetVersionMajor() = 0;
  virtual int GetVersionMinor() = 0;
  virtual int GetVersionPatch() = 0;
  virtual PdfDoc* OpenDoc(const wchar_t* path, const wchar_t* password) = 0;
  virtual PdfDoc* OpenDocFromStream(PsStream* stream, const wchar_t* password) = 0;
  virtual PdfDigSig* CreateDigSig() = 0;
#if defined _WIN32 && defined _MSC_VER
  virtual PdfCertDigSig* CreateCertDigSig() = 0;
#endif
  virtual PdfCustomDigSig* CreateCustomDigSig() = 0;
  virtual PsRegex* CreateRegex() = 0;
  virtual PsFileStream* CreateFileStream(const wchar_t* path, PsFileMode mode) = 0;
  virtual PsMemoryStream* CreateMemStream() = 0;
  virtual PsProcStream* CreateCustomStream(_callback_ PsStreamProc read_proc, PsStreamData client_data) = 0;
  virtual bool RegisterEvent(PdfEventType type, _callback_ PdfEventProc proc, void* data) = 0;
  virtual bool UnregisterEvent(PdfEventType type, PdfEventProc proc, void* data) = 0;
  virtual void SetRegex(PdfRegexType type, const wchar_t* regex) = 0;
  virtual PsImage* CreateImage(int width, int height, PsImageDIBFormat format) = 0;
};

struct PdfixPlugin {
  virtual void Destroy() = 0;
  virtual bool Initialize(Pdfix* pdfix) = 0;
  virtual int GetVersionMajor() = 0;
  virtual int GetVersionMinor() = 0;
  virtual int GetVersionPatch() = 0;
  virtual Pdfix* GetPdfix() = 0;
};

///////////////////////////////////////////////////////////////////////////////
// Pdfix initialization

#ifdef PDFIX_STATIC_LIB
Pdfix* GetPdfix();
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
typedef Pdfix* (*GetPdfixProcType)( );

// initialization
extern DLL_HANDLE* Pdfix_init(const char* path);
extern void Pdfix_destroy();
// static declarations
extern DLL_HANDLE g_Pdfix_handle;
extern GetPdfixProcType GetPdfix;

// static definitions Pdfix (use in the source file)
#define Pdfix_statics \
GetPdfixProcType GetPdfix = nullptr;\
DLL_HANDLE g_Pdfix_handle = 0;\
void Pdfix_destroy() {\
  if (g_Pdfix_handle) PdfixFreeLibrary(g_Pdfix_handle);\
  g_Pdfix_handle = nullptr;\
  GetPdfix = 0;\
}\
DLL_HANDLE* Pdfix_init(const char* path) {\
  g_Pdfix_handle = PdfixLoadLibrary(path);   if (!g_Pdfix_handle) return nullptr;\
  GetPdfix = (GetPdfixProcType)PdfixGetProcAddress(g_Pdfix_handle, "GetPdfix");\
  if (GetPdfix == nullptr) { Pdfix_destroy(); return nullptr; } return &g_Pdfix_handle; }

#ifndef Pdfix_MODULE_NAME
#if defined _WIN32
#if defined _WIN64
#define Pdfix_MODULE_NAME "pdfix64.dll"
#else
#define Pdfix_MODULE_NAME "pdfix.dll"
#endif
#elif defined __linux__
#if defined __x86_64__
#define Pdfix_MODULE_NAME "libpdfix64.so"
#else
#define Pdfix_MODULE_NAME "libpdfix.so"
#endif
#elif defined __APPLE__
#if defined __x86_64__
#define Pdfix_MODULE_NAME "libpdfix64.dylib"
#else
#define Pdfix_MODULE_NAME "libpdfix.dylib"
#endif
#else
#error unknown platform
#endif
#endif //Pdfix_MODULE_NAME

#endif // PDFIX_STATIC_LIB
#endif //_Pdfix_h
