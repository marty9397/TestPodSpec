/**************************************************
 * Copyright (c)2009 Maptext, Inc. All rights reserved.
 * www:   http://www.maptext.com
 * email: support@maptext.com
 *-------------------------------------------------
 * Workfile: $HeadURL: https://martin@mtsrv.maptext.local:8443/svn/maptext/Products/MapText/DTP/dev/src/LabelingEngine/MapTextTypes/Memory.h $
 * 
 * Revision: $Revision: 25362 $
 *
 * Last modified on: $Date: 2011-10-31 09:58:32 -0400 (Mon, 31 Oct 2011) $, by: $Author: mfraczak $
 * 
 *************************************************/

#ifndef DTP_MEMORY_H
#define DTP_MEMORY_H

namespace MapText
{
	namespace Dtp
	{
		//========================================================================

		//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
		//  Copyright (c) 2001, 2002 Peter Dimov
		//
		//  Distributed under the Boost Software License, Version 1.0. (See
		//  accompanying file LICENSE_1_0.txt or copy at
		//  http://www.boost.org/LICENSE_1_0.txt)
		//
		//  http://www.boost.org/libs/smart_ptr/scoped_ptr.htm
		//

		//  scoped_ptr mimics a built-in pointer except that it guarantees deletion
		//  of the object pointed to, either on destruction of the scoped_ptr or via
		//  an explicit reset(). scoped_ptr is a simple solution for simple needs;
		//  use shared_ptr or std::auto_ptr if your needs are more complex.

		/**
		 * A scoped smart pointer implementation. It's shared by the simple and array
		 * children versions.
		 * This is an abstract base class leaving the deletion itself to be implemented
		 * by the proper derived class.
		 */
		template<class T> class AbstractScopedPointer // noncopyable
		{

		protected:
			T* m_ptr;

		private:

			/**
			 * Private copy constructor.
			 * @param copy instance to copy
			 * @reqid 001.0063
			 */ 
			AbstractScopedPointer(AbstractScopedPointer const &);

			/**
			 * Private assignment operator.
			 * @param source source instance
			 * @return reference to self
			 * @reqid 001.0063
			 */
			AbstractScopedPointer & operator=(AbstractScopedPointer const &);

			typedef AbstractScopedPointer<T> this_type;

			/**
			 * Private equality operator.
			 * @param rhv right-hand value
			 * @reqid 001.0063
			 */
			void operator==( AbstractScopedPointer const& ) const;

			/**
			 * Private inequality operator.
			 * @param rhv right-hand value
			 * @reqid 001.0063
			 */
			void operator!=( AbstractScopedPointer const& ) const;

			/**
			 * Delete a pointer. This needs to be implemented by the derived classes.
			 * @reqid 001.0063
			 */
			virtual void Delete( T* ptr ) = 0;

		public:

			typedef T element_type;

			/**
			 * Constructor.
			 * @param p the pointer to wrap
			 * @reqid 001.0063
			 */
			explicit AbstractScopedPointer(T* p = 0): m_ptr(p) // never throws
			{
			}

			/**
			 * Destructor.
			 * @reqid 001.0063
			 */
			~AbstractScopedPointer() // never throws
			{
			}

			/**
			 * Assign a new pointer. The old one gets released.
			 * @param p - new pointer
			 * @reqid 001.0063
			 */
			void Reset(T* ptr = 0) // never throws
			{
				if ( ptr != m_ptr )
				{
					Delete( m_ptr );
					m_ptr = ptr;
				}
			}

			/**
			 * Cast to type reference.
			 * @return - Reference to the held object.
			 * @reqid 001.0063
			 */
			T & operator*() const // never throws
			{
				assert(m_ptr != 0);
				return *m_ptr;
			}

			/**
			 * Cast to type pointer.
			 * @return - Pointer to the held object.
			 * @reqid 001.0063
			 */
			T* operator->() const // never throws
			{
				assert(m_ptr != 0);

				return m_ptr;
			}

			/**
			 * Get the raw pointer.
			 * @return - Held pointer.
			 * @reqid 001.0063
			 */
			T* Get() const // never throws
			{
				return m_ptr;
			}

 			/**
 			 * Return wrapped pointer and give up ownership
 			 * @return - The held pointer.
 			 * @reqid 001.0063
 			 */
 			T* Release()
 			{
 				T* temp = m_ptr;
 				m_ptr = NULL;
 				return temp;
 			}


			// implicit conversion to "bool"

			typedef T* this_type::*unspecified_bool_type;

			/**
			 * Safe-bool idiom to avoid undesirable casting.
			 * @reqid 001.0063
			 */
			operator unspecified_bool_type() const // never throws
			{
				return m_ptr == 0;//? 0: &this_type::ptr;
			}

			/**
			 * Bool test for null.
			 * @return - True if held pointer is NULL, false otherwise.
			 * @reqid 001.0063
			 */
			bool operator! () const // never throws
			{
				return m_ptr == 0;
			}

			/**
			 * Swap content with another scoped pointer.
			 * @param b - The pointer to swap content with.
			 * @reqid 001.0063
			 */
			void swap(AbstractScopedPointer & b) // never throws
			{
				T* tmp = b.m_ptr;
				b.m_ptr = m_ptr;
				m_ptr = tmp;
			}
		};

 		/**
 		* Regular (non-array) scoped pointer
 		* @reqid 001.0063
 		*/
 		template<class T>
 		class ScopedPointer : public AbstractScopedPointer<T>
 		{	// wrap an object pointer to ensure destruction
 		public:

 			/**
 			 * Constructor.
 			 * @param ptr - Pointer to wrap.
 			 * @reqid 001.0063
 			 */
 			explicit ScopedPointer(T*ptr = 0)
 				: AbstractScopedPointer<T>(ptr)
 			{	// construct from object pointer
 			}
 
 			 /**
 			  * Destructor
 			  * @reqid 001.0063
 			  */
 			 ~ScopedPointer()
 			 {
				 Delete( AbstractScopedPointer<T>::m_ptr );
 			 }
 
 		private:

 			/**
 			 * No longer abstract. Delete the reguilar pointer.
 			 * @reqid 001.0063
 			 */
 			void Delete( T* ptr )
 			{
				delete ptr;
 			}
 		};
 
 		/**
 		* Array scoped pointer
 		* @reqid 001.0063
 		*/
 		template<class T>
 		class ScopedArrayPointer : public AbstractScopedPointer<T>
 		{
 		public:


 			explicit ScopedArrayPointer( T* ptr = 0 )
 				: AbstractScopedPointer<T>( ptr )
 			{
 			}
 
 			/**
 			 * Destructor.
 			 * @reqid 001.0063
 			 */
 			~ScopedArrayPointer()
 			{
 				Delete( AbstractScopedPointer<T>::m_ptr );
 			}
 		private:

 			/**
 			 * No longer abstract. Delete the array pointer.
 			 * @reqid 001.0063
 			 */
 			virtual void Delete( T* ptr )
 			{
				delete[] ptr;
 			}
 		};

	} // namespace Dtp
} // namespace MapText

#endif // DTP_MEMORY_H