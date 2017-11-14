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

#ifndef DTP_MTSTRING_H
#define DTP_MTSTRING_H

#include "MapTextDefs.h"
#include "Collection.h"

namespace MapText
{
	namespace Dtp
	{
		template <class C> class LinkedList;
		class Error;

		/**
		* Class that encapsulates a character buffer.
		* @reqid 001.0063
		*/
		class MAPTEXTTYPES_API String
		{
			public:
				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
					ErrorCode_InvalidStringIndex
				};

				/**
				* Possible sides from which to trim specified text from a string.
				* @reqid 001.0063
				*/
				enum StringSide
				{
					LeftSide = 0,
					RightSide,
					BothSides
				};

				/**
				 * Collection of strings
				 * @reqid 001.0063
				 */
				class MAPTEXTTYPES_API Collection : public LinkedList<String>
				{
					public:

					/**
					 * Constructor.
					 * @see LinkedList constructor
					 * @reqid 001.0063
					 */
					Collection( ) : LinkedList<String>( false )
					{
					}

					/**
					 * Constructor.
					 * @see LinkedList constructor
					 * @param ownsElements 
					 * @reqid 001.0063
					 */
					Collection( bool ownsElements ) : LinkedList<String>( ownsElements )
					{
					}

					/**
					 * Destructor declared explicitly to avoid compiler generated object code.
					 * @reqid 001.0063
					 */
					~Collection()
					{}
				};

				/**
				* Default constructor.
				* @reqid 001.0063
				*/
				String();

				/**
				* Copy constructor.
				* @param str		The string to copy
				* @param error	An error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				String( const String &str, Error& error );

				/**
				* Constructor.  This always makes copy of the buffer.
				* @param inbuffer		The character buffer to copy
				* @param error			An error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				String( const TCHAR *inbuffer, Error& error );

				/**
				* Destructor.
				* @reqid 001.0063
				*/
				~String();

			private:
				// Prohibit use of assignment operators that allocate memory
				// because they cannot report the error.
				// Added AssignString to be used instead of assignment operator.
				// Added ConcatString to be used instead of concatenation operator.
				// For the same reason, prohibit use of the standard copy constructor.
				void operator= ( const String& str );
				void operator= ( const TCHAR* str );
				String( const String &str );

			public:
				/**
				* Non liguistically-sensitive comparison. For lingustical comparison
				* use the TextProcessor StringComparer classes.
				* @param str the string to compare to
				* @return true if strings are equal
				*/

				/**
				* Equality operator.
				* Performs a simple comparison. For linguistically sensitive comparisons,
				* use the TextProcessor StringComparer class.
				* @param str	The string to compare to this one
				* @return true if strings are equal
				* @reqid 001.0063
				*/
				bool operator== ( const String& str ) const;

				/**
				* Equality operator.
				* Performs a simple comparison. For linguistically sensitive comparisons,
				* use the TextProcessor StringComparer class.
				* @param str	The buffer to compare to this string's buffer
				* @return true if strings are equal
				* @reqid 001.0063
				*/
				bool operator== ( const TCHAR* str ) const;

				/**
				* Inequality operator.
				* Performs a simple comparison. For linguistically sensitive comparisons,
				* use the TextProcessor StringComparer class.
				* @param str	The string to compare to this string
				* @return true if strings are NOT equal; false if equal
				* @reqid 001.0063
				*/
				bool operator!= ( const String& str ) const;

				/**
				* Inequality operator.
				* @param str	The buffer to compare to this string's buffer
				* Performs a simple comparison. For linguistically sensitive comparisons,
				* use the TextProcessor StringComparer class.
				* @return true if strings are NOT equal; false if equal
				* @reqid 001.0063
				*/
				bool operator!= ( const TCHAR* str ) const;

				/**
				* Indexing operator.
				* @param i	The index of the requested character in the buffer
				* @reqid 001.0063
				*/
				TCHAR operator[] ( int i ) const;

				/**
				* Set the internal buffer pointer to the incoming one.
				* @param inbuffer				The incoming character buffer
				* @param takeownership	Flag indicating whether the string should free the buffer
				* @reqid 001.0063
				*/
				void SetString( const TCHAR* inbuffer, bool takeownership = false );

