/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __SharedPtr_H__
#define __SharedPtr_H__

#include "OgrePrerequisites.h"

namespace Ogre {
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/

	/// The method to use to free memory on destruction
	enum SharedPtrFreeMethod
	{
		/// Use OGRE_DELETE to free the memory
		SPFM_DELETE,
		/// Use OGRE_DELETE_T to free (only MEMCATEGORY_GENERAL supported)
		SPFM_DELETE_T,
		/// Use OGRE_FREE to free (only MEMCATEGORY_GENERAL supported)
		SPFM_FREE,
		/// Do not free or delete pointer when releasing
		SPFM_NO_FREE
	};

	struct shared_ptr_static_cast_tag {};
	struct shared_ptr_const_cast_tag {};
	struct shared_ptr_dynamic_cast_tag {};

	/** Reference-counted shared pointer, used for objects where implicit destruction is 
        required. 
    @remarks
        This is a standard shared pointer implementation which uses a reference 
        count to work out when to delete the object. 
	@par
		If OGRE_THREAD_SUPPORT is defined to be 1, use of this class is thread-safe.
    */
	template<class T> class SharedPtr
	{
	protected:
		T* pRep;
		unsigned int* pUseCount;
		SharedPtrFreeMethod useFreeMethod; // if we should use OGRE_FREE instead of OGRE_DELETE
	public:
		OGRE_AUTO_SHARED_MUTEX // public to allow external locking
		/** Constructor, does not initialise the SharedPtr.
			@remarks
				<b>Dangerous!</b> You have to call bind() before using the SharedPtr.
		*/
		SharedPtr() : pRep(0), pUseCount(0), useFreeMethod(SPFM_NO_FREE)
        {
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
        }

		/** Constructor.
		@param rep The pointer to take ownership of
		@param freeMode The mechanism to use to free the pointer
		*/
        template< class Y >
		explicit SharedPtr( Y* rep, SharedPtrFreeMethod freeMethod = SPFM_DELETE ) 
			: pRep( rep )
			, pUseCount(pRep ? OGRE_NEW_T(unsigned int, MEMCATEGORY_GENERAL)(1) : 0)
			, useFreeMethod(freeMethod)
		{
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
			OGRE_NEW_AUTO_SHARED_MUTEX
		}

		template< class Y >
		SharedPtr( const SharedPtr<Y>& r )
            : pRep(0), pUseCount(0), useFreeMethod(SPFM_NO_FREE)
		{
			// lock & copy other mutex pointer

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
				pRep = r.getPointer();
			    pUseCount = r.useCountPointer();
				useFreeMethod = r.freeMethod();
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
		}

		SharedPtr(const SharedPtr& r)
            : pRep(0), pUseCount(0), useFreeMethod(SPFM_NO_FREE)
		{
			// lock & copy other mutex pointer
            
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = r.pRep;
			    pUseCount = r.pUseCount; 
				useFreeMethod = r.useFreeMethod;
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount); 
			    }
            }
		}
		

		SharedPtr& operator=(const SharedPtr& r) {
			if (pRep == r.pRep)
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		template< class Y>
		SharedPtr& operator=(const SharedPtr<Y>& r) {
			if (pRep == r.getPointer())
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		virtual ~SharedPtr() {
            release();
		}


		inline T& operator*() const { assert(pRep); return *pRep; }
		inline T* operator->() const { assert(pRep); return pRep; }
		inline T* get() const { return pRep; }


		// implicit conversion to "bool"
		typedef T* SharedPtr<T>::*unspecified_bool_type;
		operator unspecified_bool_type() const // never throws
		{
			return pRep == 0 ? 0 : &SharedPtr<T>::pRep;
		}

		//boost::shared_ptr interface adaptation
		inline void reset()	{
			SharedPtr<T> tmp;
			swap(tmp);
		}
		inline void reset(T* rep, SharedPtrFreeMethod freeMethod = SPFM_DELETE)	{
			SharedPtr<T> tmp(rep, freeMethod);
			swap(tmp);
		}
		inline unsigned int use_count()	{
			return useCount();
		}
		template<typename Y>
		SharedPtr( SharedPtr<Y> const &r, shared_ptr_dynamic_cast_tag )
			: pRep(0) , pUseCount(0) , useFreeMethod(SPFM_NO_FREE)
		{
			OGRE_SET_AUTO_SHARED_MUTEX_NULL
			OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
			{
				OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
				pRep = dynamic_cast<T*>(r.getPointer());
				if(pRep)
				{
					OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
						pUseCount = r.useCountPointer();
					useFreeMethod = r.freeMethod();
					// Handle zero pointer gracefully to manage STL containers
					if(pUseCount) {
						++(*pUseCount);
					}
				}
			}
		}


		/** Binds rep to the SharedPtr.
			@remarks
				Assumes that the SharedPtr is uninitialised!
		*/
		void bind(T* rep, SharedPtrFreeMethod freeMethod = SPFM_DELETE )
		{
			setNull();
			OGRE_NEW_AUTO_SHARED_MUTEX
			OGRE_LOCK_AUTO_SHARED_MUTEX
			pRep = rep;
			if( pRep != 0 )pUseCount = OGRE_NEW_T(unsigned int, MEMCATEGORY_GENERAL)(1);
			useFreeMethod = freeMethod;
		}

		inline bool unique() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount == 1; }
		inline unsigned int useCount() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount; }
		inline unsigned int* useCountPointer() const { return pUseCount; }

		inline T* getPointer() const { return pRep; }
		inline T& getReference()const { assert(pRep); return *pRep; }
		inline SharedPtrFreeMethod freeMethod() const { return useFreeMethod; }

		inline bool isNull(void) const { return pRep == 0; }

        inline void setNull(void) { 
			if (pRep)
			{
				// can't scope lock mutex before release in case deleted
				release();
				pRep = 0;
				pUseCount = 0;
			}
        }

    protected:

        inline void release(void)
        {
			bool destroyThis = false;

            /* If the mutex is not initialized to a non-zero value, then
               neither is pUseCount nor pRep.
             */

            OGRE_MUTEX_CONDITIONAL(OGRE_AUTO_MUTEX_NAME)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				OGRE_LOCK_AUTO_SHARED_MUTEX
				if (pUseCount)
				{
					if (--(*pUseCount) == 0)
					{
						destroyThis = true;
	                }
				}
            }
			if (destroyThis)
				destroy();

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
        }

        virtual void destroy(void)
        {
            // IF YOU GET A CRASH HERE, YOU FORGOT TO FREE UP POINTERS
            // BEFORE SHUTTING OGRE DOWN
            // Use setNull() before shutdown or make sure your pointer goes
            // out of scope before OGRE shuts down to avoid this.
			if( pRep != 0 )
			{
				switch(useFreeMethod)
				{
				case SPFM_DELETE:
					OGRE_DELETE pRep;
					break;
				case SPFM_DELETE_T:
					OGRE_DELETE_T(pRep, T, MEMCATEGORY_GENERAL);
					break;
				case SPFM_FREE:
					OGRE_FREE(pRep, MEMCATEGORY_GENERAL);
					break;
				};
			}
			pRep = 0;

			// use OGRE_FREE instead of OGRE_DELETE_T since 'unsigned int' isn't a destructor
			// we only used OGRE_NEW_T to be able to use constructor
            if( pUseCount != 0 )
				OGRE_FREE(pUseCount, MEMCATEGORY_GENERAL);
			pUseCount = 0;

			OGRE_DELETE_AUTO_SHARED_MUTEX
        }

		virtual void swap(SharedPtr<T> &other) 
		{
			std::swap(pRep, other.pRep);
			std::swap(pUseCount, other.pUseCount);
			std::swap(useFreeMethod, other.useFreeMethod);
#if OGRE_THREAD_SUPPORT
			std::swap(OGRE_AUTO_MUTEX_NAME, other.OGRE_AUTO_MUTEX_NAME);
#endif
		}
	};

	template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}
	
	template<class T, class U> inline bool operator<(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return std::less<const void*>()(a.get(), b.get());
	}
	/** @} */
	/** @} */
}



#endif
