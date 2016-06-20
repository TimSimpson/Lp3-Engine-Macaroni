#define LP3_TEST_MODULE PrimitivesTest
#include <Lp3/Test.h>

#include <Lp3/Assert.h>
#include <Lp3/LE.h>
#include <Lp3/Exception.h>

LP3_TEST(LP3_TEST_MAIN)
{
	// Make sure these match the sizes we're expecting...
	LP3_ASSERT_EQUAL(sizeof(Lp3::Int16), 2);
	LP3_ASSERT_EQUAL(sizeof(Lp3::UInt16), 2);
	LP3_ASSERT_EQUAL(sizeof(Lp3::Int32), 4);
	LP3_ASSERT_EQUAL(sizeof(Lp3::UInt32), 4);
	LP3_ASSERT_EQUAL(sizeof(Lp3::Int64), 8);
	LP3_ASSERT_EQUAL(sizeof(Lp3::UInt64), 8);
	LP3_ASSERT_EQUAL(sizeof(Lp3::Float32), 4);
    LP3_ASSERT_EQUAL(sizeof(Lp3::Float64), 8);
}
