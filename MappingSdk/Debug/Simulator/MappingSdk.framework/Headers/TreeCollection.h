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

#ifndef DTP_TREECOLLECTION_H
#define DTP_TREECOLLECTION_H

#include "MapTextDefs.h"
#include "Error.h"
#include "Collection.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

namespace MapText
{
	namespace Dtp
	{
		/**
		* Keyed collection template, a suffix tree implementation.
		* @param VALUE type of the elements to store; collection stores pointers to the elements
		* @param RANGE range of character codes in the key
		* @param KEYTYPE type of the key
		* @reqid 001.0063
		*/
		template < typename VALUE, int RANGE, typename KEYTYPE >
		class TreeCollection
		{
			public:
				/**
				* Possible error codes
				* @reqid 001.0063
				*/
				enum ErrorCode
				{
					ErrorCode_AllocationFailure = 1
				};

			protected:
				/**
				* A typedef used to reference the value data type on which the template is specialized
				* @reqid 001.0063
				*/
				typedef VALUE value_t;
				typedef VALUE* value_ptr_t;

			private:
				enum
				{
					/**
					* An enumeration of constants used in range masking a value.
					* @reqid 001.0063
					*/
					range_mask = RANGE-1,
					range = RANGE
				};

				/**
				* A typedef used to reference the key data type on which the template is specialized
				* @reqid 001.0063
				*/
				typedef KEYTYPE key_t;
				/**
				* A typedef used to reference the specialized tree collection type
				* @reqid 001.0063
				*/
				typedef TreeCollection < VALUE, RANGE, KEYTYPE > type_t;

			protected:
				/** Value stored in a node of a tree. */
				value_ptr_t	m_result;
				/** Pointer to a parent node. */
				type_t*	m_parent;
				/** Children. */
				type_t*	m_table[RANGE];
				/** Number of elements in a tree. */
				int	m_elementCount;

				/**
				* A functor used for iterating a branch of the tree until the key is found
				* or determined to be unmatched
				* @reqid 001.0063
				*/
				struct iterator_callback
				{
					virtual bool operator() ( key_t* key, int len, value_ptr_t value ) = 0;
				};

				/**
				* Default constructor. Initializes the first level of child nodes of the tree,
				* one per possible value in the specified range.
				* @reqid 001.0063
				*/
				TreeCollection()
					:m_result(), m_parent( 0 ), m_elementCount( 0 )
				{
					for ( int i = 0; i < range; ++i )
					{
						m_table[i] = 0;
					}
				}

				/**
				* Constructor. Initializes the roots of the tree, one per
				* possible value in the specified range.
				* @param parent	The parent node at which this tree begins,
				* @reqid 001.0063
				*/
				TreeCollection( type_t* parent )
					:m_result(), m_parent( parent ), m_elementCount( 0 )
				{
					for ( int i = 0; i < range; ++i )
					{
						m_table[i] = 0;
					}
				}

				/**
				* Destructor.
				* @reqid 001.0063
				*/
				~TreeCollection()
				{
					for ( int i = 0; i < range; ++i )
					{
						if ( m_table[i] )
						{
							delete m_table[i]->m_result;
							delete m_table[i];
						}
					}
				}

				/**
				* Insert an element to the collection.
				* Replaces the existing value if an element with the specified key is present
				* @param buf	The key that identifies the element
				* @param len	The length of the key in bytes
				* @param val	The value associated with the key
				* @return true if added (changed) successfully; false if not
				* @reqid 001.0063
				*/
				bool Insert( const key_t* buf, int len, value_ptr_t val )
				{
					type_t* t = this;

					while ( len-- )
					{
						unsigned int c = (unsigned int)( *buf )&range_mask;

						if ( !t->m_table[c] )
						{
							t->m_table[c] = new type_t( t );
							if (t->m_table[c] == NULL)
								return false;
						}
						t = t->m_table[c];
						++buf;
					}
					t->m_result = val;

					++m_elementCount;
					return true;
				}

