#define LP3_TEST_MODULE MemoryStackTest
#include <Lp3/Test.h>

#include <Lp3/Assert.h>
#include <Lp3/LE.h>
#include <Lp3/Exception.h>

#include <Lp3/Engine/Memory/GlobalHeap.h>
#include <Lp3/Engine/Memory/GlobalHeapPtr.h>
#include <Lp3/Engine/Memory/Host.h>
#include <Lp3/Engine/Memory/ManagedPointer.h>
#include <Lp3/Engine/Memory/MemoryStackPtr.h>
#include <Lp3/Engine/Memory/MemoryStack.h>
#include <Lp3/Engine/Memory/MemoryStackEntryFooter.h>
#include <Lp3/Engine/Memory/MemoryStackException.h>
#include <string>

using namespace Lp3::Engine::Memory;

struct Pod
{
    Lp3::Byte b1;
    Lp3::Byte b2;
    Lp3::Byte b3;
    Lp3::Byte b4;
};

class SimpleClass
{
private:
    std::string & msg;
public:
    SimpleClass(std::string & msg)
        : msg(msg)
    {
        msg = "created";
    }

public: ~SimpleClass()
    {
        msg = "destroyed" ;
    }
};

LP3_TEST(Memory_Stack_throws_when_too_many_allocations)
{
    GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    const size_t requestedSize = 0;
    MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 0, 0);
    LP3_ASSERT_THROW(stack->Alloc(4), MemoryStackException);
}

LP3_TEST(Memory_Stack_throws_when_too_many_allocations_2)
{
    GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    const size_t requestedSize = 0;
    MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 0, 0);
    #ifdef LP3_COMPILE_TARGET_DREAMCAST
        LP3_ASSERT_THROW({
            new (stack.get()) Pod;
        }, MemoryStackException);
    #else
        try
        {
            Pod * pod = new (stack.get()) Pod;
            LP3_FAIL("Should not have reached this point.");
        }
        catch (const MemoryStackException & e)
        {
            LP3_ASSERT_EQUAL(e.actualEntryCount, 0);
            LP3_ASSERT_EQUAL(e.reservedEntryCount, 0);
        }
    #endif
}

LP3_TEST(MemoryStackTests)
    // This test creates a memory stack, puts some stuff on it, and makes sure
    // that as things are deleted the various accessors return the correct
    // metrics.
{
    const size_t requestedSize = 6 * 1024 * 1024;
    GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 10,
        requestedSize);
	const size_t expectedTotalSize = sizeof(MemoryStack)+requestedSize
		+ sizeof(MemoryStackEntryFooter)* 10;
    // Initial state
	LP3_ASSERT_EQUAL(expectedTotalSize, stack->GetSize());
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      0);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),      requestedSize);


    // routine allocation.
    Pod * pod = new (stack.get()) Pod;
    LP3_ASSERT_EQUAL(sizeof(Pod), stack->GetBytesUsed());
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      sizeof(Pod));
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),      requestedSize - sizeof(Pod));

    // Routine allocation 2
    char * const text = static_cast<char *>(stack->Alloc(1024));
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      sizeof(Pod) + 1024);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(), requestedSize - sizeof(Pod) - 1024);

    // Routine allocation 3
    Pod * pod2 = stack->New<Pod>();  // Fancy style.
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      (sizeof(Pod) * 2)+ 1024);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),
                                      requestedSize - (sizeof(Pod) * 2) - 1024);

    std::string message = "construct";
    // Routine allocation 4
    SimpleClass * sc = new (stack.get()) SimpleClass(message);
    LP3_ASSERT_EQUAL("created", message);
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),
                             ((sizeof(Pod) * 2) + 1024 + sizeof(SimpleClass)));
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),
             (requestedSize - (sizeof(Pod) * 2) - 1024 - sizeof(SimpleClass)));

    // Make sure the entry footer has the inuse flag set.
    char * scPtr = static_cast<char *>(static_cast<void *>(sc));
    scPtr += sizeof(sc);
    scPtr += (sizeof(void *));
    Lp3::UInt32 * flagPtr = static_cast<Lp3::UInt32 *>(
        static_cast<void *>(scPtr));
    LP3_ASSERT_EQUAL_MESSAGE(0x830C, *flagPtr,
        "Flag must state memory is in use.");

    // routine deallocation - removes last item, behaves intuitively.
    stack->Delete(sc);
    LP3_ASSERT_EQUAL_MESSAGE("destroyed", message,
        "Destroy should call the destructor.");
    LP3_ASSERT_EQUAL_MESSAGE(0xAC41, *flagPtr,
        "Flag must state memory is free.");
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      (sizeof(Pod) * 2)+ 1024);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),
                                      requestedSize - (sizeof(Pod) * 2) - 1024);

    // Deallocation two- we delete something low in the stack. The space
    // can't be claimed because there are allocations later on.
    stack->Delete(pod);
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      (sizeof(Pod) * 2)+ 1024);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),
                                      requestedSize - (sizeof(Pod) * 2) - 1024);

    // Deallocation three - again, its at the bottom so nothing happens.
    stack->Free(text);
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      (sizeof(Pod) * 2)+ 1024);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),
                                      requestedSize - (sizeof(Pod) * 2) - 1024);

    // Deallocation four - with the third allocation deleted, it
    // should now iterate through the footers and free up all space until
    // the start.
    stack->Delete(pod2);
    LP3_ASSERT_EQUAL(stack->GetBytesAvailable(), requestedSize);
    LP3_ASSERT_EQUAL(stack->GetBytesUsed(),      0);
    LP3_ASSERT_EQUAL(stack->GetBytesFree(),      requestedSize);
}

