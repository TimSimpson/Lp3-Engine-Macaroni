#include <Lp3/LE.h>
#ifndef LP3_COMPILE_TARGET_DREAMCAST
    #define PLATFORM_NAME "Dreamcast"
    #include <Lp3/Engine/NoOptMain.h>
#else

//#include <alogrithm>
#include <Lp3/Assert.h>
#include <boost/format.hpp>
#include <Lp3/Log.h>
#include <Lp3/Engine/DemoUtils.h>
#include <kos.h>
#include <Lp3/Engine/Gfx/InternalPixel.h>
//#include <boost/smart_ptr.hpp>

using namespace boost;
using namespace Lp3::Engine::Gfx;

struct Point2D
{
    float X;
    float Y;
};

struct Triangle
{
    Point2D P1;
    Point2D P2;
    Point2D P3;
};

struct Quad
{
    Point2D P1;
    Point2D P2;
    Point2D P3;
    Point2D P4;
};

struct Box
{
    Point2D Location;
    float Width;
    float Height;
};

struct Color
{
    Lp3::UInt8 A;
    Lp3::UInt8 R;
    Lp3::UInt8 G;
    Lp3::UInt8 B;

    inline operator Lp3::UInt32() const
    {
        return PVR_PACK_COLOR(A / 255, R / 255, G / 255, B / 255);
    }
};

#define TWIDTAB(x) ( (x&1)|((x&2)<<1)|((x&4)<<2)|((x&8)<<3)|((x&16)<<4)| \
            ((x&32)<<5)|((x&64)<<6)|((x&128)<<7)|((x&256)<<8)|((x&512)<<9) )
#define TWIDOUT(x, y) ( TWIDTAB((y)) | (TWIDTAB((x)) << 1) )

struct Texture
{
    pvr_ptr_t data;
    float height;
    float width;
    const size_t _min;   // Needed for calculations.
    const size_t _mask;

    Texture(float width, float height)
    :   data(nullptr),
        height(height),
        width(width),
        _min(std::min(width, height)),
        _mask(_min - 1)
    {
        data = pvr_mem_malloc(width * height * sizeof(InternalPixel));
    }

    ~Texture()
    {
        pvr_mem_free(data);
    }

    inline float Height()
    {
        return height;
    }

    inline size_t IndexOf(const size_t x, const size_t y) const
    {
        return TWIDOUT(x & _mask, y & _mask) + (x/_min + y / _min) * _min * _min;
    }

    inline InternalPixel * PixelArray()
    {
        return reinterpret_cast<InternalPixel *>(data);
    }

    void SetPixel(const size_t x, const size_t y, InternalPixel & newValue)
    {
        LP3_ASSERT_TRUE_MESSAGE(x < width, "X must be less than width.");
        LP3_ASSERT_TRUE_MESSAGE(y < height, "Y must be less than height.");
        //InternalPixel & pixel = PixelArray()[IndexOf(x, y)];
        // Lp3::UInt16 * array = reinterpret_cast<Lp3::UInt16 *>(data);
        // Lp3::UInt16 nv = newValue.argb;
        // //pixel = newValue;
        // array[IndexOf(x, y)] = newValue.argb;
        reinterpret_cast<Lp3::UInt16 *>(data)[IndexOf(x, y)] = newValue.argb;
    }

    inline void SetPixel(const size_t x, const size_t y,
                         const Lp3::UInt16 & newValue)
    {
        LP3_ASSERT_TRUE_MESSAGE(x < width, "X must be less than width.");
        LP3_ASSERT_TRUE_MESSAGE(y < height, "Y must be less than height.");
        Lp3::UInt16 * array = reinterpret_cast<Lp3::UInt16 *>(data);
        //InternalPixel & pixel = PixelArray()[IndexOf(x, y)];
        //pixel = newValue;
        array[IndexOf(x, y)] = newValue;
    }

    float Width()
    {
        return width;
    }
};

//typedef boost::shared_ptr<Texture> TexturePtr;


