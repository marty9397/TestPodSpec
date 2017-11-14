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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4996)
#endif

#ifndef DTP_SHAREDOBJECTS_H
#define DTP_SHAREDOBJECTS_H

#include "Error.h"
#include "Collection.h"

namespace MapText
{
	namespace Dtp
	{
		/**
		 * A base class representing a reference counted object.
		 * @reqid 001.0063
		 */
		class MAPTEXTTYPES_API RefCountedObject
		{
		public:
			/**
			 * Constructor -- initializes the refernce count.
			 * @reqid 001.0063
			 */
			RefCountedObject() : m_refCount(0)
			{
			}

			/**
			 * Destructor.
			 * @reqid 001.0063
			 */
			virtual ~RefCountedObject() {}

			/**
			 * Increments the instance's reference count.
			 * @reqid 001.0063
			 */
			void IncrementRef()
			{
				++m_refCount;
			}

			/**
			 * Decrements the instance's reference count and
			 * deletes self if goes to 0.
			 * @reqid 001.0063
			 */
			void DecrementRef()
			{
				if ( --m_refCount == 0 )
					delete this;
			}

		protected:
			int m_refCount;
		};

		/**
		 * A smart pointer class that wraps a reference counted object and
		 * facilitates sharing.
		 * @reqid 001.0063
		 */
		template < class T >
		class SharedPointer
		{
		public:
			/**
			* Default constructor.
			* @reqid 001.0063
			*/
			SharedPointer()
			{
				m_rawptr=NULL;
			}

			/**
			 * Constructor that assigns m_rawptr member and calls IncrementRef on it.
			 * @param rawptr	shared instance pointer to be wrapped
			 * @reqid 001.0063
			 */
			SharedPointer( T* rawptr )
			{
				m_rawptr = rawptr;
				m_rawptr->IncrementRef();
			}

			/**
			 * Destructor -- decrements the wrapped instance's reference count.
			 * @reqid 001.0063
			 */
			~SharedPointer()
			{
				if (m_rawptr!=NULL)
					m_rawptr->DecrementRef();
			}

			/**
			 * Copy constructor copies the raw pointer and calls IncrementRef on it.
			 * @param sharedPtr	SharedPointer instance to be copied
			 * @reqid 001.0063
			 */
			SharedPointer( const SharedPointer& sharedPtr )
			{
				m_rawptr =
					const_cast<T*>(sharedPtr.GetRawPointer());
                
                if ( m_rawptr )
                {
                    m_rawptr->IncrementRef();
                }
			}

			/**
			 * Assignment operator.
			 * After ensuring that the two raw pointers are different,
			 * calls DecrementRef on m_rawptr member,
			 * copies raw pointer and increments reference count.
			 * @param sharedPtr	SharedPointer instance to be copied
			 * @reqid 001.0063
			 */
			void operator=( const SharedPointer& sharedPtr )
			{
				if ( &sharedPtr != this )
				{
					m_rawptr->DecrementRef();

					m_rawptr =
						const_cast<T*>(sharedPtr.GetRawPointer());

					m_rawptr->IncrementRef();
				}
			}

			/**
			* Tests equality of wrapped shared instance pointers.
			* @param otherSharedPtr	SharedPointer instance with which to be compared
			* @return true if the wrapped instance pointers are equal in value
			* @reqid 001.0063
			*/
			bool Equals( const SharedPointer& otherSharedPtr ) const
			{
				return ( m_rawptr == otherSharedPtr.GetRawPointer() );
			}

			/**
			* Retrieves the raw pointer wrapped by this smart pointer.
			* @return the wrapped instance pointer
			* @reqid 001.0063
			*/
			T* GetRawPointer() const
			{
				return m_rawptr;
			}

		protected:
			T* m_rawptr;
		};

		/*******************************************************/
		//
		// String related derivation and template specialization.
		//
		/*******************************************************/

		/**
		* A derived reference counted object class that maintains a character buffer.
		* @reqid 001.0063
		*/
		class MAPTEXTTYPES_API RefCountedString : public RefCountedObject
		{
		public:
			/**
			* Possible error codes
			*/
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
				ErrorCode_NotImplemented
			};

