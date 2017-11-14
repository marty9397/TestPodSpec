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

#ifndef DTP_COLLECTION_H
#define DTP_COLLECTION_H

#include <stdlib.h>

#include "Serialization.h"

#include "MapTextDefs.h"
#include "Error.h"
#include <string>
#include <map>

namespace MapText
{
	namespace Dtp
	{
		template < class C > 
		class CollectionUtility;

#ifdef S11N
		// Forward declare
		class Map;
#endif

		/**
		 * Simple implementation of a double linked list.
		 * @param C the class to be stored inside the collection
		 * @reqid 001.0063
		 */
		template < class C >
		class LinkedList
		{
			friend class CollectionUtility<C>;

		public:
			/**
			 * Possible error codes
			 * @reqid 001.0063
			 */
			enum ErrorCode
			{
				ErrorCode_AllocationFailure = 1,
				ErrorCode_IteratorIsNull
			};

			protected:
				class Element
				{
				public:
					Element()
						:	m_content( NULL ),
							m_next( NULL ),
							m_previous( NULL ),
							m_ownsContent( false )
					{}

					/**
					 * Constructor.
					 * @reqid 001.0063
					 */
					Element( C* content, bool takeOwnership = true )
						:	m_content( content ),
							m_next( NULL ),
							m_previous( NULL ),
							m_ownsContent( takeOwnership )
					{
					}

					/**
					 * Destructor.
					 * @reqid 001.0063
					 */
					~Element()
					{
						if ( m_ownsContent )
						{
							delete m_content;
						}
					}

					/**
					 * Get the wrapped element
					 * @reqid 001.0063
					 */
					C* GetContent()
					{
						return m_content;
					}

					/**
					* Get the wrapped element
					* @reqid 001.0063
					*/
					const C* GetContent() const
					{
						return m_content;
					}

					/**
					 * Get the next element
					 * @reqid 001.0063
					 */
					Element* GetNext() const
					{
						return m_next;
					}

					/**
					 * Get the previous element
					 * @reqid 001.0063
					 */
					Element* GetPrevious() const
					{
						return m_previous;
					}

					/**
					 * Set the wrapped element
					 * @reqid 001.0063
					 */
					void SetContent( C* content )
					{
						m_content = content;
					}

					/**
					 * Set the next element
					 * @reqid 001.0063
					 */
					void SetNext( Element* next )
					{
						m_next = next;
					}

					/**
					 * Set the previous element
					 * @reqid 001.0063
					 */
					void SetPrevious( Element* previous )
					{
						m_previous = previous;
					}
				private:
					C* m_content;
					Element* m_next;
					Element* m_previous;
					bool m_ownsContent;
				};

			public:

				/**
				 * A bidirectional mutable iterator class to allow iteration over the linked list.
				 * @reqid 001.0063
				 */
				class Iterator
				{
					friend class CollectionUtility<C>;

				public:
					/**
					 * Constructor.
					 * @param parentList the parent list for the iterator
					 * @param currentElement position the iterator at a specific element of the list.
					 *                       if left to default or initialized with NULL iterator is in an
					 *                       invalid state (IsNull returns true).
					 * @reqid 001.0063
					 */
					Iterator( LinkedList<C> &parentList, Element* currentElement = NULL )
						: m_parentList( &parentList ), m_currentElement( currentElement )
					{
					}

					/**
					 * Copy constructor.
					 * @param otherIterator - The iterator to copy.
					 * @reqid 001.0063
					 */
					Iterator( const Iterator& otherIterator )
						:	m_parentList( otherIterator.m_parentList ), 
							m_currentElement( otherIterator.m_currentElement )
					{

					}

					/**
					 * Clone yourself.
					 * @reqid 001.0063
					 */
					Iterator* Clone( Error& error )
					{
						Iterator* clone = new Iterator( *this );
						if ( !clone )
						{
							SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
							return clone;
						}

						return clone;
					}

					/**
					 * Test if the iterator is in a valid state.
					 * @return If false is returned it may be incremented/decremented 
					 * and dereferenced. If not, then it's positioned beyond the sequence 
					 * and should be reset.
					 * @reqid 001.0063
					 */
					bool IsNull()
					{
						return m_currentElement == NULL;
					}

					/**
					 * Increment (or move forward) operator. Advances the iterator by one element.
					 * If iterator is beyond the controlled sequence (IsNull returns true) it has 
					 * no effect.
					 * @reqid 001.0063
					 */
					Iterator operator++( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetNext();
						}
						return *this;
					}

					/**
					* Decrement (or move backwards) operator. Moves the iterator back by one element.
					* If iterator is beyond the controlled sequence (IsNull returns true) it has 
					* no effect.
					* @reqid 001.0063
					*/
					Iterator operator--( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}
						return *this;
					}

					/**
					 * Dereferencing operator. It allows you to call the wrapped element methods on the
					 * iterator directly. It's critical NOT to use it when iterator is in invalid
					 * state (IsNull returns true).
					 * @reqid 001.0063
					 */
					C* operator->()
					{
						assert( m_currentElement != NULL );
						return (&*m_currentElement->GetContent());
					}

					/**
					 * Casting operator allows to cast the iterator to a pointer of the wrapped type.
					 * May not be called when IsNull returns true.
					 * @reqid 001.0063
					 */
					operator C*()
					{
						return GetElement();
					}

					/**
					* Retrieve the current element's content.
					* @reqid 001.0063
					*/
					C* GetElement()
					{
						assert( m_currentElement != NULL );
						return m_currentElement->GetContent();
					}

					/**
					* Obtain the container of the current element of the underlying collection.
					* @return the container of the current element in the underlying linked list
					* @reqid 002.0038
					*/
					Element* GetElementContainer() const
					{
						return m_currentElement;
					}

					/**
					 * Position the iterator at the beginning of the list.
					 * @reqid 001.0063
					 */
					void ResetToBeginning()
					{
						m_currentElement = m_parentList->m_head;
					}

					/**
					* Position the iterator at the end of the list.
					* @reqid 001.0063
					*/
					void ResetToEnd()
					{
						m_currentElement = m_parentList->m_tail;
					}

					/**
					* Removes the current element from the list and returns a pointer to it.
					* The element itself is NOT destroyed, caller has to take ownership over it.
					* After removal the iterator is pointing to the element before the removed one.
					* @return the removed element
					* @reqid 001.0063
					*/
					C* Remove()
					{
						if ( m_currentElement == NULL )
						{
							return NULL;
						}

						Element *prev = m_currentElement->GetPrevious();
						Element *next = m_currentElement->GetNext();

						m_parentList->RemoveElement( m_currentElement );
                        
                        C* ret = m_currentElement->GetContent();
                        m_currentElement->SetContent(NULL);
                        delete m_currentElement;

						// Go back
						if(prev != NULL)
						{
							m_currentElement = prev;
						}
						else // or position at the *now* first element
						{
							m_currentElement = next;
						}

						return ret;
					}