				/**
				* Set the internal buffer pointer to a copy of the incoming one.
				* This always takes ownership of the copied buffer.
				* @param inbuffer		The incoming character buffer
				* @param error			An Error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				void AssignString( const TCHAR* inbuffer, Error& error );

				/**
				* Append the specified character buffer to this string.
				* @param str			The character buffer to be appended
				* @param error		An Error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				void ConcatString( const TCHAR* str, Error& error );

				/**
				* Get a pointer to the internal buffer of this string.
				* @reqid 001.0063
				*/
				const TCHAR* GetString() const;

				/**
				* Get the length of this string.
				* @reqid 001.0063
				*/
				int GetLength() const;

				/**
				* Change the character value of an element of the internal buffer.
				* to the specified character.
				* @param character	The new character value
				* @param location		The index of the element of the internal buffer to change
				* @reqid 001.0063
				*/
				void ChangeCharacter( TCHAR character, int location );

				/**
				* Determine whether this string matches a specified string.
				*
				* @warning This method does not provide linguistically sensitive comparison
				* in Unicode mode. It requires the strings to be in the in the precomposed form
				* for the comparison to be accurate. The case-insensitive form is never linguistically
				* sensitive. If you need linguistically sensitive comparison please use the
				* TextProcessor StringComparer class hierarchy.
				* @see StringComparer in TextProcessor
				*
				* @param str the string to compare internal string to
				* @param ignorecase		A flag indicating whether to perform a case insensitive comparison
				* @return true if the strings are equal
				* @reqid 001.0063
				*/
				bool SameStringAs( const String& str, bool ignorecase =
						false ) const;

				/**
				* Retrieve the requested substring contained in the string. Indexing from 1 (-1 if indexing from the end of the string)
				* @param startindex					The start index of the requested substring. If startindex is negative, the substring
				*									is indexed from the end of the string. 0 value not allowed.
				* @param substringlength			The length of the requested substring
				* @param subString					The address of the string to be populated with the requested substring
				* @param error						An Error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				void SubString( int startindex, int substringlength, String* subString, Error &error ) const;

				/**
				* Perform an in-place trimming of the string, removing all whitespace on both sides of the string.
				* @param error						An Error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				void Trim(Error& error);

				/**
				* Perform an in-place trimming of the string, removing all instances of the
				* specified trim string from the specified side(s) of the string.
				* @param trimstring		The string to be removed
				* @param error				An Error object to be populated on allocation failure
				* @param stringside		The side of sides from the trim strings are to be removed
				* @reqid 001.0063
				*/
				void Trim(const String& trimstring, StringSide stringside, Error &error );

				/**
				* Perform an in-place trimming of the string, removing all instances of the
				* specified trim strings from the specified side(s) of the string.
				* @param trimstrings	A collection of strings to be removed
				* @param error				An Error object to be populated on allocation failure
				* @param stringside		The side of sides from the trim strings are to be removed
				* @reqid 001.0063
				*/
				void Trim(LinkedList<String> &trimstrings, StringSide stringside, Error &error );

				/**
				* Perform an in-place removal of the string, removing all instances of the
				* specified remove string and compacting the string.
				* @param removestring	The string to be removed
				* @reqid 001.0063
				*/
				void Remove( const String& removestring );

				/**
				* Search for a character within the string
				*
				* @warning comparison is not linguistically sensitive.
				* See TextProcessor StringComparer classes.
				*
				* @param character		what to search for
				* @param startPos			at which 1-based index to begin search
				* @param ignoreCase		true to search disregarding case
				* @return the 1-based (to make it consistent with SubString method)
				*  index at which the character can be found or -1 if not there
				*/
				int Find( const TCHAR character, int startPos = 1,
					bool ignoreCase = false );

				/**
				* Perform an in-place modification of the string, replacing all instances of the
				* specified string to replace with the specified replacement string.
				* @param stringtoreplace		The string to be replaced
				* @param replacestring			The replacement string
				* @param error							An Error object to be populated on allocation failure
				* @reqid 001.0063
				*/
				void Replace( const TCHAR* stringtoreplace,
					const TCHAR* replacestring, Error& error );

