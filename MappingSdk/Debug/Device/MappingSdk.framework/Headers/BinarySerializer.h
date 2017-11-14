/**************************************************
 * Copyright (c)2011 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/MapTextTypes/BinarySerializer.h $
 * 
 * Revision: $Revision: 79324 $
 *
 * Last modified on: $Date: 2015-12-09 13:03:07 -0500 (Wed, 09 Dec 2015) $, by: $Author: mfraczak $
 * 
 *************************************************/

#ifndef DTP_BINARYSERIALIZER_H
#define DTP_BINARYSERIALIZER_H

#include "MapTextDefs.h"
#include "Serialization.h"
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <utility>

// #define DIAGNOSTICS

#ifdef bool
#   undef bool
#endif

namespace MapText
{
	namespace Dtp
	{

		enum BlockType
		{
			BlockType_Bool,
			BlockType_Int,
			BlockType_UnsignedInt,
			BlockType_Short,
			BlockType_UnsignedShort,
			BlockType_Long,
			BlockType_UnsignedLong,
			BlockType_LongLong,
			BlockType_UnsignedLongLong,
			BlockType_Float,
			BlockType_Double,
			BlockType_UnsignedChar,
			BlockType_Char,
			BlockType_Chapter,
			BlockType_List,
			BlockType_Object,
			BlockType_Null
		};

		struct ContainerHeader
		{
			char ContainerType;
			int BlockSize;
			int ListCount;
#ifdef DIAGNOSTICS
			TCHAR ContainerName[50];
#endif
		};

		struct ComplexTypeHeader
		{
			char ComplexType;
			unsigned int Size;
		};

		typedef std::pair<ContainerHeader, int> AnchoredHeader;

		/**
		 * Implements the bookmark for binary serializer
		 */
		class MAPTEXTTYPES_API BinarySerializerBookmark : public IBookmark
		{
		public:

			BinarySerializerBookmark( int filePosition )
				: m_filePosition( filePosition )
			{
			}

			/**
			* Gets FilePosition
			* @return - FilePosition
			* @reqid .
			*/
			int GetFilePosition() const
			{ 
				return m_filePosition; 
			}

			/**
			* Sets FilePosition
			* @param val - FilePosition
			* @reqid .
			*/
			void SetFilePosition(int val)
			{ 
				m_filePosition = val; 
			}

			/**
			 * Serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void Serialize( ISerializer& emitter, Error& error ) const;

			/**
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void Deserialize( IDeserializer& deserializer, Error& error );
		private:
			int m_filePosition;
		};


		class MAPTEXTTYPES_API BinarySerializer : public ISerializer
		{
		public:
			enum ErrorCode
			{
				ErrorCode_VersionInvalid = 1,
			};

			BinarySerializer( const TCHAR* fileCore, bool append, float versionNumber, Error& error );

			/**
			 * Format version must be indentifiable.
			 * @return - the format version as a float
			 * @reqid N/A.
			 */
			virtual float GetFormatVersion()
			{
				return m_formatVersionNumber;
			}

			virtual ISerializer* CreateInstance( const TCHAR* fileNameCore, bool append, float versionNumber, Error& error );

#ifdef S11N_STAT
#   define NAME_FOR_STATS name
#else
#   define NAME_FOR_STATS
#endif

			virtual void Serialize( bool v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("bool"));
#endif

				EmitValueType<bool>(v, BlockType_Bool, error );
			}

			virtual void Serialize( int v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("int"));
#endif
				EmitValueType<int>(v, BlockType_Int, error);
			}

			virtual void Serialize( unsigned int v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("unsigned int"));
#endif
				EmitValueType<unsigned int>(v, BlockType_UnsignedInt, error);
			}

			virtual void Serialize( short v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("short"));
#endif
				EmitValueType<short>(v, BlockType_Short, error);
			}

			virtual void Serialize( unsigned short v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("unsigned short"));
#endif
				EmitValueType<unsigned short>(v, BlockType_UnsignedShort, error);
			}

			virtual void Serialize( long v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("long"));
#endif
				EmitValueType<long>(v, BlockType_Long, error);
			}

			virtual void Serialize( unsigned long v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("unsigned long"));
#endif
				EmitValueType<unsigned long>(v, BlockType_UnsignedLong, error);
			}

			virtual void Serialize( long long v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("long long"));
#endif
				EmitValueType<long long>(v, BlockType_LongLong, error);
			}

			virtual void Serialize( unsigned long long v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("unsigned long long"));
#endif
				EmitValueType<unsigned long long>(v, BlockType_UnsignedLongLong, error);
			}

			virtual void Serialize( float v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("float"));