					/*
					 * If iterator points to NULL, then insert at beginning of the list. Otherwise, insert after the current element.
					 * @param element - element to insert
					 * @param error
					 */
					void InsertElementAfterIterator( C* element, Error& error )
					{
						Element* wrapper = new Element( element, m_parentList->m_ownsElements );
						if ( wrapper == NULL )
						{
							SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
							return;
						}

						if (m_currentElement==NULL)
						{
							wrapper->SetNext(m_parentList->m_head);
							m_parentList->m_head=wrapper;
						}
						else
						{
							wrapper->SetNext( m_currentElement->GetNext() );
							m_currentElement->SetNext( wrapper );
						}
						
						wrapper->SetPrevious( m_currentElement );

						if (m_parentList->m_tail==m_currentElement)
						{
							m_parentList->m_tail=wrapper;
						}

						m_parentList->m_numberOfItems++;
					}

				private:
					LinkedList<C> *m_parentList;
					Element *m_currentElement;
				};

				/**
				* A bidirectional non-mutable iterator class to allow iteration over a const linked list.
				* @reqid 001.0063
				*/
				class ConstIterator
				{
				public:
					/**
					* Constructor.
					* @param parentList the parent list for the iterator
					* @param currentElement position the iterator at a specific element of the list.
					*                       if left to default or initialized with NULL iterator is in an
					*                       invalid state (IsNull returns true).
					* @reqid 001.0063
					*/
					ConstIterator( const LinkedList<C> &parentList, const Element* currentElement = NULL )
						: m_parentList( &parentList ), m_currentElement( currentElement )
					{
					}

					/**
					* Test if the iterator is in a valid state.
					* @return If false is returned it may be incremented/decremented 
					* and dereferenced. If not, then it's positioned beyond the sequence 
					* and should be reset.
					* @reqid 001.0063
					*/
					bool IsNull() const
					{
						return m_currentElement == NULL;
					}

					/**
					* Increment (or move forward) operator. Advances the iterator by one element.
					* If iterator is beyond the controlled sequence (IsNull returns true) it has 
					* no effect.
					* @reqid 001.0063
					*/
					ConstIterator operator++( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetNext();
						}
						return *this;
					}