LP3_TEST(MemoryStack2Tests)
{
    // Create a memory stack that actually lives on the stack (LOL?)
    const size_t requestedSize = 1024;
    MemoryStack2<10, requestedSize> stack;

    const size_t expectedTotalSize = sizeof(MemoryStack)+requestedSize
        + sizeof(MemoryStackEntryFooter)* 10;
    // Initial state
    LP3_ASSERT_EQUAL(expectedTotalSize, stack.GetSize());
    LP3_ASSERT_EQUAL(requestedSize, stack.GetBytesAvailable());
    LP3_ASSERT_EQUAL(0, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize, stack.GetBytesFree());
    // routine allocation.
    Pod * pod = new (&stack) Pod;
    LP3_ASSERT_EQUAL(sizeof(Pod), stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - sizeof(Pod), stack.GetBytesFree());
    // Routine allocation 2
    char * const text = static_cast<char *>(stack.Alloc(1024));
    LP3_ASSERT_EQUAL(sizeof(Pod) + 1024, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - sizeof(Pod) - 1024,
        stack.GetBytesFree());
    // Routine allocation 3
    Pod * pod2 = new (&stack) Pod;
    LP3_ASSERT_EQUAL((sizeof(Pod) * 2)+ 1024, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - (sizeof(Pod) * 2) - 1024,
        stack.GetBytesFree());

    std::string message = "construct";
    // Routine allocation 4
    SimpleClass * sc = new (&stack) SimpleClass(message);
    LP3_ASSERT_EQUAL("created", message);
    LP3_ASSERT_EQUAL(((sizeof(Pod) * 2) + 1024 + sizeof(SimpleClass)),
        stack.GetBytesUsed());
    LP3_ASSERT_EQUAL((requestedSize - (sizeof(Pod) * 2) - 1024 -
        sizeof(SimpleClass)),
        stack.GetBytesFree());
    // Make sure the entry footer has the inuse flag set.
    char * scPtr = static_cast<char *>(static_cast<void *>(sc));
    scPtr += sizeof(sc);
    scPtr += (sizeof(void *));
    Lp3::UInt32 * flagPtr = static_cast<Lp3::UInt32 *>(
        static_cast<void *>(scPtr));
    LP3_ASSERT_EQUAL_MESSAGE(0x830C, *flagPtr,
        "Flag must state memory is in use.");

    // routine deallocation - removes last item, behaves intuitively.
    stack.Delete(sc);
    LP3_ASSERT_EQUAL_MESSAGE("destroyed", message,
        "Destroy should call the destructor.");
    LP3_ASSERT_EQUAL_MESSAGE(0xAC41, *flagPtr,
        "Flag must state memory is free.");
    LP3_ASSERT_EQUAL((sizeof(Pod) * 2)+ 1024, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - (sizeof(Pod) * 2) - 1024,
        stack.GetBytesFree());

    // Deallocation two- we delete something low in the stack. The space
    // can't be claimed because there are allocations later on.
    stack.Delete(pod);
    LP3_ASSERT_EQUAL((sizeof(Pod) * 2)+ 1024, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - (sizeof(Pod) * 2) - 1024,
        stack.GetBytesFree());

    // Deallocation three - again, its at the bottom so nothing happens.
    stack.Free(text);
    LP3_ASSERT_EQUAL((sizeof(Pod) * 2)+ 1024, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - (sizeof(Pod) * 2) - 1024,
        stack.GetBytesFree());

    // Deallocation four - with the third allocation deleted, it
    // should now iterate through the footers and free up all space until
    // the start.
    stack.Delete(pod2);
    LP3_ASSERT_EQUAL(0, stack.GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize, stack.GetBytesFree());
}

// TODO: Make a MemoryStack, create more objects that it has entries for.

// TODO: Make a MemoryStack, reserve more memory than space

// TODO: Make a memory stack, add a bunch of stuff, then delete the memory
//       stack and ensure all destructors were called.

