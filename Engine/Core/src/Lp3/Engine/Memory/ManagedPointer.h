#ifndef MEMORY_SMART_PTR_H
#define MEMORY_SMART_PTR_H

#include <Lp3/Assert.h>
#include <Lp3/Log.h>
#include <Lp3/Engine/Memory/Host.h>

// Much of this is copied from boost/shared_ptr:
// http://www.boost.org/boost/shared_ptr.hpp

namespace Lp3 { namespace Engine { namespace Memory {


template<typename T>
class HostObject
{
public:
    T * ptr;
    int refCount;
    Host * memHost;
};

/** Manages a pointer to memory that was created using a Host.
 * RESTRICTIONS:
 * Here's a hard and fast restriction I just thought up :
 * 1. The host that allocates the ptr structure has to also be the host
 *    for the HostObject created for this managing which will have
 *    it's Free() function called.
 * 2. The object T must be a class.
 * 3. Not thread safe.
 **/
template<typename T>
class ManagedPointer // ManagedPointer or HostPointer
{
public:
    typedef HostObject<T> HostObjectT;

    /** Creates a new ManagedPointer. */
    ManagedPointer(Host * host, T * tPtr)
    {
        void * ptr = host->Alloc(sizeof(HostObjectT));//<T>));
        objPtr = static_cast<HostObjectT *>(ptr);//<T> *>(ptr);
        objPtr->memHost = host;
        objPtr->refCount = 1;
        objPtr->ptr = tPtr;
    }

    /** Not for normal use - this creates a pointer looking
     * directly at the HostObject. */
    ManagedPointer(HostObjectT * object)
    {
        objPtr = object;
        objPtr->refCount ++;
    }

    /** Default constructor initializes to nullptr. */
    ManagedPointer()
    {
        objPtr = nullptr;
    }

    /** Copy ctor.
     * The rhs object is assumed to be in a good state. */
    ManagedPointer(const ManagedPointer & rhs)
    {
        objPtr = rhs.objPtr;
        if (objPtr != nullptr)
        {
            objPtr->refCount ++;
        }
    }

    T * GetAddress()
    {
        if (this->objPtr != nullptr)
        {
            return static_cast<T *>(objPtr->ptr);
        }
        return nullptr;
    }

    /** Assignment equals operator.
     * Like copy ctor BUT "this" already exists... so we must DEREF! */
    ManagedPointer<T> & operator = (const ManagedPointer & rhs)
    {
        if (this == &rhs) return *this; // Handle self assignment.

        if (objPtr != nullptr)
        {
            deref();
        }

        if (&rhs == nullptr)
        {
            objPtr = nullptr;
        }
        else
        {
            objPtr = rhs.objPtr;
        }

        if (objPtr != nullptr)
        {
            objPtr->refCount ++;
        }
        return *this;
    }

    /** Access via the -> operator. */
    T * operator ->()
    {
        return static_cast<T *>(objPtr->ptr);
    }

    /** Access via the dereference operator. */
    T &operator * ()
    {
        return *(static_cast<T *>(objPtr->ptr));
    }

    T * Get() const // never throws
    {
        return objPtr->ptr;
    }

    /** The number of references to this object. */
    inline int GetRefCount() const
    {
        if (objPtr == nullptr)
        {
            return -1;
        }
        return objPtr->refCount;
    }

    /** Deletes only the pointer, will only delete what is pointed to if
     * the ref count is below zero. */
    ~ManagedPointer()
    {
        if (objPtr != nullptr)
        {
            deref();
        }
    }


    operator bool () const
    {
        return objPtr != nullptr;
    }

    bool operator! () const // never throws
    {
        return objPtr == nullptr;
    }

    bool operator== (T * other) const // never throws
    {
        if (objPtr == nullptr)
        {
            if (other == nullptr)
                return true;
            return false;
        }
        return static_cast<T *>(objPtr->Ptr) == other;
    }

    bool operator!= (T * other) const // never throws
    {
        if (objPtr == nullptr)
        {
            if (other == nullptr)
                return false;
            return true;
        }
        return static_cast<T *>(objPtr->Ptr) != other;
    }

private:
    /** Stored pointer to the ref count, and HostPointer. */
    HostObjectT * objPtr;//<T> * objPtr;

    inline void deref()
    {
        LP3_ASSERT_TRUE_MESSAGE(objPtr->refCount > 0,
                     "ManagedPointer Error! objPtr->refCount <= 0!!!");
        objPtr->refCount --;
        if (objPtr->refCount <= 0)
        {
            LP3_LOG_DEBUG("...ManagedPointer ref <= 0, time to kill!");
            // Holy crap, that was the last ref! It is time to delete!
            Host * memHost = objPtr->memHost;
            // Destroy pointed to class object.
            memHost->Delete(objPtr->ptr);
            // Free the actual memory.
            memHost->Delete(objPtr); //Call memHost->Free instead?>
        }
        objPtr = nullptr;
    }
};

} } }

#endif

