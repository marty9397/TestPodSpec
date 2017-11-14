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

#ifndef DTP_SERIALIZATION_H
#define DTP_SERIALIZATION_H

#ifndef S11N
#   define S11N
#endif

//Uncomment to enable serialization statistics
// #ifndef S11N_STAT
// #define S11N_STAT
// #endif

#ifdef S11N

#include "MapTextDefs.h"
#include "Error.h"
#include <stack>
#include <map>
#include <string>

/**
 * File format history:
 * TODO mfraczak: Document the earlier versions
 * ----------------------------
 * 1.4
 * - PointSymbol no longer serializes cached values
 */

/**
 * List of the formats we are still able to generate
 */
#define SUPPORTED_SERIALIZATION_FORMAT_VERSIONS_COUNT 4
static const float s_supportedSerializarionFormatVersions[SUPPORTED_SERIALIZATION_FORMAT_VERSIONS_COUNT] 
	= { 2.0f, 2.1f, 2.2f, 2.3f };

/**
 * List of the formats we are still able to read
 */
#define SUPPORTED_DESERIALIZATION_FORMAT_VERSIONS_COUNT 4
static const float s_supportedDeserializarionFormatVersions[SUPPORTED_DESERIALIZATION_FORMAT_VERSIONS_COUNT] 
	= { 2.0f, 2.1f, 2.2f, 2.3f };

#define CURRENT_FORMAT_VERSION 2.3f

namespace MapText
{
	namespace Dtp
	{

#ifdef _MSC_VER
#	pragma warning( push )
		// Disable useless warning
#	pragma warning( disable: 4251 )
#endif
		/**
		 * Forward declarations
		 */
		class Map;
		template < class C > class LinkedList;

#ifdef S11N_STAT
		/**
		 * Size statistics generator for serialization (singleton).
		 * It generates two files with the following information:
		 * - size of every record saved in the serialization file (stat file)
		 * - total size per object type (aggregated file)
		 * - total count and total size per object id (aggregated file)
		 */
		class MAPTEXTTYPES_API SerializationStat
		{
		public:
			/**
			 * Get singleton instance.
			 * @return singleton instance
			 */
			static SerializationStat& GetInstance()
			{
				if (SerializationStat::s_instance == NULL)
				{
					SerializationStat::s_instance = new SerializationStat();
					assert(SerializationStat::s_instance);
				}
				return *SerializationStat::s_instance;
			}

			/**
			 * Set serialization file for which statistics should be gathered.
			 * @param serializationFile serialization file stream
			 */
			void SetSerializationFile(FILE* serializationFile);

			/**
			 * Initialize statistics.
			 * @param statFileName name of output file that contains information for every object saved in the serialization file
			 * @param resultFileName name of output file that contains aggregated results
			 * @param error error information
			 */
			void Init(const TCHAR* statFileName, const TCHAR* resultFileName, Error& error);

			/**
			 * Close statistics files.
			 */
			void Close();

			/**
			 * Statistics record. 
			 * Instantiate this class before object is serialized and destroy it once serialization of the object is complete.
			 * E.g.
			 * <code>
			 *    Object object;
			 *    {
			 *	      SerializationStat::Record statRecord(&object, object.GetName(), "Object");
			 *        object.Serialize();
			 *    } //statRecord gets destroyed and the serialization statistics record is saved to file
			 * </code>
			 */
			class MAPTEXTTYPES_API Record
			{
			public:
				/**
				 * Create statistics record and save current stream position. 
				 * @param 
				 * @return
				 */
				Record(const void *objectID, const TCHAR* objectName, const TCHAR* typeName);

				/**
				 * Destroy record. Save stream position to calculate size.
				 * Write out information to statistics file.
				 */
				~Record();

				/**
				 * Explicitly write out information to statistics file.
				 */
				void SaveRecord();

			private:
				bool m_saved;
				long m_startPos;
				uintptr_t m_objectId;
				TCHAR m_objectName[20];
				TCHAR m_typeName[20];
				SerializationStat* m_serializationStat;
			};
			
