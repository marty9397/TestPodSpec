/**************************************************
 * Copyright (c)2008 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL$
 *
 * Revision: $Revision$
 *
 * Last modified on: $Date$, by: $Author$
 *
 *************************************************/

#ifndef DTP_GENERICTEXTPROCESSOR_H
#define DTP_GENERICTEXTPROCESSOR_H

#include "MapTextDefs.h"

#include "SecureStringFunctions.h"
#include "PortableTchar.h"
#include <assert.h>
#include <stdio.h>

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the TEXTPROCESSOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// UNICODETEXTPROCESSOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DTP_TARGET_PLATFORM_WINDOWS
#   ifdef TEXTPROCESSOR_EXPORTS
#       define UNICODETEXTPROCESSOR_API __declspec( dllexport )
#       include <windows.h>
#   else
#       define UNICODETEXTPROCESSOR_API __declspec( dllimport )
#       ifdef DTP_TARGET_PLATFORM_WIN64
#           ifdef _DEBUG
#               pragma comment(lib, "TextProcessor-x64_dbg.lib")
#           else
#               pragma comment(lib, "TextProcessor-x64.lib")
#           endif
#       elif defined(DTP_TARGET_PLATFORM_WIN32)
#           ifdef _DEBUG
#               pragma comment(lib, "TextProcessor_dbg.lib")
#           else
#               pragma comment(lib, "TextProcessor.lib")
#           endif
#       endif
#   endif
#else
#   define UNICODETEXTPROCESSOR_API
#endif

// This class is exported from the UnicodeTextProcessor.dll
// It is built as a DLL for targeting Windows.
// Unicode is not supported on Unix.
// This module is exposed from AnsiTextProcessorLib.lib for Unix.
// The Windows DLL UnicodeTextProcessor.dll includes a stdafx.h that
// includes the above headers.

//class UNICODETEXTPROCESSOR_API CUnicodeTextProcessor {
//public:
//	CUnicodeTextProcessor(void);
// add your methods here.
//};

//extern UNICODETEXTPROCESSOR_API int nUnicodeTextProcessor;

//UNICODETEXTPROCESSOR_API int fnUnicodeTextProcessor(void);

#endif // DTP_GENERICTEXTPROCESSOR_H