					/**
					* Decrement (or move backwards) operator. Moves the iterator back by one element.
					* If iterator is beyond the controlled sequence (IsNull returns true) it has 
					* no effect.
					* @reqid 001.0063
					*/
					ConstIterator operator--( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}
						return *this;
					}

					/**
					* Dereferencing operator. It allows you to call the wrapped element methods on the
					* iterator directly. It's critical NOT to use it when iterator is in invalid
					* state (IsNull returns true).
					* @reqid 001.0063
					*/
					const C* operator->()
					{
						assert( m_currentElement != NULL );
						return (&*m_currentElement->GetContent());
					}

					/**
					* Casting operator allows to cast the iterator to a pointer of the wrapped type.
					* May not be called when IsNull returns true.
					* @reqid 001.0063
					*/
					operator const C*()
					{
						return GetElement();
					}

					/**
					* Retrieve the current element's content.
					* @reqid 001.0063
					*/
					const C* GetElement() const
					{
						assert( m_currentElement != NULL );
						return m_currentElement->GetContent();
					}

					/**
					* Obtain the container of the current element of the underlying collection.
					* @return the container of the current element in the underlying linked list
					* @reqid 002.0038
					*/
					const Element* GetElementContainer() const
					{
						return m_currentElement;
					}

					/**
					* Position the iterator at the beginning of the list.
					* @reqid 001.0063
					*/
					void ResetToBeginning()
					{
						m_currentElement = m_parentList->m_head;
					}

					/**
					* Position the iterator at the end of the list.
					* @reqid 001.0063
					*/
					void ResetToEnd()
					{
						m_currentElement = m_parentList->m_tail;
					}
				private:
					const LinkedList<C> *m_parentList;
					const Element *m_currentElement;
				};

				/**
				 * A bidirectional mutable iterator class to allow iteration over the shown elements
				 * of a linked list of elements of a type that can be hidden.
				 * @reqid 001.0063
				 */
				class ShownElementIterator
				{
					friend class CollectionUtility<C>;

				public:
					/**
					 * Constructor.
					 * @param parentList the parent list for the iterator
					 * @param currentElement position the iterator at a specific element of the list.
					 *                       if left to default or initialized with NULL iterator is in an
					 *                       invalid state (IsNull returns true).
					 * @reqid 001.0063
					 */
					ShownElementIterator( LinkedList<C> &parentList, Element* currentElement = NULL )
						: m_parentList( &parentList ), m_currentElement( currentElement )
					{
					}

					/**
					 * Copy constructor.
					 * @param otherIterator - The iterator to copy.
					 * @reqid 001.0063
					 */
					ShownElementIterator( const ShownElementIterator& otherIterator )
						:	m_parentList( otherIterator.m_parentList ), 
							m_currentElement( otherIterator.m_currentElement )
					{
					}

					/**
					 * Test if the iterator is in a valid state.
					 * @return If false is returned it may be incremented/decremented 
					 * and dereferenced. If not, then it's positioned beyond the sequence 
					 * and should be reset.
					 * @reqid 001.0063
					 */
					bool IsNull()
					{
						const Element* element = m_currentElement;
						while ( ( element != NULL ) && ( element->GetContent()->IsHidden() ) )
						{
							element = element->GetNext();
						}
						return element == NULL;
					}

					/**
					 * Increment (or move forward) operator. Advances the iterator by one element.
					 * If iterator is beyond the controlled sequence (IsNull returns true) it has 
					 * no effect.
					 * @reqid 001.0063
					 */
					ShownElementIterator operator++( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetNext();
						}

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetNext();
						}

						return *this;
					}

					/**
					* Decrement (or move backwards) operator. Moves the iterator back by one element.
					* If iterator is beyond the controlled sequence (IsNull returns true) it has 
					* no effect.
					* @reqid 001.0063
					*/
					ShownElementIterator operator--( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}

						return *this;
					}

					/**
					 * Dereferencing operator. It allows you to call the wrapped element methods on the
					 * iterator directly. It's critical NOT to use it when iterator is in invalid
					 * state (IsNull returns true).
					 * @reqid 001.0063
					 */
					C* operator->()
					{
						assert( m_currentElement != NULL );
						return (&*m_currentElement->GetContent());
					}

					/**
					 * Casting operator allows to cast the iterator to a pointer of the wrapped type.
					 * May not be called when IsNull returns true.
					 * @reqid 001.0063
					 */
					operator C*()
					{
						return GetElement();
					}

					/**
					* Retrieve the current element's content.
					* @reqid 001.0063
					*/
					C* GetElement()
					{
						assert( m_currentElement != NULL );
						return m_currentElement->GetContent();
					}

					/**
					* Obtain the container of the current element of the underlying collection.
					* @return the container of the current element in the underlying linked list
					* @reqid 002.0038
					*/
					Element* GetElementContainer() const
					{
						return m_currentElement;
					}

					/**
					 * Position the iterator at the beginning of the list.
					 * @reqid 001.0063
					 */
					void ResetToBeginning()
					{
						m_currentElement = m_parentList->m_head;

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetNext();
						}
					}

					/**
					* Position the iterator at the end of the list.
					* @reqid 001.0063
					*/
					void ResetToEnd()
					{
						m_currentElement = m_parentList->m_tail;

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}
					}

					/**
					* Removes the current element from the list and returns a pointer to it.
					* The element itself is NOT destroyed, caller has to take ownership over it.
					* After removal the iterator is pointing to the element before the removed one.
					* @return the removed element
					* @reqid 001.0063
					*/
					C* Remove()
					{
						if ( m_currentElement == NULL )
						{
							return NULL;
						}

						Element *prev = m_currentElement->GetPrevious();
						while ( ( prev != NULL ) && prev->GetContent()->IsHidden() )
						{
							prev = prev->GetPrevious();
						}

						Element *next = m_currentElement->GetNext();
						while ( ( next != NULL ) && next->GetContent()->IsHidden() )
						{
							next = next->GetNext();
						}

						if ( prev != NULL )
						{
							prev->SetNext( next );
						}
						else
						{
							// We've just removed the head
							m_parentList->m_head = next;
						}

						if ( next != NULL )
						{
							next->SetPrevious( prev );
						}
						else
						{
							// We've just removed the tail
							m_parentList->m_tail = prev;
						}

						C* ret = m_currentElement->GetContent();

						// Make sure the wrapper will not remove the contents
						m_currentElement->SetContent( NULL );
						delete m_currentElement;

						// Go back
						if(prev != NULL)
						{
							m_currentElement = prev;
						}
						else
						{
							m_currentElement = next;
						}

						m_parentList->m_numberOfItems--;

						return ret;
					}

					/**
					* Get the count of elements not hidden.
					* @return the count of elements not hidden
					* @reqid 001.0063
					*/
					int GetShownElementCount()
					{
						int count = 0;
						const Element* holdCurrentElement = m_currentElement;
						ResetToBeginning();
						while( !IsNull() )
						{
							count++;
							(*this)++;
						}
						m_currentElement = holdCurrentElement;
						return count;
					}

					/*
					 * Implemented but quickly became obsolete.
					 * Commented out and not deleted just in case we need 
					 * it again.
					 *
					void InsertElementAfterIterator( C* element, Error& error )
					{
						if ( m_currentElement == NULL )
						{
							SET_ERROR_NOMSG( error, ErrorCode_IteratorIsNull )
						}

						Element* wrapper = new Element( element, m_parentList->m_ownsElements );
						if ( wrapper == NULL )
						{
							SET_ERROR_NOMSG( error, ErrorCode_AllocationFailure );
							return;
						}

						wrapper->SetNext( m_currentElement->GetNext() );
						wrapper->SetPrevious( m_currentElement );
						m_currentElement->SetNext( wrapper );
					}*/

				private:
					LinkedList<C> *m_parentList;
					Element *m_currentElement;
				};

				/**
				* A bidirectional non-mutable iterator class to allow iteration over a const linked list.
				* @reqid 001.0063
				*/
				class ShownElementConstIterator
				{
				public:
					/**
					* Constructor.
					* @param parentList the parent list for the iterator
					* @param currentElement position the iterator at a specific element of the list.
					*                       if left to default or initialized with NULL iterator is in an
					*                       invalid state (IsNull returns true).
					* @reqid 001.0063
					*/
					ShownElementConstIterator( const LinkedList<C> &parentList, const Element* currentElement = NULL )
						: m_parentList( &parentList ), m_currentElement( currentElement )
					{
					}

					/**
					* Test if the iterator is in a valid state.
					* @return If false is returned it may be incremented/decremented 
					* and dereferenced. If not, then it's positioned beyond the sequence 
					* and should be reset.
					* @reqid 001.0063
					*/
					bool IsNull() const
					{
						const Element* element = m_currentElement;
						while ( ( element != NULL ) && ( element->GetContent()->IsHidden() ) )
						{
							element = element->GetNext();
						}
						return element == NULL;
					}

					/**
					* Increment (or move forward) operator. Advances the iterator by one element.
					* If iterator is beyond the controlled sequence (IsNull returns true) it has 
					* no effect.
					* @reqid 001.0063
					*/
					ShownElementConstIterator operator++( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetNext();
						}

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetNext();
						}

						return *this;
					}

					/**
					* Decrement (or move backwards) operator. Moves the iterator back by one element.
					* If iterator is beyond the controlled sequence (IsNull returns true) it has 
					* no effect.
					* @reqid 001.0063
					*/
					ShownElementConstIterator operator--( int )
					{
						if ( m_currentElement )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}

						return *this;
					}

					/**
					* Dereferencing operator. It allows you to call the wrapped element methods on the
					* iterator directly. It's critical NOT to use it when iterator is in invalid
					* state (IsNull returns true).
					* @reqid 001.0063
					*/
					const C* operator->()
					{
						assert( m_currentElement != NULL );
						return (&*m_currentElement->GetContent());
					}

					/**
					* Casting operator allows to cast the iterator to a pointer of the wrapped type.
					* May not be called when IsNull returns true.
					* @reqid 001.0063
					*/
					operator const C*()
					{
						return GetElement();
					}

					/**
					* Retrieve the current element's content.
					* @reqid 001.0063
					*/
					const C* GetElement() const
					{
						assert( m_currentElement != NULL );
						return m_currentElement->GetContent();
					}

					/**
					* Obtain the container of the current element of the underlying collection.
					* @return the container of the current element in the underlying linked list
					* @reqid 002.0038
					*/
					const Element* GetElementContainer() const
					{
						return m_currentElement;
					}

					/**
					* Position the iterator at the beginning of the list.
					* @reqid 001.0063
					*/
					void ResetToBeginning()
					{
						m_currentElement = m_parentList->m_head;

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetNext();
						}
					}

					/**
					* Position the iterator at the end of the list.
					* @reqid 001.0063
					*/
					void ResetToEnd()
					{
						m_currentElement = m_parentList->m_tail;

						while ( ( m_currentElement != NULL ) && ( m_currentElement->GetContent()->IsHidden() ) )
						{
							m_currentElement = m_currentElement->GetPrevious();
						}
					}

					/**
					* Get the count of elements not hidden.
					* @return the count of elements not hidden
					* @reqid 001.0063
					*/
					int GetShownElementCount()
					{
						int count = 0;
						const Element* holdCurrentElement = m_currentElement;
						ResetToBeginning();
						while( !IsNull() )
						{
							count++;
							(*this)++;
						}
						m_currentElement = holdCurrentElement;
						return count;
					}

				private:
					const LinkedList<C> *m_parentList;
					const Element *m_currentElement;
				};
				/////////////////////////////////////////////////////////////////////////////

				/**
				 * Default constructor, initializes the list plumbing.
				 * @param ownsElements if set to true ownership over each element put into 
				 *        the collection is assumed. Otherwise the collection only stores
				 *        references and elements are NOT destroyed on collection destruction.
				 * @reqid 001.0063
				 */
				LinkedList( bool ownsElements = true )
					:	m_numberOfItems( 0 ), 
						m_head( NULL ),
						m_tail( NULL ),
						m_iterator( NULL ),
						m_ownsElements( ownsElements )
				{
				}

				/**
				 * Deallocation of the whole list
				 * @reqid 001.0063
				 */
				virtual ~LinkedList()
				{
					Clear();
				}

				/**
				 * Get an iterator positioned at the beginning of the list
				 * @return the iterator
				 * @reqid 001.0063
				 */
				Iterator Begin()
				{
					return Iterator( *this, m_head );
				}

				/**
				 * Get an iterator positioned at the beginning of the list, allocate iterator on heap
				 * @return the iterator
				 * @reqid 001.0063
				 */
				Iterator *BeginOnHeap(Error &error)
				{
					Iterator *iterator=new Iterator(*this, m_head);

					if (iterator==NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
						return NULL;
					}
					else
						return iterator;
				}

				/**
				* Get an iterator positioned at the specified element of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				Iterator Begin( Element* bookmarkElement )
				{
					if ( bookmarkElement == NULL )
					{
						return Iterator( *this, m_head );
					}

					return Iterator( *this, bookmarkElement );
				}

				/**
				* Get an iterator positioned at the end of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				Iterator End()
				{
					return Iterator( *this, m_tail );
				}

				/**
				* Get a const iterator positioned at the beginning of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ConstIterator Begin() const
				{
					return ConstIterator( *this, m_head );
				}

				/**
				* Get a const iterator positioned at the specified element 
				* @return the iterator
				* @reqid 001.0063
				*/
				ConstIterator Begin( Element* bookmarkElement ) const
				{
					if ( bookmarkElement == NULL )
					{
						return ConstIterator( *this, m_head );
					}

					return ConstIterator( *this, bookmarkElement )++;
				}

				/**
				* Get a const iterator positioned at the end of the list
				* @return the iterator
				* @reqid 001.0063
				*/
				ConstIterator End() const
				{
					return ConstIterator( *this, m_tail );
				}

				/**
				 * Obtain the first element of the list
				 * @reqid 001.0063
				 */
				C* GetFirstElement()
				{
					m_iterator = m_head;
					return ( m_iterator == NULL ? NULL : m_iterator->GetContent() );
				}

				/**
				 * Obtain the last element of the list
				 * @return the last element in the linked list
				 * @reqid 001.0063
				 */
				C* GetLastElement()
				{
					m_iterator = m_tail;
					return ( m_iterator == NULL ? NULL : m_iterator->GetContent() );
				}

				/**
				 * Obtain the next element of the list
				 * @reqid 001.0063
				 */
				C* GetNextElement()
				{
					if ( m_iterator == NULL )
					{
						// Handle the case when we're before the head
						// e.g. when the first element has been removed
						m_iterator = m_head;
					}
					else
					{
						m_iterator = m_iterator->GetNext();
					}

					return ( m_iterator == NULL ? NULL : m_iterator->GetContent() );
				}

				/**
				 * Obtain the previous element of the list
				 * @return previous element from the current one in the linked list.
				 * @reqid 001.0063
				 */
				C* GetPreviousElement()
				{
					if ( m_iterator == NULL )
					{
						/*If all elements have been traversed or traversal is being started with this method
						  instead of the GetLastElement or GetFirstElement methods, then return the last 
						  element's content.*/
						m_iterator = m_head;
					}
					else
					{
						m_iterator = m_iterator->GetPrevious();
					}

					return ( m_iterator == NULL ? NULL : m_iterator->GetContent() );
				}

				/**
				 * Add an element to the list. In case of an error ownership of the object
				 * is NOT taken and it's still the client responsibility to manage its lifecycle.
				 * @reqid 001.0063
				 */
				virtual void AddElement( C* element, Error& error )
				{
					Element* wrapper = new Element( element, m_ownsElements );
					if (wrapper == NULL)
					{
						SET_ERROR_NOMSG(error, ErrorCode_AllocationFailure);
					}

					if ( m_tail == NULL )
					{
						m_head = wrapper;
						m_tail = wrapper;
					}
					else
					{
						m_tail->SetNext( wrapper );
						wrapper->SetPrevious( m_tail );
						m_tail = wrapper;
					}
					m_numberOfItems++;

					return;
				}

				/**
				 * Remove last element from the list and returns a pointer to it.
				 * Caller takes responsibility to delete this element.
				 * After removal, if the iterator was pointing to the last element, it is updated 
				 * to point to the element before the removed one.
				 * @return pointer to the removed element
				 * @reqid 001.0063
				 */
				virtual C* RemoveLastElement()
				{
					if (m_head == NULL)
					{
						return NULL;  //list empty
					}
					assert(m_tail);

					//update previous
					Element *prev = m_tail->GetPrevious();
					if (prev != NULL)
					{
						prev->SetNext(NULL); //detach previous						
					}

					//update iterator
					if (m_iterator == m_tail) //update iterator
					{
						m_iterator = prev;
					}

					C* ret = m_tail->GetContent();

					//remove tail
					m_tail->SetContent(NULL); // Make sure the wrapper will not delete the contents
					delete m_tail;
					--m_numberOfItems;

					//set new tail, head
					m_tail = prev; //set new tail
					if (m_numberOfItems == 0)
					{

						m_tail = m_head = NULL; //list is empty
					}

					return ret;
				}
			
				/**
				 * Removes the current element from the list and returns a pointer to it.
				 * The element itself is NOT destroyed, caller has to take ownership over it.
				 * After removal the iterator is pointing to the element before the removed one.
				 * @return the removed element
				 * @reqid 001.0063
				 */
				virtual C* RemoveCurrentElement()
				{
					if ( m_iterator == NULL || m_head == NULL )
					{
						return NULL;
					}

					Element *prev = m_iterator->GetPrevious();
					Element *next = m_iterator->GetNext();

					if ( prev != NULL )
					{
						prev->SetNext( next );
					}
					else
					{
						// We've just removed the head
						m_head = next;
					}

					if ( next != NULL )
					{
						next->SetPrevious( prev );
					}
					else
					{
						// We've just removed the tail
						m_tail = prev;
					}

					C* ret = m_iterator->GetContent();

					// Make sure the wrapper will not remove the contents
					m_iterator->SetContent( NULL );
					delete m_iterator;

					// Go back
					m_iterator = prev;
					m_numberOfItems--;

					return ret;
				}

				/**
				 * Get the number of elements in the list
				 * @reqid 001.0063
				 */
				virtual int Count() const
				{
					return m_numberOfItems;
				}

				/**
				 * Clear the collection and release all the items.
				 * @reqid 001.0063
				 */
				virtual void Clear()
				{
					while ( m_head != NULL )
					{
						Element *temp = m_head;
						m_head = m_head->GetNext();
						delete temp;
					}

					m_tail = m_head;
					m_iterator = NULL;
					m_numberOfItems = 0;
				}

				/**
				* Check if the elements are owned by the collection. If not they 
				* will not get deallocated when the collection is destructed.
				* @return true if elements are owned
				* @reqid 001.0063
				*/
				bool GetOwnsElements() const 
				{ 
					return m_ownsElements; 
				}

				/**
				* Set if the collection owns its elements. If not they 
				* will not get deallocated when the collection is destructed.
				* @param ownsElements - true if the collection does, false if not
				* @reqid 001.0063
				*/
				void SetOwnsElements(bool ownsElements)
				{
					m_ownsElements = ownsElements;
				}

			protected:

				/**
				 * Removes the container element and returns the owned pointer.
                 * The element itself is NOT deallocated. Caller should assume it's ownership.
				 * @reqid 001.0063
				 */
				virtual void RemoveElement( Element* e )
				{
					if ( e == NULL )
					{
						return;
					}

					Element *prev = e->GetPrevious();
					Element *next = e->GetNext();

					if ( prev != NULL )
					{
						prev->SetNext( next );
					}
					else
					{
						// We've just removed the head
						m_head = next;
					}

					if ( next != NULL )
					{
						next->SetPrevious( prev );
					}
					else
					{
						// We've just removed the tail
						m_tail = prev;
					}

					m_numberOfItems--;
				}

				/**
				 * Number of items in the list
				 * @reqid 001.0063
				 */
				int m_numberOfItems;

				Element* m_head;
				Element* m_tail;
				Element* m_iterator;
				bool m_ownsElements;
		};

		/**
		* Cloner class that defines clone operation for class C.
		* Subclass and override operator().To be used with ClonableLinkedList & 
		* CollectionUtility::CopyElements method.
		* @param C class for cloning 
		*/
		template< class C >
		class Cloner
		{
		public:
			/** 
			* Virtual desctructor.
			*/
			virtual ~Cloner()
			{}

			/**
			* Clone valueToClone instance.
			*/
			virtual C* operator() ( const C* valueToClone, Error& error ) const
			{
				assert( valueToClone != NULL );

				return valueToClone->Clone( error ); //if error pass back to a caller
			}
		};

		/**
		 * A cloneable linked list. Template separated from linked list to
		 * avoid potential dead code in the template instantiation
		 * @param C the class to be stored inside the collection
		 * @param W class deriving from the template, parameter needed for the
		 *          Clone method to return the proper derived type
		 */
		template < class C, class W >
		class ClonableLinkedList : public LinkedList< C >
		{
			public:
				/**
				 * @see LinkedList::LinkedList
				 * @param ownsElements
				 * @reqid 001.0063
				 */
				ClonableLinkedList( bool ownsElements = true )
					: LinkedList< C >( ownsElements )
				{
				}

				/**
				 * Create a deep copy of the wrapper collection class.
				 * @param error error information
				 * @param cloner (optional parameter) an object that supplies non-standard cloning
				 *               operation for elements of linked list (@see Cloner)
				 * @reqid 001.0063
				 */
				virtual W* Clone(Error& error, const Cloner<C>& cloner = Cloner<C>() ) const
				{
					W* clone = new W();
					if (clone == NULL)
					{
						SET_ERROR_NOMSG(error, LinkedList<C>::ErrorCode_AllocationFailure);
						return NULL;
					}

					typename LinkedList<C>::Element* element = LinkedList<C>::m_head;

					while ( element )
					{
						// Cast in case the Clone method returns parent class.
						C* content = cloner(element->GetContent(), error);
						if (content == NULL)
						{
							clone->Clear();
							return NULL;
						}

						clone->AddElement( content, error );
						if(error)
						{
							clone->Clear();
							return NULL;
						}

						element = element->GetNext();
					}

					return clone;
				}
		};

		/**
		 * Utility class 
		 * @reqid 001.0063
		 */
		template< class C >
		class CollectionUtility
		{
		public:
			enum ErrorCode
			{
				ErrorCode_IllegalOperation = 1,
			};

			/**
			 * Transfer element from src appending them to dest.
			 * src is emptied.
			 * @param src source of the elements
			 * @param dest destination for the elements
			 * @param errorInfo error information
			 * @return 0 on success, non-zero error code otherwise
			 * @reqid 001.0063
			 */
			static void TransferElements( LinkedList<C>& src, LinkedList<C>& dest, Error& errorInfo )
			{
				if ( src.m_ownsElements != dest.m_ownsElements )
				{
					SET_ERROR( errorInfo, ErrorCode_IllegalOperation, _T( "Can not transfer elements between collections with different ownership settings." ) );
					return;
				}

				if ( src.m_numberOfItems == 0 )
				{
					// Nothing to do
					return;
				}

				if ( dest.m_numberOfItems == 0 )
				{
					dest.m_head = src.m_head;
					dest.m_tail = src.m_tail;
					dest.m_numberOfItems = src.m_numberOfItems;
				}
				else
				{
					dest.m_tail->SetNext( src.m_head );
					src.m_head->SetPrevious( dest.m_tail );
					dest.m_tail = src.m_tail;
					dest.m_numberOfItems += src.m_numberOfItems;
				}

				src.m_head = NULL;
				src.m_tail = NULL;
				src.m_iterator = NULL;
				src.m_numberOfItems = 0;
			}

			/**
			 * Transfers a single element between collections. The iterator is appended to the target
			 * list. The owning iterator is pushed forward to the next element.
			 * @param src - An iterator pointing to the element to be transferred.
			 * @param dest - The collection to which the ement should be transferred to.
			 * @param error[out] - Error info.
			 * @reqid 001.0063
			 */
			static void TransferSingleElement( typename LinkedList<C>::Iterator& src, LinkedList<C>& dest, Error& error )
			{
				if ( src.m_parentList->m_ownsElements != dest.m_ownsElements )
				{
					SET_ERROR( error, ErrorCode_IllegalOperation, _T( "Can not transfer elements between collections with different ownership settings." ) );
					return;
				}

				assert( !src.IsNull() );
				
				typedef typename LinkedList<C>::Element MyElement;
				MyElement* wrapper = src.m_currentElement;
				src.m_currentElement = wrapper->GetNext(); //update iterator in src list

				src.m_parentList->RemoveElement( wrapper );

				// Append the element to the target list
				wrapper->SetNext( NULL );
				if ( dest.m_tail != NULL )
				{
					wrapper->SetPrevious( dest.m_tail );
					dest.m_tail->SetNext( wrapper );
					dest.m_tail = wrapper;
				}
				else
				{
					// Adding a first element
					wrapper->SetPrevious( NULL );
					dest.m_head = dest.m_tail = wrapper;
				}

				// Fix element count.
				dest.m_numberOfItems++;

			}

			/**
			* Copy elements of one linked list to another one. Elements are
			* appended to the dest collection.
			* Caution: only owning or non-owning collection pairs can be used.
			* @param src Source of the elements.
			* @param dest Destination for the copied elements.
			* @param errorInfo error information.
			* @param cloner (optional parameter) an object that supplies non-standard cloning
			*               operation for elements of linked list (@see Cloner)
			* @reqid 001.0063
			*/
			static void CopyElements( const LinkedList<C>& src, LinkedList<C>& dest, Error &errorInfo, 
				const Cloner<C>& cloner = Cloner<C>() )
			{
				if ( src.m_ownsElements != dest.m_ownsElements )
				{
					SET_ERROR( errorInfo, ErrorCode_IllegalOperation, 
						_T( "Can not copy elements between collections with different ownership settings." ) );
					return;
				}

				for ( typename LinkedList<C>::ConstIterator i = src.Begin(); !i.IsNull(); i++ )
				{
					C* clone = cloner( i.GetElement(), errorInfo );
					if ( errorInfo )
					{
						return;
					}

					dest.AddElement( clone, errorInfo );
					if ( errorInfo )
					{
						return;
					}
				}
			}
		};

