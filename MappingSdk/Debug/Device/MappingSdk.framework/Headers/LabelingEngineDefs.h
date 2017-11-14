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

#ifndef DTP_LABELINGENGINEDEFS_H
#define DTP_LABELINGENGINEDEFS_H

#include "SecureStringFunctions.h"
#include <assert.h>
#include "PortableTchar.h"
#include "MapTextDefs.h"


#ifdef DTP_TARGET_PLATFORM_WINDOWS
#   ifdef LABELINGENGINE_EXPORTS
#       define LABELINGENGINE_API __declspec( dllexport )
#   else
#       define LABELINGENGINE_API __declspec( dllimport )
#       if DTP_TARGET_PLATFORM_WIN64
#           ifdef _DEBUG
#               pragma comment(lib, "LabelingEngine-x64_dbg.lib")
#           else
#               pragma comment(lib, "LabelingEngine-x64.lib")
#           endif
#       elif defined(DTP_TARGET_PLATFORM_WINDOWS_RT) //uses the LabelingEngineCore lib
#           ifdef NET
#               ifdef _M_X64
#                   ifdef _DEBUG
#                       pragma comment(lib, "LabelingEngineCore-x64_dbg.lib")
#                   else
#                       pragma comment(lib, "LabelingEngineCore-x64.lib")
#                   endif
#               else _M_IX86
#                   ifdef _DEBUG
#                       pragma comment(lib, "LabelingEngineCore-x86_dbg.lib")
#                   else
#                       pragma comment(lib, "LabelingEngineCore-x86.lib")
#                   endif
#               endif
#           else
#               ifdef _M_ARM
#                   ifdef _DEBUG
#                       pragma comment(lib, "LabelingEngineCore-winrt-arm_dbg.lib")
#                   else
#                       pragma comment(lib, "LabelingEngineCore-winrt-arm.lib")
#                   endif
#               elif _M_X64
#                   ifdef _DEBUG
#                       pragma comment(lib, "LabelingEngineCore-winrt-x64_dbg.lib")
#                   else
#                       pragma comment(lib, "LabelingEngineCore-winrt-x64.lib")
#                   endif
#               elif _M_IX86
#                   ifdef _DEBUG
#                       pragma comment(lib, "LabelingEngineCore-winrt-x86_dbg.lib")
#                   else
#                       pragma comment(lib, "LabelingEngineCore-winrt-x86.lib")
#                   endif
#               endif
#           endif
#       elif defined(DTP_TARGET_PLATFORM_WIN32)  //regular Win32
#           ifdef _DEBUG
#               pragma comment(lib, "LabelingEngine_dbg.lib")
#           else
#               pragma comment(lib, "LabelingEngine.lib")
#           endif
#       else
#           error "Should not be possible, unless a new windows architecture appears."
#       endif
#   endif
#else
#   define LABELINGENGINE_API
#endif //DTP_TARGET_PLATFORM_WINDOWS

#endif // DTP_LABELINGENGINEDEFS_H