			/**
			 * Statistics holder.
			 */
			struct ObjectStatRecord
			{
				long count;
				size_t size;
				TCHAR typeName[20];
				ObjectStatRecord()
					: count(0), size(0)
				{
					typeName[0] = _T('\0');
				};
			};

			typedef std::map< std::basic_string<TCHAR>, long > SizePerTypeMap;
			typedef std::map< uintptr_t, ObjectStatRecord> CountPerObjectMap;

			enum ErrorCode
			{
				ErrorCode_SerializationStatFailed = 1,
			};

		private:
			/**
			 * Private constructor to enforce singleton.
			 */
			SerializationStat()
				: m_statFile(NULL), m_resultFile(NULL), m_serializationFile(NULL)
			{
				m_statFileName[0] = _T('\0');
				m_resultFileName[0] = _T('\0');
			};

			/**
			 * Generate aggregated results.
			 */
			void GenerateResultFile();

			static SerializationStat* s_instance;

			FILE* m_statFile;
			FILE* m_resultFile;
			TCHAR m_statFileName[_MAX_PATH];
			TCHAR m_resultFileName[_MAX_PATH];
			FILE* m_serializationFile;
			SizePerTypeMap m_sizePerType;
			CountPerObjectMap m_countPerObject;

			class CountPerObjectCompare
			{
			public:
				bool operator() (const CountPerObjectMap::value_type& x, const CountPerObjectMap::value_type& y) const
				{
					return x.second.size < y.second.size;
				}
			};
		};
#   define STAT_ONLY_TYPENAME typeNameArgument
#   define STAT_ONLY_NAME nameArgument
#else
#   define STAT_ONLY_TYPENAME
#   define STAT_ONLY_NAME
#endif //S11N_STAT

		/**
		 * Forward declarations
		 */
		 class ISerializer;
		 class IDeserializer;

		/**
		 * A bookmark which allows to save a position within the serialization media
		 * and later use it to "jump" to the same position in deserializer.
		 */
		class MAPTEXTTYPES_API IBookmark
		{
		public:
			virtual ~IBookmark()
			{
			}

			/**
			 * @{ Serialization: the bookmark itself also needs to be serializable.
			 */

			/**
			 * Serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			virtual void Serialize( ISerializer& emitter, Error& error ) const = 0;

			/**
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			virtual void Deserialize( IDeserializer& deserializer, Error& error ) = 0;

			/**
			 * }@
			 */
		};

		class MAPTEXTTYPES_API ISerializer
		{
		public:

			ISerializer()
			{
			}

			virtual ~ISerializer()
			{
			}

			/**
			 * Format version must be indentifiable.
			 * @return - the format version as a float
			 * @reqid N/A.
			 */
			virtual float GetFormatVersion() = 0;

			/**
			 * Factory method to allow spawning new instances of the serializer
			 * inside client code without deciding on any particular implementation.
			 * @param fileNameCore - core of the filename which should be the output if this serializer.
			 * @param versionNumber - which file version number to use. Caution: this should almmost always be
			 *                        the CURRENT_FORMAT_VERSION, unless you really understand what you're doing.
			 * @param error [out] - error info.
			 * @reqid N/A.
			 */
			virtual ISerializer* CreateInstance( const TCHAR* fileNameCore, bool append, float versionNumber, Error& error ) = 0;

			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
				ErrorCode_SerializationFailed,
			};

			template<class C>
			void SerializeObject( const C& object, const TCHAR* name, const TCHAR* STAT_ONLY_TYPENAME, Error& error )
			{
				if ( (C*)(&object) == NULL )
				{
					SaveNull( name, error );
					if ( error )
					{
						return;
					}
				}
				else
				{
#ifdef S11N_STAT
					SerializationStat::Record statRecord((const void *)&object, name, STAT_ONLY_TYPENAME);
#endif

					StartNestedObject( name, error );
					if ( error )
					{
						return;
					}

					object.Serialize( *this, error );
					if ( error )
					{
						return;
					}
					EndNestedObject( error );
					if ( error )
					{
						return;
					}
				}
			}