LP3_TEST(ConstructAndDestruct)
{
    const size_t requestedSize = 6 * 1024 * 1024;
    GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 10,
        requestedSize);
    // routine allocation.
    auto * pod = stack->New<Pod>();
    LP3_ASSERT_EQUAL(sizeof(Pod), stack->GetBytesUsed());
    LP3_ASSERT_EQUAL(requestedSize - sizeof(Pod), stack->GetBytesFree());

    // std::string message = "construct";
    // // Routine allocation 4
    // auto * sc = stack->New<SimpleClass>(message);
    // LP3_ASSERT_EQUAL("created", message);
    // LP3_ASSERT_EQUAL(((sizeof(Pod) * 2) + 1024 + sizeof(SimpleClass)),
    //     stack->GetBytesUsed());
    // LP3_ASSERT_EQUAL((requestedSize - (sizeof(Pod) * 2) - 1024 -
    //     sizeof(SimpleClass)),
    //     stack->GetBytesFree());

    // // routine deallocation - removes last item, behaves intuitively.
    // stack->Delete(sc);
    // LP3_ASSERT_EQUAL_MESSAGE("destroyed", message,
    //     "Destroy should call the destructor.");
    // LP3_ASSERT_EQUAL_MESSAGE(0xAC41, *flagPtr,
    //     "Flag must state memory is free.");
    // LP3_ASSERT_EQUAL((sizeof(Pod) * 2)+ 1024, stack->GetBytesUsed());
    // LP3_ASSERT_EQUAL(requestedSize - (sizeof(Pod) * 2) - 1024,
    //     stack->GetBytesFree());

    // stack->Delete(pod);
    // LP3_ASSERT_EQUAL(0, stack->GetBytesUsed());
    // LP3_ASSERT_EQUAL(requestedSize, stack->GetBytesFree());
}


struct KwazyConstructor
{
    const std::string blah;
    const unsigned int number;
    const bool other;

    KwazyConstructor(const std::string blah, unsigned int number, bool other)
    :   blah(blah),
        number(number),
        other(other)
    {

    }
};


LP3_TEST(ConstructMultipleArgs)
{
    const size_t requestedSize = 1024;
    MemoryStack2<10, requestedSize> stack;
    KwazyConstructor * kc = stack.New<KwazyConstructor>("hi", 5u, false);
    LP3_ASSERT_EQUAL(kc->blah, "hi");
    LP3_ASSERT_EQUAL(kc->number, 5);
    LP3_ASSERT_EQUAL(kc->other, false);
    stack.Delete(kc);
}


LP3_TEST(MakeUnique)
{
    const size_t requestedSize = 6 * 1024 * 1024;
    GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    auto kc2 = MemoryStack::null_unique_ptr<KwazyConstructor>();

    MemoryStack * dangerousPtr;
    size_t originalSize;
    {
        MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 10,
            requestedSize);
        originalSize = stack->GetBytesFree();
        {
            auto kc = stack->make_unique<KwazyConstructor>("hi", 5u, false);
            LP3_ASSERT_EQUAL(kc->blah, "hi");
            LP3_ASSERT_EQUAL(kc->number, 5);
            LP3_ASSERT_EQUAL(kc->other, false);
            LP3_ASSERT_EQUAL(originalSize - sizeof(KwazyConstructor),
                             stack->GetBytesFree());
        }
        LP3_ASSERT_EQUAL(originalSize, stack->GetBytesFree());

        kc2 = stack->make_unique<KwazyConstructor>("hi2", 5u, false);
        dangerousPtr = stack.get();
    }

    // Though inaccessible, it still exists due to the second unique pointer.
    LP3_ASSERT_EQUAL(originalSize - sizeof(KwazyConstructor),
                     dangerousPtr->GetBytesFree());
    kc2.reset();
    // Boost Test will determine if there is a leak with the stack- if not,
    // it should be gone at this point.
}



struct MemoryAwareClass
{
    SharedHostPtr host;
    std::string & myString;
    int referenceCount;

    MemoryAwareClass(SharedHostPtr host, const char * const strValue)
    :   host(host),
        myString(host->NewRef<std::string>(strValue)),
        referenceCount(0)
    {
    }

    ~MemoryAwareClass()
    {
        host->Delete(myString);
    }


};

void intrusive_ptr_add_ref(MemoryAwareClass * np)
{
    ++ np->referenceCount;
}

void intrusive_ptr_release(MemoryAwareClass * np)
{
    -- np->referenceCount;
    if (np->referenceCount <= 0)
    {
        np->host->Delete(np);
    }
}

LP3_TEST(MakeIntrusive)
{
    const size_t requestedSize = 6 * 1024 * 1024;
    GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 10,
                                               requestedSize);
    const size_t originalSize = stack->GetBytesFree();
    {
        auto mac = stack->make_intrusive<MemoryAwareClass>("HELLO");
        LP3_ASSERT_EQUAL(mac->myString, "HELLO");
        LP3_ASSERT_EQUAL(mac->host, stack);
        LP3_ASSERT_EQUAL(stack->GetBytesUsed(), sizeof(MemoryAwareClass)
                                                + sizeof(std::string));
        LP3_ASSERT_EQUAL(originalSize - sizeof(MemoryAwareClass)
                            - sizeof(std::string),
                         stack->GetBytesFree());
    }
    LP3_ASSERT_EQUAL(originalSize, stack->GetBytesFree());
}
