#define LP3_TEST_MODULE PixelTest
#include <Lp3/Test.h>

#include <Lp3/Assert.h>
#include <Lp3/LE.h>
#include <Lp3/Exception.h>
#include <Lp3/Engine/Gfx/InternalPixel.h>
#include <boost/format.hpp>

using namespace Lp3::Engine::Gfx;
using namespace boost;

LP3_TEST(PixelStuff)
{
#ifdef LP3_COMPILE_TARGET_DREAMCAST
    InternalPixel pixel;
    pixel.SetF(1.0f, 1.0f, 1.0f, 1.0f);
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb)
    LP3_ASSERT_EQUAL(0xFFFF, pixel.argb);

    LP3_ASSERT_EQUAL(0xF000, 0xF << 12);
    LP3_ASSERT_EQUAL(0x0F00, 0xF << 8);
    LP3_ASSERT_EQUAL(0x00F0, 0xF << 4);
    LP3_ASSERT_EQUAL(0x000F, 0xF << 0);


    LP3_LOG_DEBUG("This is what we have: %x", ((0x0 << 12) | (0xF << 8)));
    LP3_LOG_DEBUG("This is what we have: %x", 0x0F00);
    LP3_ASSERT_EQUAL(0x0F00, ((0x0 << 12) | (0xF << 8)));

    pixel.SetF(0.0f, 0.0f, 0.0f, 1.0f);  // WORKS! 0xf000
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb);
    LP3_ASSERT_EQUAL(0xF000, pixel.argb);
    pixel.SetF(1.0f, 0.0f, 0.0f, 0.0f); //0xff00
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb);
    LP3_ASSERT_EQUAL(0x0F00, pixel.argb);
    pixel.SetF(0.0f, 1.0f, 0.0f, 0.0f); // 0x00ff?
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb);
    LP3_ASSERT_EQUAL(0x00F0, pixel.argb);
    pixel.SetF(0.0f, 0.0f, 1.0f, 0.0f); // 0xff00
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb);
    LP3_ASSERT_EQUAL(0x000F, pixel.argb);

    pixel.SetF(1.0f, 0.0f, 0.0f, 0.0f);
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb)
    LP3_ASSERT_EQUAL(0x0F00, pixel.argb);

    pixel.SetF(0.0f, 0.0f, 0.0f, 1.0f);
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb)
    LP3_ASSERT_EQUAL(0xF000, pixel.argb);

    pixel.SetF(0.0f, 1.0f, 0.0f, 0.0f);
    LP3_LOG_DEBUG("This is what we have: %x", pixel.argb)

    static_assert(sizeof(InternalPixel) == 2, "NO!");
#endif
}
