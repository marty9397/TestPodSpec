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

#ifndef DTP_MAPTEXTDEFS_H
#define DTP_MAPTEXTDEFS_H

/**
 Include this file as early as possible in client projects, preferrably in the
 pre-compiled header. It establishes a few defines which are cruicial for proper
 building on different platforms. These platform-specific defines shall be set:
 
 DTP_TARGET_PLATFORM_IOS - building for iOS platform
 DTP_TARGET_PLATFORM_MAC_OS_X - building for Apple desktop platform
 DTP_TARGET_PLATFORM_APPLE - DTP_TARGET_PLATFORM_IOS || DTP_TARGET_PLATFORM_MAC_OS_X
 DTP_TARGET_PLATFORM_WIN32 - building for Windows 32 bit platform
 DTP_TARGET_PLATFORM_WIN64 - building for Windows 64 bit platform
 DTP_TARGET_PLATFORM_WINDOWS_DESKTOP - DTP_TARGET_PLATFORM_WIN32 || DTP_TARGET_PLATFORM_WIN64
 DTP_TARGET_PLATFORM_WINDOWS_RT - building for Windows Store platform
 DTP_TARGET_PLATFORM_WINDOWS - DTP_TARGET_PLATFORM_WINDOWS_DESKTOP || DTP_TARGET_PLATFORM_WINDOWS_RT
 DTP_TARGET_PLATFORM_ANDROID - building for Android platform
 DTP_TARGET_PLATFORM_NIX - building for the *nix ecosystem
 
 All of the flags above are mutually exclusive with the exception of the ones 
 explicitly defined as combinations. Once detected they will be defined as `1`.
 */

#include "SecureStringFunctions.h"
#ifdef _MSC_VER
#   include <Windows.h>
#endif

#ifdef WINAPI_FAMILY
#   if WINAPI_FAMILY == WINAPI_FAMILY_APP
#       define WINDOWS_RUNTIME
#   endif
#endif

#include <assert.h>

#ifdef __APPLE__
//////////////////////////////////////
//              Apple
//////////////////////////////////////
#   define DTP_TARGET_PLATFORM_APPLE 1
#   define MAPTEXTTYPES_API
#   import <TargetConditionals.h>
#   if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
//////////////////////////////////////
//               iOS
//////////////////////////////////////
#       define DTP_TARGET_PLATFORM_IOS 1
#   else // Apple & not iOS means Mac OS desktop target
//////////////////////////////////////
//          Mac OS X desktop
//////////////////////////////////////
#       define DTP_TARGET_PLATFORM_MAC_OS_X 1
#   endif
#elif defined(_WIN32)
//////////////////////////////////////
//           Windows (any)
//////////////////////////////////////
#   define DTP_TARGET_PLATFORM_WINDOWS 1
#   if defined(_WIN64) && !defined(WINDOWS_RUNTIME) // WIN 64 detected
//////////////////////////////////////
//              Win 64
//////////////////////////////////////
#       define DTP_TARGET_PLATFORM_WIN64 1
#       define DTP_TARGET_PLATFORM_WINDOWS_DESKTOP 1
#       ifdef MAPTEXTTYPES_EXPORTS
#           define MAPTEXTTYPES_API __declspec( dllexport )
#       else
#           define MAPTEXTTYPES_API __declspec( dllimport )
#           ifdef _DEBUG
#               pragma comment(lib, "MapTextTypes-x64_dbg.lib")
#           else
#               pragma comment(lib, "MapTextTypes-x64.lib")
#           endif
#       endif
#   elif defined(WINDOWS_RUNTIME)  // Windows RT detected
//////////////////////////////////////
//       Windows RT (Store App)
//////////////////////////////////////
#       define DTP_TARGET_PLATFORM_WINDOWS_RT 1
#       ifdef MAPTEXTTYPES_EXPORTS
#           define MAPTEXTTYPES_API __declspec( dllexport )
#       else
#           define MAPTEXTTYPES_API __declspec( dllimport )
#       endif
#   elif !defined(WINDOWS_RUNTIME) // WIN 32 detected
//////////////////////////////////////
//              Win 32
//////////////////////////////////////
#       define DTP_TARGET_PLATFORM_WIN32 1
#       define DTP_TARGET_PLATFORM_WINDOWS_DESKTOP 1
#       ifdef MAPTEXTTYPES_EXPORTS
#           define MAPTEXTTYPES_API __declspec( dllexport )
#       else
#           define MAPTEXTTYPES_API __declspec( dllimport )
#           ifdef _DEBUG
#               pragma comment(lib, "MapTextTypes_dbg.lib")
#           else
#               pragma comment(lib, "MapTextTypes.lib")
#           endif
#       endif
#   endif
#elif defined (ANDROID)
//////////////////////////////////////
//             Android
//////////////////////////////////////
#   define DTP_TARGET_PLATFORM_ANDROID 1
#else
//////////////////////////////////////
//          Other *nix
// ----------------------------------
// If we need to further differentiate
// it would happen here.
//////////////////////////////////////
#   define DTP_TARGET_PLATFORM_NIX 1
#   define MAPTEXTTYPES_API
#endif //_WIN32

#include "SecureStringFunctions.h"
#ifdef __cplusplus
#   include "PortableTchar.h"
#endif

// Visual Studio uses _DEBUG, GCC uses DEBUG, it causes problems.
// Make sure one type of debug directive triggers the other.
#ifdef _DEBUG
#	define DEBUG
#else
#	ifdef DEBUG
// TODO: Fix _DEBUG content for GCC before uncommenting this
//#		define _DEBUG
#	endif
#endif

#endif // DTP_MAPTEXTDEFS_H