				/**
				* Look up an element in the collection, if present, by its key.
				* @param buf	The key that identifies the element
				* @param len	The length of the key in bytes
				* @param val	The value associated with the key, populated if key is found
				* @return true if key is found; false if not
				* @reqid 001.0063
				*/
				bool Lookup( const key_t* buf, int len, value_ptr_t* val ) const
				{
					const type_t* t = this;

					while ( len-- )
					{
						t = t->m_table[(unsigned int)( *( buf++ )&( range_mask ) )];
						if ( !t )
						{
							return false;
						}
					}

					*val = t->m_result;
					return true;
				}

				/**
				* Remove an element from the collection, if present, by its key.
				* @param buf	The key that identifies the element
				* @param len	The length of the key in bytes
				* @return true if key is found and element removed; false if not
				* @reqid 001.0063
				*/
				bool RemoveElement( const key_t* buf, int len )
				{
					type_t* lastFork = this;
					int lastForkPos = 0;
					int pos = 0;
					type_t* t = this;

					// follow the branch, but remember where the last fork in the branch is

					while ( len-- )
					{
						int entryCount = 0;
						for ( int i = 0; i < range; ++i )
						{
							entryCount += (int)( t->m_table[i] != 0 );
						}

						if ( entryCount > 1 )
						{
							lastForkPos = pos;
							lastFork = t;
						}

						t = t->m_table[(unsigned int)( buf[pos]&( range_mask ) )];
						if ( !t )
						{
							return false;
						}

						++pos;
					}

					// We found a match. Just delete the stored object.
					// NOTE: Original code deleted the whole branch of the tree!
					/**************************************************************************
					type_t** killPoint = &lastFork->m_table[buf[lastForkPos]&( range_mask )];
					delete *killPoint;
					*killPoint = 0;
					**************************************************************************/

					delete t->m_result;
					t->m_result = NULL;

					--m_elementCount;

					return true;
				}

				/**
				* Retrieve the count of elements in the collection.
				* @return the number of elements in the collection
				* @reqid 001.0063
				*/
				int Count()
				{
					return m_elementCount;
				}


				/*
				//Skeleton of the implementation of the non-recursive iteration
				bool Iterate(iterator_callback& cb)
				{
					int i = 0;
					type_t* table = this;

					while ( true )
					{
						if ( table->m_result )
						{
							//element found - invoke callback
						}

						if ( table->m_table[i] )
						{
							table = table->m_table[i]; //go inside
							//push(i) - push current i index on stack
							i = 0; //start iteration on next level
							continue;
						}

						i = i + 1; //move to next element

						if ( i == range ) //if at the end 
						{
							if (table == this)
							{
								//we reach the end
								break;
							}
							else
							{
								//go one level up
								table = table->m_parent;
								//i = pop index from stack
							}

						}
					}
				}
				*/

				/*
				* Successively retrieves all elements in the collection.
				* @param buf key buffer
				* @param maxlen max length of key
				* @param cb callback iterator; will be called for each element
				* @param bufpos for internal use
				* @return internal use
				*
				* @warning This is a recursive method and may not be DO-178B compliant (look above
				* for skeleton of non-iterative implementation).
				*
				* @reqid 001.0076
				*/
				//bool Iterate( key_t* buf, int maxlen, iterator_callback& cb, int bufpos = 0 )
				//{
				//	if ( bufpos >= maxlen )
				//	{
				//		return false;
				//	}

				//	if ( bufpos > 0 )
				//	{
				//		if ( !cb( buf, bufpos, m_result ) )
				//		{
				//			return false;
				//		}
				//	}

				//	for ( int i = 0; i < range; ++i )
				//	{
				//		if ( m_table[i] )
				//		{
				//			buf[bufpos] = i;
				//			if ( !m_table[i]->Iterate( buf, maxlen, cb, bufpos+1 ) )
				//			{
				//				return false;
				//			}
				//		}
				//	}
				//	return true;
				//}

			public:

				/**
				 * Assign contents of one tree to the other.
				 * @param rhs			tree to assign (right-hand value)
				 * @param error		an Error object
				 * @param cloner	(optional parameter) an object that supplies non-standard cloning
				 *								operation for elements of linked list (@see MapText::Dtp::Cloner)
				 * @reqid 001.0076
				 */
				virtual void Assign( const type_t& rhs, Error& error, 
					const Cloner< VALUE >& cloner = Cloner< VALUE >() )
				{
					m_parent = NULL;
					m_result = NULL;
					if ( rhs.m_result ) 
					{
						m_result = cloner(rhs.m_result, error );
						if ( error )
						{
							return; //pass error back to client
						}
					}

					m_elementCount = rhs.m_elementCount;

					for ( int i = 0; i < range; ++i )
					{
						if ( m_table[i] )
						{
							delete m_table[i];
							m_table[i] = NULL;
						}
					}

					for ( int i = 0; i < range; ++i )
					{
						if ( rhs.m_table[i] )
						{
							m_table[i] = rhs.m_table[i]->Clone( error, this, cloner );
							if ( error )
							{
								return ; //pass back error to client
							}
						}
						else
						{
							m_table[i] = NULL;
						}
					}
				}

				/**
				* Creates copy of self.
				* @param error an error object to be populated
				* @param parentTree a parent tree for the cloned object; should be NULL for trunk
				* @param cloner (optional parameter) an object that supplies non-standard cloning
				*               operation for elements of linked list (@see MapText::Dtp::Cloner)
				* @return copy of self
				*
				* @warning this method is using recursion 
				* 
				* @reqid 001.0076
				*/
				virtual type_t* Clone( Error& error, const type_t* parentTree = NULL, 
					const Cloner< VALUE >& cloner = Cloner< VALUE >() ) const
				{
					type_t* copy = new type_t( const_cast< type_t* >( parentTree ) );
					if ( copy == NULL )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return copy;
					}

					copy->m_elementCount = m_elementCount;
					copy->m_result = NULL;
					if ( m_result ) 
					{
						copy->m_result = cloner(m_result, error );
						if ( error )
						{
							delete copy;
							copy = NULL;
							return copy;
						}
					}

					for ( int i = 0; i < range; ++i )
					{
						if ( m_table[i] )
						{
							copy->m_table[i] = m_table[i]->Clone( error, this, cloner );
							if ( error )
							{
								delete copy;
								copy = NULL;
								return copy;
							}
						}
						else
						{
							copy->m_table[i] = NULL;
						}
					}

					return copy;
				}

		};

		template < typename VALUE >
		class StringKeyedTree :
			public TreeCollection < VALUE, 256, TCHAR >
		{
			public:
				/**
				* Insert an element to the collection.
				* Replaces the existing value if an element with the specified key is present
				* @param key			the key that identifies the element
				* @param val			the value associated with the key
				* @param error		an Error object
				* @reqid 001.0063
				*/
				void Insert( const TCHAR* key, VALUE* val, Error& error )
				{
					if ( !TreeCollection<VALUE, 256, TCHAR>::Insert( key, (int)_tcslen( key ), val ) )
					{
						SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
						return;
					}
					return;
				}

				/**
				* Remove an element from the collection, if present, by its key.
				* @param key	The key that identifies the element
				* @return true if key is found and element removed; false if not
				* @reqid 001.0063
				*/
				bool RemoveElement( const TCHAR* key )
				{
					return TreeCollection<VALUE, 256, TCHAR>::RemoveElement( key, (int)_tcslen(key) );
				}

				/**
				* Look up an element in the collection, if present, by its key.
				* @param key	The key that identifies the element
				* @param val	The value associated with the key, populated if key is found
				* @return true if key is found; false if not
				* @reqid 001.0063
				*/
				bool Lookup( const TCHAR* key, VALUE** val ) const
				{
					return (
						TreeCollection<VALUE, 256, TCHAR>::Lookup( key, (int)_tcslen(key), val ) &&
						(*val != NULL) );
				}

				/**
				* Indexer for array-style access of an element by its key.
				* @param key	The key that identifies the element
				* @return the value associated with the key, populated if key is found; NULL if not
				* @reqid 001.0063
				*/
				const VALUE* operator[] ( const TCHAR* key ) const
				{
					VALUE* value;
					if ( !Lookup( key, &value ) )
					{
						memset( value, 0, sizeof( VALUE* ) );
					}
					return value;
				}


		};
	}	// namespace Dtp
}	// namespace MapText

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  //DTP_TREECOLLECTION_H
