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

#ifndef DTP_ERROR_H
#define DTP_ERROR_H

#include "MapTextDefs.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ERRMSG_LEN 4095

namespace MapText
{
	namespace Dtp
	{

		/**
		 * This abstract class may be inherited from and implement passing some
		 * additional error details beyond the standard Error class interface.
		 */
		class ErrorDetails
		{
		public:
			/**
			 * Constructor.
			 * @reqid 001.0063
			 */
			ErrorDetails( const TCHAR* message )
				: m_next( NULL )
			{
				SetMessage( message );
			}

			/**
			 * Add a unique enum value for each deriving class.
			 */
			enum ErrorDetailsType
			{
				ErrorDetailsType_ErrorObjectReference,
			};

			/**
			 * 'RTTI' method. Each deriving class has to implement this and return its type here.
			 * @return - enum value identifying the class.
			 * @reqid 001.0063
			 */
			virtual ErrorDetailsType GetType() = 0;

			/**
			 * Destructor.
			 * @reqid 001.0063
			 */
			virtual ~ErrorDetails()
			{
			}

			/**
			 * Set the detailed message.
			 * @param message - The message.
			 * @reqid 001.0063
			 */
			void SetMessage( const TCHAR* message )
			{
				assert( message );
				_tcsncpy_s(m_message, MAX_ERRMSG_LEN, message, _TRUNCATE);
			}

			const TCHAR* GetMessage() const
			{
				return m_message;
			}

			/**
			* Gets Next. Details can be chained.
			* @return - Next
			* @reqid 001.0063
			*/
			ErrorDetails* GetNext() const 
			{ 
				return m_next; 
			}

			/**
			* Sets the next detail in the chain. Ownership is taken.
			* @param val - Next error detail.
			* @reqid 001.0063
			*/
			void SetNext(ErrorDetails* val) 
			{ 
				delete m_next;
				m_next = val; 
			}

		private:
			TCHAR m_message[MAX_ERRMSG_LEN+1];
			ErrorDetails* m_next;
		};

		/**
		 * This class may be used by the code returning an error to supply a reference
		 * to the object which caused the error.
		 */
		template <class C>
		class ErrorObjectReference : public ErrorDetails
		{
		public:

			enum ObjectReferenceType
			{
				ObjectReferenceType_Feature,
				ObjectReferenceType_Label,
			};

			/**
			 * Constructor. Builds a new instance of error details class. Ownership is NOT taken.
			 * The error object travels up the stack, so it's necessary to only pass objects
			 * which will not be deleted before the error reaches the client. If this is impossible
			 * a clone should be made or some other means of reporting the error should be used.
			 * @param objectReference - The object causing the error.
			 * @reqid 001.0063
			 */
			ErrorObjectReference( const TCHAR* message, const C& objectReference )
				: m_objectReference( &objectReference ), ErrorDetails( message )
			{
			}

			/**
			* Gets ObjectReference
			* @return - Reference to the object which caused an error.
			* @reqid 001.0063
			*/
			const C* GetObjectReference() const 
			{ 
				return m_objectReference; 
			}

			/**
			 * @see ErrorDetails::GetType
			 * @reqid 001.0063
			 */
			ErrorDetailsType GetType()
			{
				return ErrorDetailsType_ErrorObjectReference;
			}

			ObjectReferenceType GetObjectType()
			{
				return m_objectReference->GetObjectType();
			}

		private:
			const C* m_objectReference;
		};


/**
 * A class used to record error information at the error site and communicate it up the stack.
 * @reqid 001.0063
 */
class MAPTEXTTYPES_API Error
{
	public:
				/**
				 * Default constructor.
				 * @reqid 001.0063
				 */
				Error();

				/**
				 * Constructor.
				 * @param errorCode			the ErrorCode value
				 * @param errorMessage	the associated message string, if any
				 * @param file					the source file in which error occurred
				 * @param line					the source line at which error occurred
				 * @reqid 001.0063
				 */
				Error( int errorCode, const TCHAR* errorMessage,
					const TCHAR* file, int line );

				~Error();

				/**
				 * Set the error information.
				 * @param errorCode			the ErrorCode value
				 * @param errorMessage	the associated message string, if any
				 * @param file					the source file in which error occurred
				 * @param line					the source line at which error occurred
				 * @reqid 001.0063
				 */
				void SetError( int errorCode, const TCHAR* errorMessage,
					const TCHAR* file, int line  );