				/**
				 * Assemble a formatted string using a specified format string and arguments.
				 * @param maxLength maximum length of resulting string including terminating NULL
				 *  If length is exceeded the string will be cut to max length.
				 * @param formattedString	[out] address of the resulting string
				 * @param error						an Error object
				 * @param format					format string (standard C-style)
				 * @param ... arguments
				 */
				static void Format( size_t maxLength, String* formattedString, Error& error, const TCHAR* format, ... );

				/**
				* Create an upper case version of the string.
				*
				* @warning This method does not provide linguistically sensitive conversion
				* in Unicode mode. If you need linguistically sensitive comparison please use the
				* TextProcessor CaseCoverter class hierarchy.
				* @see CaseCoverter in TextProcessor
				*
				* @return an upper case version of the string
				* @reqid 001.0063
				*/
				String& Ucase();

				/**
				* Create a lower case version of the string.
				*
				* @warning This method does not provide linguistically sensitive conversion
				* in Unicode mode. If you need linguistically sensitive comparison please use the
				* TextProcessor CaseCoverter class hierarchy.
				* @see CaseCoverter in TextProcessor
				*
				* @return a lower case version of the string
				* @reqid 001.0063
				*/
				String& Lcase();

				/**
				* Create a proper case version of the string.
				*
				* @warning This method does not provide linguistically sensitive conversion
				* in Unicode mode. If you need linguistically sensitive comparison please use the
				* TextProcessor CaseCoverter class hierarchy.
				* @see CaseCoverter in TextProcessor
				*
				* @return a proper case version of the string
				* @reqid 001.0063
				*/
				String& Pcase();

				/**
				* Converts string to an integer and returns it.
				*
				* @return integer form of string
				* @reqid 001.0063
				*/
				int ToInteger() const;

				/**
				* Determines if input string is a valid integer
				*
				* @param str - string to test
				* @return true->it is an integer, false->it isn't an integer
				* @reqid 001.0063
				*/
				bool IsValidInteger(const TCHAR *str) const;

				/**
				* Determines if this string is an integer
				*
				* @return true->it is an integer, false->it isn't an integer
				* @reqid 001.0063
				*/
				bool IsValidInteger() const;

				/**
				* Determines if this string is a float
				*
				* @param str - string representing a float whose validity is to be checked
				* @return true->it is a float, false->it isn't a float
				* @reqid 001.0063
				*/
				bool IsValidFloat(const TCHAR *str) const;

				/**
				* Determines if the stored string is a float
				*
				* @return true->it is a float, false->it isn't a float
				* @reqid 001.0063
				*/
				bool IsValidFloat() const;

				/**
				* Convert stored string into a float
				*
				* @return string converted to float
				* @reqid 001.0063
				*/
				float ToFloat() const;

#ifdef S11N
				/**
				 * Serialization.
				 * @param emitter - the emitter to serialize to.
				 * @param error - error info.
				 * @reqid N/A
				 */
				void Serialize( ISerializer& emitter, Error& error ) const;

				/**
				 * Deserializes the object from a deserializer.
				 * @param deserializer - Source for deserialization.
				 * @param error - Error info.
				 * @reqid N/A
				 */
				void Deserialize( IDeserializer& deserializer, Error& error );
#endif

			private:
				/**
				* Report whether the specified code point represents an alpha character.
				*
				* @warning This method employs the current locale
				*
				* @return true if an alpha character; false if not
				* @reqid 001.0063
				*/
				bool IsAlpha( TCHAR uc );

				/**
				* Convert the specified character to upper case.
				*
				* @warning This method employs the current locale
				*
				* @return the upper case equivalent of the specified character
				* @reqid 001.0063
				*/
				TCHAR ConvertToUpper( TCHAR uc );

				/**
				* Convert the specified character to lower case.
				*
				* @warning This method employs the current locale
				*
				* @return the lower case equivalent of the specified character
				* @reqid 001.0063
				*/
				TCHAR ConvertToLower( TCHAR uc );

				TCHAR* m_buffer;
				bool m_stringowned;
		};
	} // namespace Dtp
} // namespace MapText

#endif // DTP_MTSTRING_H