class PvrGraphics
{
private:
    pvr_poly_hdr_t hdr;
public:
    Texture texture;

public:
    PvrGraphics()
    :   texture(32, 32)
    {
        pvr_set_bg_color(1.0f, 0.25f, 0.25f);

        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
        cxt.gen.shading = PVR_SHADE_FLAT;
        pvr_poly_compile(&hdr, &cxt);

        InternalPixel pixel;
        pixel.SetF(1.0f, 1.0f, 1.0f, 1.0f);
        for (size_t x = 0; x < texture.Width(); x ++)
        {
            for (size_t y= 0 ; y < texture.Height(); y ++)
            {
                pixel.SetF(0.0f, 0.0f, 0.0f, 0.0f);
                texture.SetPixel(x, y, pixel);
            }
        }


        // for (size_t x = 0; x < 64; x ++)
        // {
        //     for (size_t y= 0 ; y < 64; y ++)
        //     {
        //         pixel.Set(0, 255, 0, 255);
        //         texture.SetPixel(x, y, pixel);
        //     }
        // }

        // for (size_t x = 0; x < 64; x ++)
        // {
        //     for (size_t y= 0 ; y < 64; y ++)
        //     {
        //         pixel.Set(0, 0, 255, 255);
        //         texture.PixelArray()[(y * 256) + x] = pixel;
        //     }
        // }
    }

    ~PvrGraphics()
    {
        //pvr_mem_free(textureData);
    }

    // void DrawQuad(const Quad & quad, const Color & color)
    // {
    //     pvr_poly_cxt_t cxt;
    //     pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
    //     pvr_poly_hdr_t hdr;
    //     pvr_poly_compile(&hdr, &cxt);
    //     pvr_prim(&hdr, sizeof(hdr));

    //     // TODO: Add debug only checks to make sure we aren't submitting
    //     //       counterclockwise shapes.

    //     pvr_vertex_t vert;
    //     vert.flags = PVR_CMD_VERTEX;
    //     vert.x = quad.P4.X;
    //     vert.y = quad.P4.Y;
    //     vert.z = 5.0f;
    //     vert.u = 0;
    //     vert.v = 0;
    //     vert.argb = color;
    //     vert.oargb = 0;
    //     pvr_prim(&vert, sizeof(vert));

    //     vert.x = quad.P1.X;
    //     vert.y = quad.P1.Y;
    //     pvr_prim(&vert, sizeof(vert));

    //     vert.x = quad.P3.X;
    //     vert.y = quad.P3.Y;
    //     pvr_prim(&vert, sizeof(vert));

    //     vert.x = quad.P2.X;
    //     vert.y = quad.P2.Y;
    //     vert.flags = PVR_CMD_VERTEX_EOL;
    //     pvr_prim(&vert, sizeof(vert));
    // }

    void DrawBox(const Box & box, const Color & color)
    {
        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);

        cxt.gen.shading = PVR_SHADE_FLAT;

        cxt.gen.alpha = PVR_ALPHA_ENABLE;
         cxt.txr.env = PVR_TXRENV_MODULATE;
   cxt.txr.enable = PVR_TEXTURE_ENABLE;
   cxt.txr.mipmap = PVR_MIPMAP_DISABLE;
   cxt.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;
   cxt.txr.filter = PVR_FILTER_NONE; //Try bilinear
  // cxt.txr.filter = PVR_FILTER_BILINEAR; // <-NEW!!!
   cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
   // cxt.txr.uv_flip = PVR_UVFLIP_NONE;
   // cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;
   //cxt.txr.format = PVR_TXRFMT_NONE;
   cxt.txr.base = texture.data;
   cxt.txr.width = texture.Width();
   cxt.txr.height = texture.Height();
        cxt.txr.format = PVR_TXRFMT_ARGB4444;

        pvr_poly_hdr_t hdr;
        pvr_poly_compile(&hdr, &cxt);
        pvr_prim(&hdr, sizeof(hdr));

        // TODO: Add debug only checks to make sure we aren't submitting
        //       counterclockwise shapes.

        pvr_vertex_t vert;
        vert.flags = PVR_CMD_VERTEX;
        vert.x = box.Location.X;
        vert.y = box.Location.Y + box.Height;
        vert.z = 5.0f;
        vert.u = 0.0f;
        vert.v = 0.95;
        vert.argb = color;
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        vert.x = box.Location.X;
        vert.y = box.Location.Y;
        vert.u = 0.0f;
        vert.v = 0.0f;
        pvr_prim(&vert, sizeof(vert));