#define INITIAL_QUEUE_SORTING_ARRAY_SIZE 50


		/**
		 * PriorityQueue allows you to queue elements and sort them by priority.
		 * The objects stored by the queue have to implement a GetMetric() method 
		 * returning a float metric value. This value is used to compare elements
		 * in the queue. The queue is sorted ascending, meaning the highest metric
		 * element is considered 'best'. To achieve reversed prioritization you may 
		 * change the way metric is calculated or simply negate the returned value.
		 */
		template< class C >
		class PriorityQueue
		{
		public:

			enum ErrorCode
			{
				ErrorCode_AllocationError = 1,
			};
			/**
			* Constructor.
			* @param error error info
			* @reqid 001.0063
			*/
			PriorityQueue( Error& error )
				: m_sortingArraySize( INITIAL_QUEUE_SORTING_ARRAY_SIZE ),
				m_elementsCount( 0 ), m_ordered( false )
			{
				m_sortingArray = new C*[ INITIAL_QUEUE_SORTING_ARRAY_SIZE ];
				if ( m_sortingArray == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationError );
					return;
				}
			}

			/**
			* Constructor. Takes ownership over all the elements of a linked lists 
			* and purges the list itself.
			* @param src		the collection from which the priority is to be created
			* @param error	an Error object
			* @reqid 001.0063
			*/
			PriorityQueue( LinkedList<C>& src, Error& error )
				: m_sortingArraySize( 0 ), m_sortingArray( NULL ),
				m_elementsCount( 0 ), m_ordered( false )
			{
				AbsorbAllElements( src, error );
			}

			/**
			* Destructor. Compiler generated code avoidance.
			* @reqid 001.0063
			*/
			~PriorityQueue()
			{
				for ( int i = 0; i < m_elementsCount; i++ )
				{
					delete m_sortingArray[i];
				}
				delete[] m_sortingArray;
			}

			/**
			* Take ownership over all the elements of a linked lists 
			* and purge the list itself.
			* @param src		the collection from which the priority is to be created
			* @param error	an Error object
			* @reqid 001.0063
			*/
			void AbsorbAllElements( LinkedList<C>& src, Error& error )
			{
				GrowSortingArray( m_sortingArraySize + src.Count(), error );
				if ( error )
				{
					return;
				}

				// Move the elements to the queue
				int cnt = 0;
				for ( typename LinkedList<C>::Iterator i = src.End(); ! i.IsNull(); cnt++ )
				{
					m_sortingArray[cnt] = i.Remove();
					m_elementsCount++;
				}
				m_ordered = false;
			}

			/**
			* Enqueue a new element. Ownership over the element is taken.
			* @param element
			* @param error error info
			* @reqid 001.0063
			*/
			void Enqueue( C* element, Error& error )
			{
				// Grow the sorting array if necessary
				if ( m_elementsCount == m_sortingArraySize )
				{
					GrowSortingArray( error );
					if ( error )
					{
						return;
					}
				}

				m_elementsCount++;
				m_sortingArray[ m_elementsCount-1 ] = element;
				m_ordered = false;
			}

			/**
			* Number of elements in the queue
			* @return elements count
			* @reqid 001.0063
			*/
			int Count()
			{
				return m_elementsCount;
			}

			/**
			* Remove the best element from the queue
			* @return 
			* @reqid 001.0063
			*/
			C* DequeueBestElement()
			{
				assert( m_elementsCount > 0 );

				SortQueue();

				m_elementsCount--;
				return m_sortingArray[ m_elementsCount ];
			}

			/**
			* Get a pointer to the n-th element, where 0 means
			* the best one and Count-1 is the worst one. 
			* Ownership is maintained by the queue.
			* @return the element pointer
			* @reqid 001.0063
			*/
			C* GetNthElement( int number )
			{
				assert( number < m_elementsCount );

				SortQueue();

				return m_sortingArray[ m_elementsCount-number-1 ];
			}

			/**
			 * Call this method if objects held have changed somehow, re-sort
			 * will take place.
			 * @reqid 001.0063
			 */
			/*void ForceSorting()
			{
				m_ordered = false;
				SortQueue();
			}*/

		private:
			C** m_sortingArray;
			int m_sortingArraySize;
			int m_elementsCount;
			bool m_ordered;

			/**
			* Comparator for qsort. Sorts the elements ascending (higher metric is better, 
			* but put the best element in the end, this way we can dequeue by simply 
			* decreasing the count).
			* @param a first element
			* @param b second element
			* @return -1 if metric of a < metric of b, +1 if a > b and 0 if a == b (it has 
			* to sort ascending)
			* @reqid 001.0063
			*/
			static int CompareElementsByMetric( const void *a, const void *b )
			{
				C* elementA = *const_cast<C**>(static_cast<const C* const *>(a));
				C* elementB = *const_cast<C**>(static_cast<const C* const *>(b));

				float metricA = elementA->GetMetric();
				float metricB = elementB->GetMetric();

				if ( metricA < metricB )
				{
					return -1;
				}
				else if ( metricA > metricB )
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}

			/**
			 * Grow the internal array by doubling its size (this should minimize the number of allocations).
			 * @param error
			 * @reqid 001.0063
			 */
			void GrowSortingArray( Error& error )
			{
				GrowSortingArray( m_sortingArraySize * 2, error );
			}

			/**
			* Grow the internal to the requested size.
			* @param requestedSize	the requested size
			* @param error					an Error object
			* @reqid 001.0063
			*/
			void GrowSortingArray( int requestedSize, Error& error )
			{
				assert( requestedSize > m_sortingArraySize );
				assert( requestedSize > 0 );

				// Allocate new array
				C** newArray = new C*[requestedSize];
				if ( newArray == NULL )
				{
					SET_ERROR_NOMSG( error, ErrorCode_AllocationError );
					return;
				}

				// Perform a fast mem copy of the old array
				memcpy( (void*)newArray, (void*)m_sortingArray, m_sortingArraySize*sizeof(C*) );

				delete[] m_sortingArray;
				m_sortingArray = newArray;
				m_sortingArraySize = requestedSize;
			}

			/**
			 * Sort queue if it's not sorted yet
			 * @reqid 001.0063
			 */
			void SortQueue()
			{
				if ( !m_ordered )
				{
					qsort( (void*)m_sortingArray, m_elementsCount, sizeof( C* ), CompareElementsByMetric );
					m_ordered = true;
				}
			}
		};

		/**
		* A circular, doubly linked list.
		* @param C the class to be stored inside the collection
		*/
		template < class C >
		class CircularLinkedList : public LinkedList< C >
		{
		public:
			/**
			* @see LinkedList::LinkedList
			* @param ownsElements
			* @reqid 005.0058
			*/
			CircularLinkedList( bool ownsElements = true )
				: LinkedList< C >( ownsElements )
			{
			}

			/**
			* @see LinkedList::~LinkedList
			* @reqid 005.0058
			*/
			virtual ~CircularLinkedList()
			{
				Clear();
			}

			/**
			* @see LinkedList::AddElement
			* @reqid 005.0058
			*/
			void AddElement( C* element, Error& error )
			{
				typedef typename LinkedList<C>::Element MyElement;
				MyElement* wrapper = new MyElement( element, LinkedList<C>::m_ownsElements );
				if (wrapper == NULL)
				{
					SET_ERROR_NOMSG(error, LinkedList<C>::ErrorCode_AllocationFailure);
				}

				if ( LinkedList<C>::m_tail == NULL )
				{
					LinkedList<C>::m_head = wrapper;
					LinkedList<C>::m_tail = wrapper;
				}
				else
				{
					LinkedList<C>::m_tail->SetNext( wrapper );
					wrapper->SetPrevious( LinkedList<C>::m_tail );
					LinkedList<C>::m_tail = wrapper;
				}

				LinkedList<C>::m_tail->SetNext( LinkedList<C>::m_head );
				LinkedList<C>::m_head->SetPrevious( LinkedList<C>::m_tail );

				LinkedList<C>::m_numberOfItems++;

				return;
			}

			/**
			* @see LinkedList::RemoveCurrentElement
			* @reqid 005.0058
			*/
			C* RemoveCurrentElement()
			{
				if ( LinkedList<C>::m_iterator == NULL || LinkedList<C>::m_head == NULL )
				{
					return NULL;
				}
				
				typedef typename LinkedList<C>::Element MyElement;
				MyElement* prev = LinkedList<C>::m_iterator->GetPrevious();
				MyElement* next = LinkedList<C>::m_iterator->GetNext();

				if ( prev != LinkedList<C>::m_tail )
				{
					prev->SetNext( next );
				}
				else
				{
					// We've just removed the head
					LinkedList<C>::m_head = next;
					LinkedList<C>::m_tail->SetNext( LinkedList<C>::m_head );
					LinkedList<C>::m_head->SetPrevious( LinkedList<C>::m_tail);
				}

				if ( next != LinkedList<C>::m_head )
				{
					next->SetPrevious( prev );
				}
				else
				{
					// We've just removed the tail
					LinkedList<C>::m_tail = prev;
					LinkedList<C>::m_tail->SetNext( LinkedList<C>::m_head );
					LinkedList<C>::m_head->SetPrevious( LinkedList<C>::m_tail);
				}

				C* ret = LinkedList<C>::m_iterator->GetContent();

				// Make sure the wrapper will not remove the contents
				LinkedList<C>::m_iterator->SetContent( NULL );
				delete LinkedList<C>::m_iterator;

				// Go back
				LinkedList<C>::m_iterator = prev;
				LinkedList<C>::m_numberOfItems--;

				return ret;
			}

			/**
			* @see LinkedList::Clear
			* @reqid 005.0058
			*/
			void Clear()
			{
				typedef typename LinkedList<C>::Element MyElement;
				while ( LinkedList<C>::m_head != LinkedList<C>::m_tail )
				{
					MyElement *temp = LinkedList<C>::m_head;
					LinkedList<C>::m_head = LinkedList<C>::m_head->GetNext();
					delete temp;
				}

				delete LinkedList<C>::m_head;
				LinkedList<C>::m_head = NULL;

				LinkedList<C>::m_tail = LinkedList<C>::m_head;
				LinkedList<C>::m_iterator = NULL;
				LinkedList<C>::m_numberOfItems = 0;
			}
		};

		/**
		 * A class representing a linked list of integers.
		 * @reqid 001.0063
		 */
		class MAPTEXTTYPES_API Integers : public LinkedList<int>
		{
			public:
				/**
				 * Default constructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				Integers()
				{
				}

				/**
				 * Default destructor implemented to avoid compiler generated object code.
				 * @reqid 001.0063
				 */
				~Integers()
				{
				}
		};

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
					ErrorCode_AllocationFailure = 1,