			template<class C>
			void SerializeArray( const C* array, int length, const TCHAR* name, const TCHAR* STAT_ONLY_TYPENAME, 
				Error& error )
			{
				if ( array == NULL )
				{
					SaveNull( name, error );
					if ( error )
					{
						return;
					}
				}
				else
				{
#ifdef S11N_STAT
					SerializationStat::Record statRecord((const void *)array, name, STAT_ONLY_TYPENAME);
#endif
					StartList( name, length, error );
					if ( error )
					{
						return;
					}

					for ( int i = 0; i < length; i++ )
					{
						StartContainer( error );
						if ( error )
						{
							return;
						}

						array[i].Serialize( *this, error );
						if ( error )
						{
							return;
						}

						EndContainer( error );
						if ( error )
						{
							return;
						}
					}

					EndList( error );
					if ( error )
					{
						return;
					}
				}
			}

			template<class C>
			void SerializeList( const LinkedList<C>& list, const TCHAR* name, const TCHAR* STAT_ONLY_TYPENAME, 
				Error& error )
			{
				if ( (LinkedList<C>*)(&list) == NULL )
				{
					SaveNull( name, error );
					if ( error )
					{
						return;
					}
				}
				else
				{
#ifdef S11N_STAT
					SerializationStat::Record statRecord((const void *)&list, name, STAT_ONLY_TYPENAME);
#endif
					StartList( name, list.Count(), error );
					if ( error )
					{
						return;
					}

					for ( typename LinkedList<C>::ConstIterator it = list.Begin(); !it.IsNull(); it++ )
					{
						StartContainer( error );
						if ( error )
						{
							return;
						}

						it->Serialize( *this, error );
						if ( error )
						{
							return;
						}

						EndContainer( error );
						if ( error )
						{
							return;
						}
					}

					EndList( error );
					if ( error )
					{
						return;
					}
				}
			}

