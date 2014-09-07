#define LP3_TEST_MODULE AllocatorTest
#include <Lp3/Test.h>

#include <Lp3/Assert.h>
#include <Lp3/LE.h>
#include <Lp3/Exception.h>

//#include <Lp3/Engine/Memory/Allocator.h>
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

LP3_TEST(UseAllocatorWithStrings) {

    // const size_t requestedSize = 6 * 1024 * 1024;
    // GlobalHeapPtr globalHeap = GlobalHeap::GetGlobalHeap();
    // MemoryStackPtr stack = MemoryStack::Create(globalHeap.get(), 10,
    //                                            requestedSize);
    // const size_t originalSize = stack->GetBytesFree();
    // {
    //     auto mac = stack->make_intrusive<MemoryAwareClass>("HELLO");
    //     LP3_ASSERT_EQUAL(mac->myString, "HELLO");
    //     LP3_ASSERT_EQUAL(mac->host, stack);
    //     LP3_ASSERT_EQUAL(stack->GetBytesUsed(), sizeof(MemoryAwareClass)
    //                                             + sizeof(std::string));
    //     LP3_ASSERT_EQUAL(originalSize - sizeof(MemoryAwareClass)
    //                         - sizeof(std::string),
    //                      stack->GetBytesFree());
    // }
    // LP3_ASSERT_EQUAL(originalSize, stack->GetBytesFree());
}