#ifdef S11N
					ErrorCode_DeserializationError,
#endif
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
			
#ifdef S11N
		protected:
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
			bool Iterate( key_t* buf, int maxlen, iterator_callback& cb, int bufpos = 0 )
			{
				if ( bufpos >= maxlen )
				{
					return false;
				}
				
				if ( bufpos > 0 )
				{
					if ( !cb( buf, bufpos, m_result ) )
					{
						return false;
					}
				}
				
				for ( int i = 0; i < range; ++i )
				{
					if ( m_table[i] )
					{
						buf[bufpos] = (KEYTYPE)i;
						if ( !m_table[i]->Iterate( buf, maxlen, cb, bufpos+1 ) )
						{
							return false;
						}
					}
				}
				return true;
			}
			
#endif
		};

		template < class VALUE >
		class StringKeyedTree
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
				void Insert( const TCHAR* key, VALUE* val, Error& )
				{
                    typename std::map<std::string,VALUE*>::iterator element = m_map.find( key );
                    if ( element != m_map.end() )
                    {
                        delete element->second;
                    }
                    
                    m_map[ key ] = val;
					return;
				}
            
                ~StringKeyedTree()
                {
                    Clear();
                }

				/**
				* Remove an element from the collection, if present, by its key.
				* @param key	The key that identifies the element
				* @return true if key is found and element removed; false if not
				* @reqid 001.0063
				*/
				bool RemoveElement( const TCHAR* key )
				{
                    typename std::map<std::string,VALUE*>::iterator element = m_map.find( key );
                    if ( element != m_map.end() )
                    {
                        delete element->second;
                        m_map.erase( element );
                        return true;
                    }
                    
					return false;
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
                    typename std::map<std::string,VALUE*>::const_iterator element = m_map.find( key );
                    if ( element != m_map.end() )
                    {
                        *val = element->second;
                        return true;
                    }
                    
                    *val = NULL;
					return false;
				}

				/**
				* Indexer for array-style access of an element by its key.
				* @param key	The key that identifies the element
				* @return the value associated with the key, populated if key is found; NULL if not
				* @reqid 001.0063
				*/
				const VALUE* operator[] ( const TCHAR* key ) const
				{
                    typename std::map<std::string,VALUE*>::const_iterator element = m_map.find( key );
                    if ( element != m_map.end() )
                    {
                        return element->second;
                    }
					return NULL;
				}
            
                virtual void Assign( const StringKeyedTree& source, Error& error, 
                                    const Cloner< VALUE >& cloner = Cloner< VALUE >() )
                {
                    Clear();
                    
                    // Assign
                    for ( typename std::map<std::string, VALUE*>::const_iterator element = source.m_map.begin();
                         element != source.m_map.end(); element++ )
                    {
                        VALUE* clone = cloner( element->second, error );
                        if ( error )
                        {
                            return;
                        }
                        
                        m_map[ element->first ] = clone;
                    }
                }
			