			/**
			* Constructor that encapsulates an existing or new cached buffer.
			* @param key		Character buffer to be encapsulated
			* @param error error information
			* @reqid 001.0063
			*/
			RefCountedString ( const TCHAR* key, Error& error )
				: RefCountedObject()
			{
				m_buffer = _tcsdup( key );
				if ( m_buffer == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
				}
			}

			/**
			* Decrements the instance's reference count and
			* removes self from cache ( which calls delete )
			* if reference count goes to 0.
			* @reqid 001.0063
			*/
			void DecrementRef()
			{
				if ( --m_refCount == 0 )
					m_stringCache.RemoveElement(m_buffer);
			}

			/**
			* Destructor.
			* Removes entry from collection and free m_buffer.
			* @reqid 001.0063
			*/
			~RefCountedString ()
			{
				if ( m_buffer != NULL )
				{
					free( m_buffer );
					m_buffer = NULL;
				}
			}

			/**
			* Retrieve the encapsulated character buffer.
			* Return the address of the encapsulated buffer.
			* @reqid 001.0063
			*/
			const TCHAR* GetBuffer() const
			{
				return m_buffer;
			}

			/**
			* A keyed collection class maintains the pool of shared character buffers.
			* @reqid 001.0063
			*/
			class MAPTEXTTYPES_API Collection : public StringKeyedTree < RefCountedString >
			{
			public:
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1,
				};

				/**
				* Looks up the key. If not found, creates new entry in the collection.
				* @param key				The string for which the buffer should be retrieved or created
				* @param rawString	Address of the RefCountedString instance to be retrieved or created
				* @param error			An Error object to be populated
				* @reqid 001.0063
				*/
				void GetRawString(
					const TCHAR* key,
					RefCountedString** rawString,
					Error& error )
				{
					RefCountedString* tempString = NULL;

					if (!Lookup( key, &tempString ))
					{
						tempString = new RefCountedString( key, error );
						if ( error )
						{
							return;
						}

						if (tempString == NULL)
						{
							SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						}
						else
						if (error)
						{
							delete tempString;
							tempString = NULL;
						}
						else
						{
							Insert( key, tempString, error );

							if (error)
							{
								delete tempString;
								tempString = NULL;
							}
						}
					}
					*rawString = tempString;
				}
			};

			/**
			* Retrieve the string cache.
			* Return the address of the string cache.
			* @reqid 001.0063
			*/
			static Collection* GetStringCache()
			{
				return &m_stringCache;
			}

			/**
			* Creates copy of self.
			* @param error an error object to be populated
			* @return copy of self
			* @reqid 001.0076
			*/
			virtual RefCountedString* Clone( Error& error ) const
			{
				RefCountedString* copy = new RefCountedString( m_buffer, error );
				if ( error )
				{
					return NULL;
				}

				if ( copy == NULL )
				{
					SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					return copy;
				}

				return copy;
			}

#ifdef S11N
			/**
			 * Serialization is not used, but MS compiler evaluates all the templates
			 * differently so we need to add this stub of a method.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			void Serialize( ISerializer& emitter, Error& error ) const
			{
				SET_ERROR_NOMSG( error, ErrorCode_NotImplemented );
			}

			/**
			 * Not used, only needed because of MS compiler...
			 * @param node - YAML node to be used for deserialization.
			 * @param output - Deserialized object if no error occured. Otherwise it'll be
			 *                 in an invalid state.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			static RefCountedString* DeserializeNew( IDeserializer& deserializer, Error& error )
			{
				SET_ERROR_NOMSG( error, ErrorCode_NotImplemented );
				return NULL;
			}
#endif

		private:

			static RefCountedString::Collection m_stringCache;

		protected:

			TCHAR* m_buffer;
		};

		/**
		 * A simple class derived from a specialization of the smart pointer
		 * class to simplify syntax and facilitate usage.
		 * @reqid 001.0063
		 */
		class MAPTEXTTYPES_API SharedString : public SharedPointer < RefCountedString >
		{
		public:
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
			};

