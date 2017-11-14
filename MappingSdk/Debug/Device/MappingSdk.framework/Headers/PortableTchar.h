/**************************************************
* Copyright (c)2009 Maptext, Inc. All rights reserved.
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

/**************************************************
INCLUDE FILES, PREPROCESSOR
*************************************************/

#ifndef PORTABLE_TCHAR_H
#define PORTABLE_TCHAR_H

#include "MapTextDefs.h" // For platform detection

/**************************************************
DEFINITIONS
*************************************************/

#ifdef DTP_TARGET_PLATFORM_WINDOWS
	
	#include <tchar.h> // All we need is here

#else // We define the generics

	#ifdef UNICODE
		#include <wchar.h>
		#include <wctype.h>

		#define TCHAR		wchar_t
	#else
		#include <string.h>
		
		#ifdef ANDROID
			#include <ctype.h>
		#endif

		#include <locale>		

		#define TCHAR		char
	#endif
	
//	#include <StringComparer.h>

	#ifdef UNICODE

		////////////////////////////////////////
		// UNICODE
		////////////////////////////////////////

		/* String functions */

		#define _tcschr     wcschr
		#define _tcscspn    wcscspn
		#define _tcsncat    wcsncat
		#define _tcsncpy    wcsncpy
		#define _tcsncpy_s(strDest, size, strSrc, count) wcsncpy(strDest, strSrc, size)
		#define _tcspbrk    wcspbrk
		#define _tcsrchr    wcsrchr
		#define _tcsspn     wcsspn
		#define _tcsstr     wcsstr

		static TCHAR* wcstokPtr;

		#define _tcstok( string, delim )	wcstok( string, delim, &wcstokPtr )

		#define _tcsnset    _wcsnset
		#define _tcsrev     _wcsrev
		#define _tcsset     _wcsset

		#define _tcscmp     wcscmp

		#ifdef __APPLE__
			#define _tcsicmp( s1, s2 )    TextProcessor::StringComparer::CompareNoCase( s1, s2, NULL )
		#else
			#define _tcsicmp( s1, s2 )    wcscasecmp( s1, s2 )
		#endif
		
		#define _tcsnccmp   wcsncmp
		#define _tcsncmp    wcsncmp
		#define _tcsncicmp  wcsncasecmp
		#define _tcsnicmp   _wcsnicmp

		#define _tcscoll    wcscoll
		#define _tcsicoll   _wcsicoll
		#define _tcsnccoll  _wcsncoll
		#define _tcsncoll   _wcsncoll
		#define _tcsncicoll _wcsnicoll
		#define _tcsnicoll  _wcsnicoll

		/* "logical-character" mappings */

		#define _tcsclen    wcslen
		#define _tcsnccat   wcsncat
		#define _tcsnccpy   wcsncpy
		#define _tcsncset   _wcsnset

		/* MBCS-specific functions */

		#define _tcsdec     _wcsdec
		#define _tcsinc     _wcsinc
		#define _tcsnbcnt   _wcsncnt
		#define _tcsnccnt   _wcsncnt
		#define _tcsnextc   _wcsnextc
		#define _tcsninc    _wcsninc
		#define _tcsspnp    _wcsspnp

		#define _tcslwr     _wcslwr
		#define _tcsupr     _wcsupr
		#define _tcsxfrm    wcsxfrm

		#define _tcscat     wcscat
		#define _tcscpy     wcscpy

		#ifdef __APPLE__ 
			#include <stdlib.h>
			/**
			 * Unfortunately wcsdup is missing on MacOS X, we needed to craft
			 * our own implementation.
			 * Does the job - gets a fresh new copy of a string
			 * @param inputString
			 */
			static TCHAR* wcsdup( const TCHAR* inputString )
			{
				if ( inputString == NULL )
				{
					return NULL;
				}

				TCHAR* output = (TCHAR*) malloc( ( wcslen( inputString ) + 1 ) * sizeof( TCHAR ) );

				wcscpy( output, inputString );

				return output;
			}

			#define _tcsdup( s )     wcsdup( s )
		#else 
			#define _tcsdup     wcsdup
		#endif // __APPLE__

		#define _tcslen     wcslen

		#define _tprintf    wprintf
		#define _ftprintf   fwprintf
		#define _sntprintf  swprintf
		#define _vtprintf   vwprintf
		#define _vftprintf  vfwprintf
		#define _vstprintf  vswprintf
		#define _vsntprintf vswprintf
		#define _vsntprintf_s(buffer, size, count, format, argptr) vswprintf(buffer, size, format, argptr)
		#define _TRUNCATE 0
		#define _tscanf     wscanf
		#define _ftscanf    fwscanf
		#define _stscanf    swscanf

		#define _itot       _itow
		#define _ltot       _ltow
		#define _ultot      _ultow
		#define _ttoi(str)  wcstol(str, NULL, 10)
		#define _ttol(str)  wcstol(str, NULL, 10)

		#define _tcstod     wcstod
		#define _tcstol     wcstol
		#define _tcstoul    wcstoul

		#define _fgettc     fgetwc
		#define _fgettchar  _fgetwchar
		#define _fgetts     fgetws
		#define _fputtc     fputwc
		#define _fputtchar  _fputwchar
		#define _fputts     fputws
		#define _gettc      getwc
		#define _gettchar   getwchar
		#define _getts      _getws
		#define _puttc      putwc
		#define _puttchar   putwchar
		#define _putts      _putws
		#define _ungettc    ungetwc

		#define _tfsopen    _wfsopen

			//For unicode build the implementation of this function
			//has to be moved from MtClassLib
			extern FILE* mtfwopen( const wchar_t* path, const wchar_t* mode );

			#define _tfopen( path, mode ) mtfwopen( path, mode )

		#define _tfreopen   _wfreopen
		#define _tperror    _wperror
		#define _tpopen     _wpopen
		#define _ttempnam   _wtempnam
		#define _ttmpnam    _wtmpnam

		#define _tasctime   _wasctime
		#define _tctime     _wctime
		#define _tstrdate   _wstrdate
		#define _tstrtime   _wstrtime
		#define _tutime     _wutime
		#define _tcsftime   wcsftime

		#define _tchdir     _wchdir
		#define _tgetcwd    _wgetcwd

		#define _tremove    _wremove
		#define _trename    _wrename

		#define _istlead(_c)    (0)
		#define _istleadbyte(_c)    (0)

		/* ctype-functions */

		#define _istalnum   iswalnum
		#define _istalpha   iswalpha
		#define _istdigit   iswdigit
		#define _istgraph   iswgraph
		#define _istlower   iswlower
		#define _istprint   iswprint
		#define _istpunct   iswpunct
		#define _istspace   iswspace
		#define _istupper   iswupper

		#define _totupper   towupper
		#define _totlower   towlower

		#define _istlegal(_c)   (1)
			
		#define _tsetlocale setlocale
			
		#define _tstoi( string )	wcstol( string, NULL, 10 )
		#define _tstof( string )	wcstof( string, NULL )

		#define _TEOF       WEOF

		/* Generic text macros to be used with string literals and character constants.
			Will also allow symbolic constants that resolve to same. */
		#define __T(x)		L ## x
		#define _T(x)       __T(x)

	#else //#ifdef UNICODE

		////////////////////////////////////////
		// NON UNICODE
		////////////////////////////////////////
		
		#define TCHAR		char
		/* String functions */
		
		#define _tcschr     strchr
		#define _tcscspn    strcspn
		#define _tcsncat    strncat
		#define _tcsncpy    strncpy
		#define _tcsncpy_s(strDest, size, strSrc, count) strncpy(strDest, strSrc, size)
		#define _tcspbrk    strpbrk
		#define _tcsrchr    strrchr
		#define _tcsspn     strspn
		#define _tcsstr     strstr
		#define _tcstok     strtok
		
		#define _tcsnset    _strnset
		#define _tcsrev     _strrev
		#define _tcsset     _strset
		
		#define _tcscmp     strcmp
		#define _tcsicmp    strcasecmp
		#define _tcsnccmp   strncmp
		#define _tcsncmp    strncmp
		#define _tcsncicmp  strncasecmp
		#define _tcsnicmp   _strnicmp
		
		#define _tcscoll    strcoll
		#define _tcsicoll   _stricoll
		#define _tcsnccoll  _strncoll
		#define _tcsncoll   _strncoll
		#define _tcsncicoll _strnicoll
		#define _tcsnicoll  _strnicoll
		
		/* "logical-character" mappings */
		
		#define _tcsclen    strlen
		#define _tcsnccat   strncat
		#define _tcsnccpy   strncpy
		#define _tcsncset   _strnset
		
		/* MBCS-specific functions */
		
		#define _tcsdec     _strdec
		#define _tcsinc     _strinc
		#define _tcsnbcnt   _strncnt
		#define _tcsnccnt   _strncnt
		#define _tcsnextc   _strnextc
		#define _tcsninc    _strninc
		#define _tcsspnp    _strspnp
		
		#define _tcslwr     _strlwr
		#define _tcsupr     _strupr
		#define _tcsxfrm    strxfrm
		
		#define _tcscat     strcat
		#define _tcscpy     strcpy
		#define _tcsdup     strdup
		
		#define _tcslen     strlen
		#define _tcsxfrm    strxfrm
		
		#define _tprintf    printf
		#define _ftprintf   fprintf
		#define _stprintf   sprintf
		#define _sntprintf  snprintf
		#define _vtprintf   vprintf
		#define _vftprintf  vfprintf
		#define _vstprintf  vsprintf
		#define _vsntprintf vsnprintf
		#define _vsntprintf_s(buffer, size, count, format, argptr) vsnprintf(buffer, size, format, argptr)
		#define _TRUNCATE 0
		#define _tscanf     scanf
		#define _ftscanf    fscanf
		#define _stscanf    sscanf
		#define _itot       _itoa
		#define _ltot       _ltoa
		#define _ultot      _ultoa
		#define _ttoi       atoi
		#define _ttol       atol
		#define _tcstod     strtod
		#define _tcstol     strtol
		#define _tcstoul    strtoul
		#define _fgettc     fgetc
		#define _fgettchar  _fgetchar
		#define _fgetts     fgets
		#define _fputtc     fputc
		#define _fputtchar  _fputchar
		#define _fputts     fputs
		#define _gettc      getc
		#define _gettchar   getchar
		#define _getts      gets
		#define _puttc      putc
		#define _puttchar   putchar
		#define _putts      puts
		#define _ungettc    ungetc
		#define _tfsopen    _fsopen
		#define _tfopen     fopen
		#define _tfreopen   freopen
		#define _tperror    perror
		#define _tpopen     _popen
		#define _ttempnam   _tempnam
		#define _ttmpnam    tmpnam
		#define _tasctime   asctime
		#define _tctime     ctime
		#define _tstrdate   _strdate
		#define _tstrtime   _strtime
		#define _tutime     _utime
		#define _tcsftime   strftime
		#define _tchdir     chdir
		#define _tgetcwd    getcwd
		#define _tremove    remove
		#define _trename    rename
		
		#define _istlead(_c)    (0)
		#define _istleadbyte(_c)    (0)
		
		/* ctype-functions */
		
		#define _istalnum   isalnum
		#define _istalpha   isalpha
		#define _istdigit   isdigit
		#define _istgraph   isgraph
		#define _istlower   islower
		#define _istprint   isprint
		#define _istpunct   ispunct
		#define _istspace   isspace
		#define _istupper   isupper
		
		#define _totupper   toupper
		#define _totlower   tolower
		
		#define _istlegal(_c)   (1)
		
		#define _tsetlocale setlocale
		
		#define _tstoi( string )	atol( string )
		#define _tstof( string )	atof( string )
		
		#ifndef WEOF
			#define WEOF EOF
		#endif
		
		/* Generic text macros to be used with string literals and character constants.
			Will also allow symbolic constants that resolve to same. */
		#define _T(x)       x

	#endif //#ifdef UNICODE

#endif //platform

#endif // PORTABLE_TCHAR_H