#ifdef S11N

#define INITIAL_SERIALIZATION_BUFFER_LENGTH 1024

			/**
			 * Needed to save key values without pulling heavy machinery
			 * @reqid N/A.
			 */
			class KeyString
			{
			public:
				KeyString( const TCHAR* value )
					: m_value( value )
				{
				}

				KeyString(){}

				void Deserialize( IDeserializer& deserializer, Error& error )
				{
                    const TCHAR* m_value = NULL;
					DESERIALIZE_SCALAR( &m_value, deserializer, error,  );
                    this->m_value = m_value;
				}

				void Serialize( ISerializer& emitter, Error& error ) const
				{
					SERIALIZE_SCALAR_WITH_NAME( m_value.c_str(), _T("m_value"), emitter, error, );
				}

				const TCHAR* GetValue()
				{
					return m_value.c_str();
				}

				void SetValue( const TCHAR* value )
				{
					m_value = value;
				}

			private:
				std::basic_string<TCHAR> m_value;
			};

			/**
			 * Serialization. Dump all the children as a map.
			 * @param emitter - the emitter to serialize to.
			 * @param error - error info.
			 * @reqid N/A
			 */
			void Serialize( ISerializer& emitter, Error& error ) const
			{
				for ( typename std::map<std::string, VALUE*>::const_iterator element = m_map.begin();
					element != m_map.end(); element++ )
				{
					VALUE& value = *(element->second);

					KeyString key( element->first.c_str() );
					if ( error )
					{
						return;
					}

					emitter.SerializeObject<KeyString>( key, element->first.c_str(), "KeyString", error );
					if ( error )
					{
						return;
					}

					std::basic_string<TCHAR> tempBuffer = element->first;
					tempBuffer.append(_T("--UNIQUE"));

					emitter.SerializeObject<VALUE>( value, tempBuffer.c_str(), "VALUE", error );
					if ( error )
					{
						return;
					}
				}
			}

			/**
			 * Deserializes a key collection from a deserializer.
			 * @param deserializer - Source for deserialization.
			 * @param error - Error info.
			 * @reqid N/A
			 */
			void Deserialize( IDeserializer& deserializer, Error& error )
			{
				typedef TreeCollection<VALUE, 256, TCHAR> ParentTree;

				std::basic_string<TCHAR> key;
				bool ret = deserializer.EnterFirstNestedObject( key, error );
				if ( error )
				{
					return;
				}

				while ( ret )
				{
					KeyString theKey;
					theKey.Deserialize( deserializer, error );
					if ( error )
					{
						return;
					}

					deserializer.LeaveNestedObject( error );
					if ( error )
					{
						return;
					}

					ret = deserializer.EnterNextNestedObject( key, error );
					if ( error )
					{
						return;
					}
					if ( !ret )
					{
						SET_ERROR( error, ParentTree::ErrorCode_DeserializationError, "Internal error." );
						return;
					}

					VALUE* newValue = VALUE::DeserializeNew( deserializer, error );
					if ( error )
					{
						return;
					}

					m_map[ theKey.GetValue() ] = newValue;

					deserializer.LeaveNestedObject( error );
					if ( error )
					{
						return;
					}

					ret = deserializer.EnterNextNestedObject( key, error );
					if ( error )
					{
						return;
					}
				}
			}
            
            std::map<std::string, VALUE*>* GetKeyStringMap()
            {
                return &m_map;
            }

            /**
             * Clear the collection and release all values.
             * @reqid N.A
             */
            void Clear()
            {
                for ( typename std::map<std::string, VALUE*>::iterator element = m_map.begin();
                     element != m_map.end(); element++ )
                {
                    delete element->second;
                }
                
                m_map.clear();
            }

        protected:
//			/**
//			 * Serialization of a single node of the tree.
//			 * @param emitter - the emitter to serialize to.
//			 * @param buffer - the current 'path' to this node.
//			 * @param curBufferUsage - length of the path.
//			 * @param error - error info.
//			 * @reqid N/A
//			 */
//			void Serialize( ISerializer& emitter, std::basic_string<TCHAR>& buffer, unsigned int curBufferUsage, 
//				Error& error ) const
//			{
//
//			}
#endif
            
            std::map<std::string, VALUE*> m_map;

		};

		//template<class C1, class C2>
		//class MAPTEXTTYPES_API Pair
		//{
		//public:
		//	/**
		//	 * Default constructor. Takes ownership over the parameters.
		//	 * @param first - First pair member.
		//	 * @param second - Second pair member.
		//	 * @reqid N/A.
		//	 */
		//	Pair( C1* first, C2* second )
		//		: m_first(first), m_firstOwned(true), m_second(second), m_secondOwned(true)
		//	{
		//	}

		//	/**
		//	 * Ownership over the parameters can be fully controlled.
		//	 * @param first - First pair member.
		//	 * @param shouldTakeOwnershipOfFirst - First pair member ownership should be taken?
		//	 * @param second - Second pair member.
		//	 * @param shouldTakeOwnershipOfSecond - Second pair member ownership should be taken?
		//	 * @reqid N/A.
		//	 */
		//	Pair( C1* first, bool shouldTakeOwnershipOfFirst, C2* second, bool shouldTakeOwnershipOfSecond )
		//		: m_first(first), m_firstOwned(shouldTakeOwnershipOfFirst), m_second(second), m_secondOwned(shouldTakeOwnershipOfSecond)
		//	{
		//	}

		//	virtual ~Pair()
		//	{
		//		if ( m_firstOwned )
		//		{
		//			delete m_first;
		//		}
		//		if ( m_secondOwned )
		//		{
		//			delete m_second;
		//		}
		//	}

		//private:
		//	C1* m_first;
		//	bool m_firstOwned;
		//	C2* m_second;
		//	bool m_secondOwned;
		//};

	} // namespace Dtp
} // namespace MapText

#endif // DTP_COLLECTION_H