			/**
			* Collection of shared strings
			* @reqid 001.0063
			*/
			class MAPTEXTTYPES_API Collection : public LinkedList<SharedString>
			{
			public:
				/**
				* Constructor.
				* @see LinkedList constructor
				* @param ownsElements 
				* @reqid 001.0063
				*/
				Collection( bool ownsElements = true ) : LinkedList<SharedString>( ownsElements )
				{
				}

				/**
				* Destructor declared explicitly to avoid compiler generated object code.
				* @reqid 001.0063
				*/
				~Collection()
				{}

				/**
				 * Make collection identical to passed collection.
				 * @param rhs right-hand side collection
				 * @param error error information
				 * @reqid 001.0076
				 */
				void Assign( const Collection& rhs, Error& error )
				{
					if (this == &rhs)
					{
						return ;
					}

					Clear();

					SharedString::Collection::ConstIterator iter = rhs.Begin();
					for ( ; !iter.IsNull(); iter++ )
					{
						SharedString* element = new SharedString( iter.GetElement()->GetString(), error );
						if ( element == NULL )
						{
							SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
							return ;
						}
						if ( error )
						{
							delete element;
							element = NULL;
							return ; //pass error back to client
						}
						AddElement( element, error );
						if ( error )
						{
							delete element;
							element = NULL;
							return ; //pass error back to client
						}
					}
				}
			};

			/**
			* Default constructor.
			* @reqid 001.0063
			*/
			SharedString() : SharedPointer<RefCountedString>()
			{
			}

			/**
			 * Constructor that encapsulates an existing or new cached buffer.
			 * @param buffer	Character buffer to be encapsulated and managed
			 * @param error		Error object to be populated
			 * @reqid 001.0063
			 */
			SharedString( const TCHAR* buffer, Error& error );

			/**
			 * Populate with existing or new raw pointer to RefCountedString instance.
			 * @param buffer	Character buffer to be encapsulated and managed
			 * @param error		Error object to be populated
			 * @reqid 001.0063
			 */
			void Populate( const TCHAR* buffer, Error& error );

			/**
			 * Assign the raw RefCountedString instance pointer to this smart pointer.
			 * @param buffer	Character buffer to be encapsulated and managed
			 * @param error		Error object to be populated
			 * @reqid 001.0063
			 */
			void AssignString( const TCHAR* buffer, Error& error );

			/**
			* Retrieve the encapsulated and managed character buffer.
			* Return the address of the encapsulated buffer.
			* @reqid 001.0063
			*/
			const TCHAR* GetString() const;

			/**
			* Test the equality with another shared string.
			* Return true if the raw pointers are identical; false if not.
			* @reqid 001.0063
			*/
			bool operator== ( const SharedString& str ) const;
			
#ifdef S11N

			/**
			 * Serialization.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			void Serialize( ISerializer& emitter, Error& error ) const
			{
				SERIALIZE_SCALAR_WITH_NAME( GetString(), _T( "Buffer" ), emitter, error, );
			}

			/**
			 * Deserializes a new object from the deserializer.
			 * @param deserializer - The source.
			 * @param output - Deserialized object if no error occured. NULL otherwise.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			static SharedString* DeserializeNew( IDeserializer& deserializer, Error& error )
			{
				SharedString* newString = new SharedString();
				if ( newString == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
					return NULL;
				}

				newString->Deserialize( deserializer, error );
				if ( error )
				{
					delete newString;
					return NULL;
				}

				return newString;
			}

			/**
			 * Deserializes the object from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			void Deserialize( IDeserializer& deserializer, Error& error )
			{
				const TCHAR* Buffer = NULL;

				DESERIALIZE_SCALAR( &Buffer, deserializer, error, );

				AssignString( Buffer, error );
				if ( error )
				{
					return;
				}
			}

#endif

		private:
			/**
			 * Assignment operator marked private to prevent its use.
			 * It may allocate memory and cannot report error.
			 * @reqid 001.0063
			 */
			void operator=( const SharedString& sharedString );
		};
	}
}

#ifdef _MSC_VER
// Revert warning levels
#pragma warning(pop)
#endif

#endif // DTP_SHAREDOBJECTS_H
