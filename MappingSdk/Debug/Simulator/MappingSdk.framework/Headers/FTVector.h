/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef    __FTVector__
#define    __FTVector__

#ifdef _MSC_VER
#include <glew.h>
#elif __APPLE__
#	include <OpenGLES/ES1/gl.h>
#	include <OpenGLES/ES1/glext.h>
#	include <OpenGLES/ES2/gl.h>
#	include <OpenGLES/ES2/glext.h>
#elif __GNUC__
#	include <GLES/gl.h>
#	include <GLES/glext.h>
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#endif

typedef struct verticeStruct_
{
	GLfloat point[2];
} verticeStruct;

/**
 * Provides a non-STL alternative to the STL vector
 */
template <typename FT_VECTOR_ITEM_TYPE>
class FTVector
{
    public:
        typedef FT_VECTOR_ITEM_TYPE value_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* iterator;
        typedef const value_type* const_iterator;
        typedef size_t size_type;

        FTVector()
        {
            Capacity = Size = 0;
            Items = 0;
            allocationFailed = false;
        }


        virtual ~FTVector()
        {
            clear();
        }

        FTVector& operator =(const FTVector& v)
        {
            reserve(v.capacity());

            iterator ptr = begin();
            const_iterator vbegin = v.begin();
            const_iterator vend = v.end();

            while(vbegin != vend)
            {
                *ptr++ = *vbegin++;
            }

            Size = v.size();
            return *this;
        }

        size_type size() const
        {
            return Size;
        }

        size_type capacity() const
        {
            return Capacity;
        }

        iterator begin()
        {
            return Items;
        }

        const_iterator begin() const
        {
            return Items;
        }

        iterator end()
        {
            return begin() + size();
        }

        const_iterator end() const
        {
            return begin() + size();
        }

        bool empty() const
        {
            return size() == 0;
        }

        reference operator [](size_type pos)
        {
            return(*(begin() + pos));
        }

        const_reference operator [](size_type pos) const
        {
            return *(begin() + pos);
        }

        void clear()
        {
            if(Capacity)
            {
                delete [] Items;
                Capacity = Size = 0;
                Items = 0;
            }
        }

		void resetSize()
		{
			Size = 0;
		}
    
        void incrementSize()
        {
            Size++;
        }

        void reserve(size_type n)
        {
            if(capacity() < n)
            {
                expand(n);
            }
        }
    
        void resizeCapacity(size_type n)
        {
            if(n < Capacity)
            {
                Capacity = n;
            }
        }

		bool contains(const value_type& x)
		{
			iterator ptr = begin();
			iterator ptrEnd = end();

			while(ptr != ptrEnd)
			{
				if(*ptr == x)
					return true;

				ptr++;
			}

			return false;
		}

        void push_back(const value_type& x)
        {
            if(size() == capacity())
            {
                expand();
                if(allocationFailed)
                {
                    return;
                }
            }

           (*this)[size()] = x;
            ++Size;
        }
    
        void push_back( const FTVector<FT_VECTOR_ITEM_TYPE>& vectorToPuchBack )
        {
            size_type oldSize = size();
            
            if(size()+ vectorToPuchBack.size() >= capacity())
            {
                expand(size()+ vectorToPuchBack.size());
            }
            
            for ( unsigned int i = (unsigned int)oldSize; i < oldSize+vectorToPuchBack.size(); i++ )
            {
                (*this)[i] = vectorToPuchBack[i-oldSize];
            }
            Size+= vectorToPuchBack.size();
        }

        void resize(size_type n, value_type x)
        {
            if(n == size())
            {
                return;
            }

            reserve(n);
            if(allocationFailed)
            {
                return;
            }
            
            iterator ibegin, iend;

            if(n >= Size)
            {
                ibegin = this->end();
                iend = this->begin() + n;
            }
            else
            {
                ibegin = this->begin() + n;
                iend = this->end();
            }

            while(ibegin != iend)
            {
                *ibegin++ = x;
            }

            Size = n;
        }
    
        bool GetAllocationError()
        {
            return allocationFailed;
        }


    protected:
        virtual bool expand(size_type capacity_hint = 0)
        {
			size_type new_capacity = capacity_hint;

			if(capacity_hint == 0)
			{
				new_capacity = (capacity() == 0) ? 256 : capacity() * 2;
			}

            value_type *new_items = new value_type[new_capacity];
            if(new_items == NULL)
            {
                allocationFailed = true;
                return false;
            }
            
            iterator ibegin = this->begin();
            iterator iend = this->end();
            value_type *ptr = new_items;

            while(ibegin != iend)
            {
                *ptr++ = *ibegin++;
            }

            if(Capacity)
            {
                delete [] Items;
            }

            Items = new_items;
            Capacity = new_capacity;
            
            return true;
        }

        size_type Capacity;
        size_type Size;
        value_type* Items;
        bool allocationFailed;
};

#endif  //  __FTVector__
