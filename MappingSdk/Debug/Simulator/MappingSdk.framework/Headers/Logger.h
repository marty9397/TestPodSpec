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

#ifndef DTP_LOGGER_H
#define DTP_LOGGER_H

#include "MapTextDefs.h"
#include "MtString.h"

#include <stdio.h>

#define LOG_BUFFER_SIZE 4096
#define _LOG_BUFFER_SIZE LOG_BUFFER_SIZE + 512

#ifndef _MAX_PATH
// In *nix there's no certain way to know, some implementations even allow unrestricted lengths of paths.
// Let's be reasonable here, worst case we'll truncate the path and error out.
#	define _MAX_PATH 1024
#endif

namespace MapText
{
	namespace Dtp
	{
//	Macro wrappers for LogXxx methods that may be controlled via conditional compilation
		/**
		 * Use these macro whenever you need to log an event
		 */

		// Errors must always be written to the log file.
		#define LOG_ERROR( message, sourceFile, line ) \
			Logger::LogError( message, sourceFile, line );

		#define LOG_WARNING( message ) \
			Logger::LogWarning( message );

		#define LOG_INFO( message ) \
			Logger::LogInfo( message );

		// forward declaration
		class Error;

		/**
		 * Class with static logging methods. Before any other method is called the
		 * Initialize method must be called.
		 */
		class MAPTEXTTYPES_API Logger
		{
			public:
				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_NotInitialized = 1
				};

				/**
				 * Enumeration of formats that can be used as part of the file name.
				 * @reqid 001.0063
				 */
				enum TimeStringFormat
				{
					/**
					 * Format that can be used as part of the file name. Full date and time.
					 */
					TS_FILE_SAFE_DATETIME,
					/**
					 * Format that can be used as part of the file name. Date only.
					 */
					TS_FILE_SAFE_DATE,
					/**
					 * Format that can be used as part of the file name. Time only.
					 */
					TS_FILE_SAFE_TIME,
					/**
					 * Format with file unsave characters ( like :, /, \ etc. ). Full date and time.
					 */
					TS_DATETIME,
					/**
					 * Format with file unsave characters ( like :, /, \ etc. ). Date only.
					 */
					TS_DATE,
					/**
					 * Format with file unsave characters ( like :, /, \ etc. ). Time only.
					 */
					TS_TIME
				};

				/**
				 * Enumeration used to filter logging of messages of only the appropriate criticality level.
				 * Enumerates the possible values for the minimum level message to be logged.
				 * Use CRITICALITYLEVEL_NONE for silent mode.
				 * @reqid 001.0063
				 */
				enum CriticalityLevel
				{
					/**
					 * Log every message.
					 */
					CRITICALITYLEVEL_INFO,

					/**
					 * Log only warning and error messages.
					 */
					CRITICALITYLEVEL_WARN,

					/**
					 * Log only error messages.
					 */
					CRITICALITYLEVEL_ERROR,

					/**
					 * Silent mode.
					 */
					CRITICALITYLEVEL_NONE
				};

				/**
				 * A static buffer - no need to allocate buffers in places of use.
				 * @reqid 001.0063
				 */
				static TCHAR s_LogBuffer[LOG_BUFFER_SIZE];

				/**
				* Close open log file(s).
				* @reqid 001.0063
				*/
				static void EndLog();

				/**
				* Action logging procedure. Logs the message into WorkLog file.
				* It is equal to Logger::Log( Logger::Verbose, const char *message )
				* @param message the formatting string
				* @param ... a variable arguments list, similar to printf
				* @return true if successful; false if not
				* @reqid 001.0063
				*/

				static bool LogInfo( const TCHAR* message, ... );

				/**
				 * Log a warning message. Time string will be appended before the actual message
				 * @param message the formatting string
				 * @param ... a variable arguments list, similar to printf
				 * @return true if successful; false if not
				 * @reqid 001.0063
				 */
				static bool LogWarning( const TCHAR* message, ... );

				/**
				 * Log an error message with diagnostics. Time string will be appended before the actual message.
				 * @param message			the message to log
				 * @param sourceFile	the source code file which triggered the error
				 * @param line				the source code line number at which the error was triggered
				 * @return true if successful; false if not
				 * @reqid 001.0063
				 */
				static bool LogError( const TCHAR* message, const TCHAR* sourceFile,
					long line );

				/**
				 * Logs message with a custom criticality level.
				 * @param customLevel the custom log level to be logged
				 * @param message to be logged (works like sprintf formatting string)
				 * @param ... a variable arguments list, similar to printf
				 * @return true if successful; false if not
				 * @reqid 001.0063
				 */
				static bool Custom( const TCHAR* customLevel, const TCHAR* message,
					... );