        vert.x = box.Location.X + box.Width;
        vert.y = box.Location.Y + box.Height;
        vert.u = 0.95;
        vert.v = 0.95;
        pvr_prim(&vert, sizeof(vert));

        vert.x = box.Location.X + box.Width;
        vert.y = box.Location.Y;
        vert.u = 0.95;
        vert.v = 0.0f;
        vert.flags = PVR_CMD_VERTEX_EOL;
        pvr_prim(&vert, sizeof(vert));
    }


    void render()
    {
        pvr_wait_ready();

        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);

        pvr_prim(&hdr, sizeof(hdr));

        renderOpaque();// Do opaque drawing here!

        //pvr_list_finish();

        pvr_list_begin(PVR_LIST_TR_POLY);

        renderTranslucent();

        pvr_list_finish();
        pvr_scene_finish();
    }

    void renderOpaque()
    {
        const Color blue = { 255, 0, 0, 255 };
        const Color red = { 255, 255, 0, 0 };
        const Color purple = { 255, 255, 0, 255 };
        const Color yellow = { 255, 255, 255, 0 };
        // DrawTriangle({{320.0, 0.0}, {640.0, 480.0f}, {0.0, 480.0}},
        //              purple);

        // DrawTriangle({{0.0, 0.0}, {64.0, 0.0f}, {0.0, 64.0}},
        //              yellow);
        // DrawTriangle({{576.0, 0.0}, {640.0, 0.0f}, {640.0, 64.0}},
        //              yellow);
        // DrawTriangle({{0.0, 414.0}, {64.0, 480.0f}, {0.0, 480.0}},
        //              yellow);
        // DrawTriangle({{640.0, 414.0}, {640.0, 480.0f}, {576.0, 480.0}},
        //              yellow);

        // DrawBox({{160, 120}, 320, 240},
        //         { 255, 255, 255, 255 });

        // DrawBox({{64, 64}, 32, 32},
        //         { 255, 255, 255, 255 });

        // static float W = 0;
        // DrawBox({{320, 64}, W, W},
        //         { 255, 255, 255, 255 });
        // W += 1.0;
        // if (W > 320) { W = 1.0f; }

        DrawBox({{64, 64}, 256, 256},
                { 255, 255, 255, 255 });

        // DrawBox({{64, 240}, 64, 64},
        //         { 255, 255, 255, 255 });

        // DrawQuad({{0.0, 0.0}, {640.0, 0.0}, {640.0, 480.0}, {0.0, 480.0}},
        //          red);
    }

    void renderTranslucent()
    {
        // const Color red = { 255, 255, 0, 0 };
        // DrawBox({{320, 240}, 256, 256},
        //         { 255, 255, 255, 255 });
    }
};

void pixelTests()
{
InternalPixel pixel;
    //pixel.Set(1.0f, 1.0f, 1.0f, 1.0f);

    // THIS WORKS
    //pixel.ab = 0xFF;
    //pixel.cd = 0xFF;
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


    // const Lp3::UInt16 b = (( (0.0f * 255) << 12) | ( (1.0f * 255) << 8));
    // LP3_LOG_DEBUG_F(format("This is what we have: %x") % b);
    // LP3_ASSERT_EQUAL(0x0F00, b);

// #define TO_BIT(n, s) (  ( ((Lp3::UInt16) n) * 0xF )  << s)
// #define TO_SIT(n, s)  ( ((Lp3::UInt16) n) * 0xF )
//     const Lp3::UInt16 c = TO_BIT(0.0f, 12) | TO_BIT(1.0f, 8) | TO_BIT(0.0f, 4)
//                             | TO_BIT(1.0f, 0);
//     LP3_LOG_DEBUG_F(format("This is what we have: %x") % c);
//     LP3_ASSERT_EQUAL(0x0F00, c);


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
}