				/**
				 * Set the error message variadically.
				 * @param format	a conversion specification
				 * @param ...			an argument list, if any specified
				 * @reqid 001.0063
				 */
				void SetErrorMessageFormatted( const TCHAR* format, ... );

				/**
				 * Report the error in a configured manner, i.e., to the log file.
				 * @reqid 001.0063
				 */
				void Report();

				/**
				 * Retrieve the error code.
				 * @return the error code
				 * @reqid 001.0063
				 */
				int GetErrorCode() const;

				/**
				 * Set the error code.
				 * @param errorCode	the value indicating the specific error that occurred
				 * @reqid 001.0063
				 */
				void SetErrorCode( int errorCode );

				/**
				 * Retrieve the error message string.
				 * @return the address of the message string buffer
				 * @reqid 001.0063
				 */
				const TCHAR* GetErrorMessage() const;

				/**
				 * Set the error code.
				 * @param errorMessage	the address of a buffer containing the error message string
				 * @reqid 001.0063
				 */
				void SetErrorMessage( const TCHAR* errorMessage );

				/**
				 * Retrieve the source line number at which the error occurred.
				 * @return the source line number at which the error occurred
				 * @reqid 001.0063
				 */
				int GetLine() const;

				/**
				 * Set the source line number at which the error occurred.
				 * @param line	the source line number at which the error occurred
				 * @reqid 001.0063
				 */
				void SetLine( int line );

				/**
				 * Retrieve the fully qualified source file name in which the error occurred.
				 * @return the source file in which the error occurred
				 * @reqid 001.0063
				 */
				const TCHAR* GetFile() const;

				/**
				 * Set the source file in which the error occurred.
				 * @param file	the fully qualified source file name in which the error occurred.
				 * @reqid 001.0063
				 */
				void SetFile( const TCHAR* file );

				/**
				 * Reset the error to initial values (no msg, error code 0, no file and line)
				 * @reqid 001.0063
				 */
				void Reset();

				/**
				 * Casting operator for bool - allows easy check if there is an error.
				 * @return true if errorCode is NOT 0, 0 otherwise.
				 * @reqid 001.0065
				 */
				operator bool();

				/**
				* Gets extended details of the error.
				* @return - An instance of an implementation o the ErrorDetails interface. 
				*           Call GetType to get the 'RTTI'.
				* @reqid 001.0063
				*/
				ErrorDetails* GetDetails() const 
				{ 
					return m_details; 
				}

				/**
				* Sets Details.
				* @param val - Details. Ownership is taken replacing the existing details.
				* @reqid 001.0063
				*/
				void SetDetails(ErrorDetails* val) 
				{ 
					delete m_details;
					m_details = val; 
				}

				/**
				* Append new details to the chain.
				* @param val - Details. Ownership is taken and the details are appended at the end of the chain.
				* @reqid 001.0063
				*/
				void AppendDetails(ErrorDetails* val) 
				{ 
					if ( m_details == NULL )
					{
						m_details = val; 
					}
					else
					{
						ErrorDetails* tail = m_details;
						while ( tail->GetNext() != NULL )
						{
							tail = tail->GetNext();
						}

						tail->SetNext( val );
					}
				}

		private:
    
                /**
                 * Disallow assignment operator
                 * @req 001.063
                 */
                Error& operator = (const Error& )
                {
                    return *this;                    
                }
                
                /**
                 * Disallow copy constructor
                 * @req 001.063
                 */
                Error(const Error& )
                {
                }
                
				TCHAR m_errorMessage[MAX_ERRMSG_LEN+1];
				int m_errorCode;
				const TCHAR* m_file;
				int m_line;
				ErrorDetails* m_details;
		};

/**
 * Use this macro whenever you need to set the error info
 */

#define SET_ERROR( error, errorCode, errorMessage ) \
	error.SetError( errorCode, (TCHAR*)errorMessage, (TCHAR*)__FILE__, __LINE__ ); \
	error.Report();

#define SET_ERROR_FORMATTED( error, errorCode, format, ... ) \
	error.SetErrorCode(errorCode); \
	error.SetErrorMessageFormatted( (TCHAR*)format, __VA_ARGS__ ); \
	error.SetFile((TCHAR*) __FILE__ ); \
	error.SetLine( __LINE__ ); \
	error.Report();

#define SET_ERROR_NOMSG( error, errorCode ) \
	error.SetError( errorCode, _T( # errorCode ), (TCHAR*)__FILE__, __LINE__ ); \
	error.Report();

	} // namespace Dtp
} // namespace MapText

#endif // DTP_ERROR_H