#endif
				EmitValueType<float>(v, BlockType_Float, error);
			}

			virtual void Serialize( double v, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("double"));
#endif
				EmitValueType<double>(v, BlockType_Double, error);
			}

			virtual void Serialize( unsigned char* v, unsigned int length, const TCHAR* NAME_FOR_STATS, Error& error )
			{
#ifdef S11N_STAT
				SerializationStat::Record statRecord(0, NAME_FOR_STATS, _T("unsigned char*"));
#endif
				EmitComplexType( (void*)v, BlockType_UnsignedChar, length, error );
				if ( error )
				{
					return;
				}
			}

			virtual void Serialize( const char *v, const TCHAR* name, Error& error );

			virtual void StartChapter( Error& error );
			virtual void EndChapter( Error& error );

			virtual void StartContainer( Error& error );
			virtual void EndContainer( Error& error );

			virtual void StartList( const TCHAR* name, int numElements, Error& error );
			virtual void EndList( Error& error );

			virtual void SaveNull( const TCHAR*name, Error& error );

			virtual void SaveToFile( Error& error );

			virtual void Reset( const TCHAR* fileName, bool append, Error& error );

			/**
			 * @see IDeserializer::GetBookmark
			 * @reqid N/A.
			 */
			virtual IBookmark* GetBookmark( Error& error );

			/**
			 * @see ISerializer::GetFileExtension
			 * @reqid N/A.
			 */
			virtual const TCHAR* GetFileExtension()
			{
				return "bin";
			}

		protected:

			virtual void StartNestedObject( const TCHAR* name, Error& error );
			virtual void EndNestedObject( Error& error );

		private:

			inline void EmitComplexType( void* v, BlockType type, int size, Error& error )
			{
				ComplexTypeHeader header;
				header.ComplexType = (char)type;
				header.Size = size;

				EmitValue( (void*)&header, sizeof( ComplexTypeHeader ), error );
				if ( error )
				{
					return;
				}

				if ( size > 0 )
				{
					EmitValue( (void*)v, size, error );
					if ( error )
					{
						return;
					}
				}
			}

			template< typename T>
			inline void EmitValueType( T v, BlockType, Error& error )
			{
				EmitValue( (void*)&v, sizeof(T), error );
				if ( error )
				{
					return;
				}
			}

			/**
			 * Emits a value type of a given type.
			 * @param v - The value to emit.
			 * @param length - Size of data in bytes.
			 * @param error[out] - Error info.
			 * @reqid N/A.
			 */
			inline void EmitValue( void* v, unsigned int length, Error& error )
			{
				assert( m_output != NULL );

				if ( fwrite( (void*)v, 1, length, m_output ) < length )
				{
					SET_ERROR( error, ErrorCode_SerializationFailed, "Buffer underwrite." );
					return;
				}
			}

			inline void PushHeader( ContainerHeader& header )
			{
				m_path.push( std::make_pair( header, (int)ftell( m_output ) ) );
			}

			inline void SaveTopHeader( Error& error )
			{

				// Save location
				int tempPos = (int)ftell( m_output );
				int size = tempPos - m_path.top().second;

				m_path.top().first.BlockSize = size-sizeof(ContainerHeader);

				// Move to where header was first saved
				fseek( m_output, m_path.top().second, SEEK_SET );

				// Overwrite it with updated values
				EmitValue( (void*)&(m_path.top().first), sizeof(ContainerHeader), error );
				if ( error )
				{
					return;
				}

				// Go back to previous position
				fseek( m_output, tempPos, SEEK_SET );
			}

			FILE* m_output;
			int m_chapterBookmark;
			std::stack<AnchoredHeader> m_path;
			std::map<std::string, unsigned int> m_fileStringDictionary;
			float m_formatVersionNumber;
		};

		class MAPTEXTTYPES_API BinaryDeserializer : public IDeserializer
		{
		public:

			enum ErrorCode
			{
				ErrorCode_VersionInvalid = 1,
				ErrorCode_NotImplemented
			};

			BinaryDeserializer( Map& ownerMap );
            
            ~BinaryDeserializer()
            {
                if ( m_input != NULL )
                {
                    fclose( m_input );
                    m_input = NULL;
                }
                
                m_stringsDictionary.clear();
                free( m_stringsBuffer );
            }

			/**
			 * Format version must be indentifiable.
			 * @return - the format version as a float
			 * @reqid N/A.
			 */
			virtual float GetFormatVersion()
			{
				return m_formatVersionNumber;
			}

			virtual IDeserializer* CreateInstance( Error& error ) const;

			virtual void SetSource( const TCHAR* baseFileName, Error& error );

			virtual void Deserialize( bool& v, const TCHAR*, Error& error )
			{
				ReadValueType<bool>(v, BlockType_Bool, error );
			}

			virtual void Deserialize( int& v, const TCHAR*, Error& error )
			{
				ReadValueType<int>(v, BlockType_Int, error);
			}

			virtual void Deserialize( unsigned int& v, const TCHAR*, Error& error )
			{
				ReadValueType<unsigned int>(v, BlockType_UnsignedInt, error);
			}

			virtual void Deserialize( short& v, const TCHAR*, Error& error )
			{
				ReadValueType<short>(v, BlockType_Short, error);
			}

			virtual void Deserialize( unsigned short& v, const TCHAR*, Error& error )
			{
				ReadValueType<unsigned short>(v, BlockType_UnsignedShort, error);
			}

            virtual void Deserialize( long& v, const TCHAR*, Error& error )
            {
                int oldLong = 0;
                ReadValueType<int>(oldLong, BlockType_Int, error);
                v = oldLong;
            }
            
            virtual void Deserialize( unsigned long& v, const TCHAR*, Error& error )
            {
                unsigned int oldULong = 0;
                ReadValueType<unsigned int>(oldULong, BlockType_UnsignedInt, error);
                v = oldULong;
            }

			virtual void Deserialize( long long& v, const TCHAR*, Error& error )
			{
				ReadValueType<long long>(v, BlockType_LongLong, error);
			}

			virtual void Deserialize( unsigned long long& v, const TCHAR*, Error& error )
			{
				ReadValueType<unsigned long long>(v, BlockType_UnsignedLongLong, error);
			}

			virtual void Deserialize( float& v, const TCHAR*, Error& error )
			{
				ReadValueType<float>(v, BlockType_Float, error);
			}

			virtual void Deserialize( double& v, const TCHAR*, Error& error )
			{
				ReadValueType<double>(v, BlockType_Double, error);
			}

			virtual void Deserialize( const TCHAR** v, const TCHAR* name, Error& error );

			virtual void Deserialize( unsigned char* v, int length, const TCHAR* name, Error& error );

			virtual void Deserialize( const char*, const TCHAR*, Error& error )
			{
				SET_ERROR_NOMSG( error, ErrorCode_NotImplemented );
			}

			virtual bool EnterChapter( Error& error );
			virtual void LeaveChapter( Error& error );

			virtual void EnterNestedObject( const TCHAR* name, Error& error );
			virtual void LeaveNestedObject( Error& error );

			virtual bool EnterFirstNestedObject( std::basic_string<TCHAR>& name, Error& error );
			virtual bool EnterNextNestedObject( std::basic_string<TCHAR>& name, Error& error );

			virtual bool EnterFirstListItem( Error& error );
			virtual bool EnterNextListItem( Error& error );

			virtual unsigned int EnterList( const TCHAR* name, Error& error );
			virtual void LeaveList( Error& error );

			virtual void EnterContainer( Error& error );
			virtual void LeaveContainer( Error& error );

			virtual void SkipNestedObject( Error& error );
			virtual void SkipList( Error& error );

			/**
			 * @see IDeserializer::JumpToBookmark
			 * @reqid N/A.
			 */
			virtual void JumpToBookmark( IBookmark* bookmark, Error& error );

			/**
			 * @see IDeserializer::DeserializeNewBookmark
			 * @reqid N/A.
			 */
			virtual IBookmark* DeserializeNewBookmark( const TCHAR* name, Error& error );

			/**
			 * @see IDeserializer::GetFileExtension
			 * @reqid N/A.
			 */
			virtual const TCHAR* GetFileExtension()
			{
				return "bin";
			}

		protected:
			virtual bool IsNull();

			template< typename T>
			inline void ReadValueType( T& v, BlockType, Error& error )
			{
				ReadValue( (void*)&v, sizeof(T), error );
				if ( error )
				{
					return;
				}
			}

			inline void ReadValue( void* v, unsigned int length, Error& error )
			{
				assert( m_input != NULL );

				if ( (unsigned int)fread( v, 1, length, m_input ) < length )
				{
					SET_ERROR( error, ErrorCode_DeserializationFailed, "Buffer underead." );
					return;
				}
			}

		private:
			FILE* m_input;
			int m_currentChapterEnd;
			std::stack<ContainerHeader> m_path;
			std::stack<int> m_iterationCount;
			std::vector<char*> m_stringsDictionary;
            char* m_stringsBuffer;
			int m_dictionaryPosition;
			float m_formatVersionNumber;
            std::string m_sourcePath;
			// This class is reused to some extent by the serializer in append mode.
			friend class BinarySerializer;
		};

	} // namespace Dtp
} // namespace MapText

#endif // DTP_BINARYSERIALIZER_H