int main(int argc, char **argv)
{
    pvr_init_params_t params = {
        // Enable opaque and translucent polygons with size 16
        { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16 },
        //{ PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0 },
        // Vertex buffer size 512K
        512*1024
        };
    pvr_init(&params);

    size_t maxx = 0;


    // Turning off alpha causes problems?
    InternalPixel pixel;
    pixel.SetF(1.0f, 1.0f, 1.0f, 1.0f);
    LP3_LOG_DEBUG("PVR demo fun.");
    {
        PvrGraphics gfx;
        for (size_t x = 0; x < gfx.texture.Width(); x ++ ) {
            for (size_t y = 0; y < gfx.texture.Width(); y ++ ) {
                //Lp3::Int16 * blah = (Lp3::Int16 *) gfx.texture.data;
                //blah[(y * (size_t) gfx.texture.Width() + x)] = 0xF000; //0xFFFF;



                // Lp3::Int16 * blah = (Lp3::Int16 *) gfx.texture.data;
                // auto index = gfx.texture.IndexOf(x, y);
                // //blah[index] = 0xFFFF;// pixel.ab << 8 | pixel.cd; //0xFFF


                // //gfx.texture.SetPixel(x, y, (Lp3::UInt16) 0xF0F0);//pixel);
                gfx.texture.SetPixel(x, y, pixel);
            }
        }
        //pixel.ab = 0xF0;
        //pixel.cd = 0x0F;

        pixel.argb = 0xF00F;
        //pixel = reinterpret_cast<InternalPixel>((Lp3::UInt16) 0xF00F);

        //pixel.SetF(1.0f, 1.0f, 1.0f, 1.0f);

        Lp3::Int16 * blah = (Lp3::Int16 *) gfx.texture.data;
        auto index = gfx.texture.IndexOf(2, 2);
        //blah[(y * (size_t) gfx.texture.Width() + x)] = 0xF000;
        gfx.texture.SetPixel(2, 2, pixel);
        //blah[index] = pixel.argb; //pixel.ab << 8 | pixel.cd; //0xFFFF;

        // index = gfx.texture.IndexOf(4, 2);
        // blah[index] = pixel.argb; //pixel.ab << 8 | pixel.cd; //0xFFFF;
        gfx.texture.SetPixel(4, 2, pixel);

        // index = gfx.texture.IndexOf(0, 0);
        // blah[index] = pixel.argb; //pixel.ab << 8 | pixel.cd; //0xFFFF;
        gfx.texture.SetPixel(0, 0, pixel);

        for (size_t x = 8; x < 12; x ++) {
            for (size_t y = 8; y < 12; y ++) {
                // index = gfx.texture.IndexOf(x, y);
                // //blah[index] = pixel.ab << 8 | pixel.cd; //0xFFFF;
                // gfx.texture.SetPixel(x, y, pixel);
                gfx.texture.SetPixel(x, y, pixel);
            }
        }

        pixel.SetF(0.0f, 1.0f, 0.0f, 1.0f);
        for (size_t x=0; x < 32; x++) {
            gfx.texture.SetPixel(x, x, pixel);
        }

        //gfx.texture.SetPixel(x, y, pixel);
        // pixel.Set(255, 255, 255, 255);
        // gfx.texture.SetPixel(2, 2, pixel);
        LP3_LOG_DEBUG("Press START on the controller to exit.");
        while(!Lp3::Engine::DemoUtils::DreamcastStartIsPressed()) {
            gfx.render();

            // gfx.texture.SetPixel(30, 30, pixel);
            // gfx.texture.SetPixel(31, 30, pixel);
            // gfx.texture.SetPixel(30, 31, pixel);
            // gfx.texture.SetPixel(31, 31, pixel);
            // for (size_t x = 0; x < gfx.texture.Width(); x ++)
            // {
            //     for (size_t y= 0 ; y < maxx; y ++)
            //     {
            //         //gfx.texture.PixelArray()[(y * 256) + x] = pixel;
            //         gfx.texture.SetPixel(x, y, pixel);
            //     }
            // }
            // maxx ++;
            // if (maxx > 64) {
            //     maxx= 0;
            //     LP3_LOG_DEBUG("It's RED time...")
            //     pixel.Set(255, 0, 0, 255);
            // }
        }
        LP3_LOG_DEBUG("Going to shut down PVR through miracle of RAII.");
    }
}

#endif // LP3_COMPILE_TARGET_DREAMCAST
