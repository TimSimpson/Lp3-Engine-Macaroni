#include <Lp3/Engine/Memory/Host.h>


namespace Lp3 { namespace Engine { namespace Memory {

//http://en.cppreference.com/w/cpp/concept/Allocator

template<HostPtrType>
class HostAllocator
{
    template<T>
    class Allocator
    {
    private:
        HostPtrType host;

    public:
        typedef T                  value_type;
        typedef value_type *       pointer;
        typedef const value_type * const_pointer;
        typedef value_type &       reference;
        typedef const value_type & const_reference;
        typedef std::size_t        size_type;
        typedef std::ptrdiff_t     difference_type;
        //typedef ? const_void_pointer;

        // Method of creating an allocator like this for type U.
        template<typename U>
        struct rebind {
            typedef Allocator<U> other;
        };

        Allocator(HostPtrType host)
        // Create Allocator.
        :   host(host)
        {
        }

        Allocator(const Allocator & other)
        :   host(other.host)
        {
        }

        template<typename U>
        Allocator(const Allocator<U> & other)
        :   host(other.host)
        {
        }

        Allocator(Allocator&& other)
        :   host(other.host)
        {
        }

        Allocator & operator=(const Allocator & other)
        {
            return Allocator(other);
        }

        template<typename U>
        Allocator & operator=(const Allocator<U> & other)
        {
            return Allocator(other);
        }

        // const T & operator * () const
        // {
        //     return *ptr;
        // }

        pointer allocate(size_type count)
        {
            // Actually allocate memory for sizeof(T) * count.
            // Abort program if it doesn't fit.
            host->Alloc(count * sizeof(T));
        }

        // There is another form of allocate accepting a pointer
        // "to aid locality".

        void deallocate(pointer p, size_type count)
        {
            host->Free(p);
        }

        // size_type max_size()
        // {
        //     return
        // }

        bool operator == (const Allocator & rhs) const
        {
            // True only if rhs can deallocate things this allocator has allocated
            // and vice-versa.
            return this->host == rhs->host;
        }

        bool operator != (const Allocator & rhs) const
        {
            return !(operator==(rhs));
        }


    };
}; // End Host Allocator

} } }