				/**
				 * Gets current time in an appropriate format
				 * @param timeString [out] the time string
				 * @param format as described on the enumeration
				 * @param bufferLength length of the buffer passed
				 * @reqid 001.0063
				 */
				static void GetTimeString( TCHAR* timeString,
					TimeStringFormat format,
					int bufferLength );

				/**
				 * Checks first char pointer for NULL and if positive returns second one.
				 * @param nilableBuffer pointer to test
				 * @param replacement pointer returned if first one is null
				 * @return first pointer if not NULL; second one otherwise
				 * @reqid 001.0063
				 */
				static const TCHAR* IsNull( const TCHAR* nilableBuffer,
					const TCHAR* replacement );

				/**
				 * Initialize the logger.
				 * Assign member variables and open and write message to log file if min level is INFO.
				 * @param s_logFilesPrefix	the prefix of the log file name
				 * @param s_appendMessage		a custom append message saved into the log file every time
				 * @param minLevel					lowest criticality level logged
				 * @param error							an Error object
				 * @reqid 001.0063
				*/
				static void Initialize( const TCHAR* logDir,
					const TCHAR* s_logFilesPrefix,
					const TCHAR* s_appendMessage, CriticalityLevel minLevel,
					Error& error );

				/**
				 * Uninitialize the logger.  Reset the initialized flag.
				 * @reqid 001.0063
				 */
				static void Uninitialize();

				/**
				 * Retrieve the name of the log file.
				 * @return fully qualified name of the log file
				 * @reqid 001.0063
				 */
				static const TCHAR* GetLogFileName( Error& error );

				/**
				 * Retrieve the criticality level.
				 * @return the lowest criticality level logged
				 * @reqid 001.0063
				 */
				static CriticalityLevel GetCriticalityLevel();

			protected:
				/**
				 * Log a message into a file.
				 * File of the given name is created or appended.
				 * The log format is a delimited text file using '|' (pipe) characters.
				 * The columns are:
				 * <code>
				 * Date | Log Level | Message
				 * </code>
				 * @param message to be logged
				 * @param logLevel	the criticality level of the message
				 * @param sourceFileCore core of the log file name. Physical file is
				 *        given a name of the form '__sourceFileCore_date in the form yyyy_mm_dd>.log'
				 * @return true if successful; false if not
				 * @reqid 001.0063
				 */
				static bool LogToFile( const TCHAR* message, const TCHAR* logLevel,
					const TCHAR* sourceFileCore );

				/**
				 * Log a message into the unified log file with a proper log level.
				 * @param message		the message to be logged
				 * @param logLevel	the criticality level of the message
				 * @return true if successful; false if not
				 * @reqid 001.0063
				 */
				static bool LogLevel( const TCHAR* message, const TCHAR* logLevel );

				// LoggerInternals is used to prevent complier from thinking that the static members below
				// This is because VC++ sucks and does not support properly STL, especially
				// map container. See http://support.microsoft.com/kb/168958 for details.

				/**
					* Struct used to prevent compiler from thinking that the static members below are also exported.
					* This is because VC++ does not properly support STL, especially the map container.
					* See http://support.microsoft.com/kb/168958 for details.
					* @reqid 001.0063
					*/
				struct LoggerInternals
				{
					static TCHAR _s_LogBuffer[_LOG_BUFFER_SIZE];

					/*
					 * Table of handles to all the files open at the time.
					 */
					//static std::map< String, FILE * > logFiles;

					/**
					 * The log file path.
					 * @reqid 001.0063
					 */
					static TCHAR s_logFilePath[_MAX_PATH+1];

					/**
					 * The open log file handle.
					 * @reqid 001.0063
					 */
					static FILE* s_logFileHandle;

					/**
					 * The folder where the log file(s) is (are) created
					 * @reqid 001.0063
					 */
					static String s_dataDir;

					/**
					 * Prefix of the log file name(s)
					 * @reqid 001.0063
					 */
					static String s_logFilesPrefix;

					/**
					 * Custom append message saved into the log file every time
					 * @reqid 001.0063
					 */
					static String s_appendMessage;

					/**
					 * Flag indicating whether initialization has been done
					 * @reqid 001.0063
					 */
					static bool s_initialized;

					/**
					 * The lowest criticality level logged
					 * @reqid 001.0063
					 */
					static Logger::CriticalityLevel s_minCriticalityLevel;
				};

			private:
				/** This is utility class that cannot be instantiated. */
				Logger()
				{
				};
				virtual ~Logger() {};
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_LOGGER_H