			/**
			 * @{ To be implemented in the derived classes
			 */
			virtual void Serialize( bool v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( const char *v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( int v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( unsigned int v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( short v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( unsigned short v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( long v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( unsigned long v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( long long v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( unsigned long long v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( float v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( double v, const TCHAR* name, Error& error ) = 0;
			virtual void Serialize( unsigned char* buffer, unsigned int length, const TCHAR* name, Error& error ) = 0;

			virtual void StartChapter( Error& error ) = 0;
			virtual void EndChapter( Error& error ) = 0;

			virtual void StartContainer( Error& error ) = 0;
			virtual void EndContainer( Error& error ) = 0;

			virtual void StartList( const TCHAR* name, int numElements, Error& error ) = 0;
			virtual void EndList( Error& error ) = 0;

			virtual void SaveNull( const TCHAR*name, Error& error ) = 0;
			virtual void SaveToFile( Error& error ) = 0;
			virtual void Reset( const TCHAR* fileName, bool append, Error& error ) = 0;

			/**
			 * Create and return a bookmark which then can be used during 
			 * deserialization to quickly position yourself in a specific 
			 * location of the serialization media and retrieve an element.
			 * @param error [out] - Error info
			 * @return - The bookmark or NULL if an error occured.
			 * @reqid N/A.
			 */
			virtual IBookmark* GetBookmark( Error& error ) = 0;

			/**
			 * Return the file extension used by the serializer.
			 * @reqid N/A.
			 */
			virtual const TCHAR* GetFileExtension() = 0;

		protected:
			virtual void StartNestedObject( const TCHAR* name, Error& error ) = 0;
			virtual void EndNestedObject( Error& error ) = 0;

			/**
			 * }@
			 */
		};

		class MAPTEXTTYPES_API IDeserializer
		{
		public:

			IDeserializer( Map& ownerMap )
				: m_ownerMap( &ownerMap )
			{
			}

			virtual ~IDeserializer()
			{}

			/**
			 * Format version must be indentifiable.
			 * @return - the format version as a float
			 * @reqid N/A.
			 */
			virtual float GetFormatVersion() = 0;

			/**
			 * Factory method to allow spawning new instances of the deserializer
			 * inside client code without deciding on any particular implementation.
			 * @param ownerMap -
			 * @param error -
			 * @return -
			 * @reqid N/A.
			 */
			virtual IDeserializer* CreateInstance( Error& error ) const = 0;

			enum ErrorCode
			{
				ErrorCode_DeserializationFailed = 1,
				ErrorCode_AllocationFailure,
			};

			virtual void SetSource( const TCHAR* baseFileName, Error& error ) = 0;

			virtual void Deserialize( bool& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( const TCHAR** v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( unsigned char* buffer, int length, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( int& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( unsigned int& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( short& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( unsigned short& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( long& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( unsigned long& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( long long& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( unsigned long long& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( float& v, const TCHAR* name, Error& error ) = 0;
			virtual void Deserialize( double& v, const TCHAR* name, Error& error ) = 0;

			//virtual void Start( Error& error ) = 0;
			//virtual void Done( Error& error ) = 0;

			Map* GetOwnerMap()
			{
				return m_ownerMap;
			}

			/**
			 * Set the map which should be assumed to be the owner map of deserialized objects.
			 * Ownership is not taken.
			 * @reqid N/A.
			 */
			void SetOwnerMap( Map& ownerMap )
			{
				m_ownerMap = &ownerMap;
			}
            
            std::string& GetFontResourcePath()
            {
                return m_fontResourcePath;
            }
            
            void SetFontResourcePath(const char* fontResourcePath)
            {
                m_fontResourcePath = fontResourcePath;
            }

			template<class C>
			void DeserializeOptionalObject( C** object, const TCHAR* name, Error& error )
			{
				EnterNestedObject( name, error );
				if ( error )
				{
					return;
				}

				if ( IsNull() )
				{
					*object = NULL;
					LeaveNestedObject( error );
					if ( error )
					{
						return;
					}
					return;
				}

				if ( *object == NULL )
				{
					*object = C::DeserializeNew( *this, error );
				}
				else
				{
					(*object)->Deserialize( *this, error );
					if ( error )
					{
						return;
					}
				}

				LeaveNestedObject( error );
				if ( error )
				{
					return;
				}
			}

			template<class C>
			void DeserializeObject( C& object, const TCHAR* name, Error& error )
			{
				EnterNestedObject( name, error );
				if ( error )
				{
					return;
				}

				if ( IsNull() )
				{
					SET_ERROR( error, ErrorCode_DeserializationFailed, _T( "Unexpected NULL." ) );
					return;
				}

				object.Deserialize( *this, error );
				if ( error )
				{
					return;
				}

				LeaveNestedObject( error );
				if ( error )
				{
					return;
				}
			}

			template<class Collection, class Value>
			void DeserializeOptionalList( Collection** list, const TCHAR* name, Error& error )
			{
				EnterList( name, error );
				if ( error )
				{
					return;
				}

				if ( IsNull() )
				{
					*list = NULL;
					LeaveList( error );
					if ( error )
					{
						return;
					}
					return;
				}

				if ( *list == NULL )
				{
					*list = new Collection();
					if ( *list == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return;
					}
				}

				DeserializeListItems<Collection, Value>( **list, error );
				if ( error )
				{
					return;
				}

				LeaveList( error );
				if ( error )
				{
					return;
				}
			}

			template<class Collection, class Value>
			void DeserializeList( Collection& list, const TCHAR* name, Error& error )
			{
				EnterList( name, error );
				if ( error )
				{
					return;
				}

				if ( IsNull() )
				{
					SET_ERROR( error, ErrorCode_DeserializationFailed, _T( "Unexpected NULL." ) );
					return;
				}

				DeserializeListItems<Collection, Value>( list, error );
				if ( error )
				{
					return;
				}

				LeaveList( error );
				if ( error )
				{
					return;
				}
			}

			template<class C>
			void DeserializeArray( C** array, int* length, const TCHAR* name, Error& error )
			{
				*length = EnterList( name, error );
				if ( error )
				{
					return;
				}

				if ( IsNull() )
				{
					*array = NULL;
					*length = 0;
					LeaveList( error );
					if ( error )
					{
						return;
					}
					return;
				}

				*array = new C[*length];
				if ( (*array) == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
					return;
				}

				bool ret = EnterFirstListItem( error );
				if ( error )
				{
					return;
				}

				if ( !ret )
				{
					SET_ERROR( error, ErrorCode_DeserializationFailed, "Not enough elements in the list" );
					return;
				}

				for ( int i = 0; i < *length; i++ )
				{
					(*array)[i].Deserialize( *this, error );
					if ( error )
					{
						return;
					}

					LeaveContainer( error );
					if ( error )
					{
						return;
					}

					ret = EnterNextListItem( error );
					if ( error )
					{
						return;
					}

					if ( !ret && i < (*length)-1 )
					{
						SET_ERROR( error, ErrorCode_DeserializationFailed, "Not enough elements in the list" );
						return;
					}
				}

				LeaveList( error );
				if ( error )
				{
					return;
				}
			}

			virtual bool EnterChapter( Error& error ) = 0;
			virtual void LeaveChapter( Error& error ) = 0;

			virtual void EnterNestedObject( const TCHAR* name, Error& error ) = 0;
			virtual void LeaveNestedObject( Error& error ) = 0;

			virtual bool EnterFirstNestedObject( std::basic_string<TCHAR>& name, Error& error ) = 0;
			virtual bool EnterNextNestedObject( std::basic_string<TCHAR>& name, Error& error ) = 0;

			virtual bool EnterFirstListItem( Error& error ) = 0;
			virtual bool EnterNextListItem( Error& error ) = 0;

			virtual unsigned int EnterList( const TCHAR* name, Error& error ) = 0;
			virtual void LeaveList( Error& error ) = 0;

			virtual void EnterContainer( Error& error ) = 0;
			virtual void LeaveContainer( Error& error ) = 0;

			virtual void SkipNestedObject( Error& error ) = 0;
			virtual void SkipList( Error& error ) = 0;

			/**
			 * Performs a jump in the serialization media. It should be used with
			 * a lot of caution as it's may not be safe to perform a jump while not
			 * on the top level of the hierarchy (i.e. inside objects, lists and chapters).
			 * Implementation should return an error if a jump is performed from 
			 * a state which would yield unpredicted results.
			 * @param bookmark - The bookmark to jump to.
			 * @param error [out] - Error info.
			 * @reqid N/A.
			 */
			virtual void JumpToBookmark( IBookmark* bookmark, Error& error ) = 0;

			/**
			 * Instantiate the appropriate type of bookmark and deserialize it from the current media.
			 * @param name - Name of the container in which the bookmark is saved in
			 * @param error [out] - error info.
			 * @return - The bookmark or NULL on error.
			 */
			virtual IBookmark* DeserializeNewBookmark( const TCHAR* name, Error& error ) = 0;

			/**
			 * Return the file extension used by the deserializer.
			 * @reqid N/A.
			 */
			virtual const TCHAR* GetFileExtension() = 0;

		protected:
			virtual bool IsNull() = 0;

			template<class Collection, class Value>
			void DeserializeListItems( Collection& list, Error& error )
			{
				bool ret = EnterFirstListItem( error );
				if ( error )
				{
					return;
				}

				while ( ret )
				{
					Value* item = (Value*)Value::DeserializeNew( *this, error );
					if ( error )
					{
						return;
					}

					list.AddElement( item, error );
					if ( error )
					{
						delete item;
						return;
					}

					LeaveContainer( error );
					if ( error )
					{
						return;
					}

					ret = EnterNextListItem( error );
					if ( error )
					{
						return;
					}
				}
			}

		protected:
			Map* m_ownerMap;
            std::string m_fontResourcePath;
		};

		/**
		 * This interface needs to be implemented by all DTP classes which are
		 * serializable.
		 * @reqid N/A.
		 */
		class ISerializable
		{
		public:
			
			/**
			 * Serialize the object using the serializer supplied
			 * @param destination - Implementation of the ISerializer interface.
			 * @param error[out] - Error info.
			 * @reqid N/A.
			 */
			virtual void Serialize( ISerializer& destination, Error& error ) const = 0;
			//virtual ISerializable* DeserializeNew( ISerializer& source, Error& error ) = 0;
			//virtual void Deserialize ( ISerializer& source, Error& error ) = 0;
		};

		
		/////////////////////////////////////////////////
		// Macros to streamline the use of serializer

		// ========= BUILDING PREDICATES FOR VERSION CONTROL
		#	define FORMAT_VER_GTE( worker, version ) \
			( worker.GetFormatVersion() >= version )

		#	define FORMAT_VER_LTE( worker, version ) \
			( worker.GetFormatVersion() <= version )

		#	define PERFORM_WITH_PREDITATE( PREDICATE, ACTION ) \
		if ( PREDICATE ) { ACTION; };
		
		// ========= SCALAR 
		#	define SERIALIZE_SCALAR( x, serializer, error, retval ) \
			(serializer).Serialize( x, #x, error ); \
			if ( error ) return retval;

		#	define SERIALIZE_SCALAR_WITH_NAME( x, name, serializer, error, retval ) \
			(serializer).Serialize( x, name, error ); \
			if ( error ) return retval;

		#	define DESERIALIZE_SCALAR( scalar, deserializer, error, retval ) \
			(deserializer).Deserialize( scalar, #scalar, error ); \
			if ( error ) return retval;

		// For enums we need to do extra casting to &int, so a separate macro was needed
		#	define DESERIALIZE_ENUM( scalar, deserializer, error, retval ) \
			(deserializer).Deserialize( (int&)scalar, #scalar, error ); \
			if ( error ) return retval;

		// ========= OBJECT
		#	define SERIALIZE_OBJECT( x, type, serializer, error, retval ) \
			(serializer).SerializeObject<type>( x, #x, #type, error ); \
			if ( error ) return retval;

		#	define DESERIALIZE_OBJECT( x, type, deserializer, error, retval ) \
			(deserializer).DeserializeObject<type>( x, #x, error ); \
			if ( error ) return retval;

		#	define DESERIALIZE_OPTIONAL_OBJECT( x, type, deserializer, error, retval ) \
			(deserializer).DeserializeOptionalObject<type>( &x, #x, error ); \
			if ( error ) return retval;

		// ========= ARRAY
		#	define SERIALIZE_ARRAY( a, length, type, serializer, error, retval ) \
			(serializer).SerializeArray<type>( a, length, #a, #type, error ); \
			if ( error ) return retval;

		#	define DESERIALIZE_ARRAY( a, length, type, deserializer, error, retval ) \
			(deserializer).DeserializeArray<type>( &a, &length, #a, error ); \
			if ( error ) return retval;


		// ========= COLLECTION
		#	define SERIALIZE_LIST( l, type, serializer, error, retval ) \
			(serializer).SerializeList<type>( l, #l, #type, error ); \
			if ( error ) return retval;

		#	define DESERIALIZE_LIST( l, type, listType, deserializer, error, retval ) \
			(deserializer).DeserializeList<listType, type>( l, #l, error ); \
			if ( error ) return retval;

		#	define DESERIALIZE_OPTIONAL_LIST( l, type, listType, deserializer, error, retval ) \
			(deserializer).DeserializeOptionalList<listType, type>( &l, #l, error ); \
			if ( error ) return retval;


			} // namespace Dtp
		} // namespace MapText

#endif // S11N

#endif