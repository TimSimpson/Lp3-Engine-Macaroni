#include <Lp3/LE.h>
#ifndef LP3_COMPILE_TARGET_DREAMCAST
    #define PLATFORM_NAME "Dreamcast"
    #include <Lp3/Engine/NoOptMain.h>
#else

#include <Lp3/Log.h>
#include <Lp3/Engine/DemoUtils.h>
#include <kos.h>
#include <Lp3/Engine/Gfx/InternalPixel.h>

using namespace Lp3::Engine::Gfx;

void quad(float x, float y, float z, float w, float h,
          float u, float v)
{
    const Lp3::UInt32 oargb = 0;
    const Lp3::UInt32 argb = 255 | (255 << 8) | (255 << 16) | 0xff000000;
    pvr_vertex_t vert;
    vert.flags = PVR_CMD_VERTEX;

    float y2 = y + h;
    float x2 = x + w;

    vert.x = x; vert.y = y; vert.z = 0.5f;
    vert.u = 0; vert.v = 1.0; vert.argb = argb; vert.oargb = oargb;
    pvr_prim(&vert, sizeof(pvr_vertex_t));

    vert.x = x2; vert.y = y; vert.z = 0.5f;
    vert.u = 0; vert.v = 0.0; vert.argb = argb; vert.oargb = oargb;
    pvr_prim(&vert, sizeof(pvr_vertex_t));

    vert.x = x; vert.y = y2; vert.z = 0.5f;
    vert.u = 1.0; vert.v = 1.0; vert.argb = argb; vert.oargb = oargb;
    pvr_prim(&vert, sizeof(pvr_vertex_t));

    vert.x = x2; vert.y = y2; vert.z = 0.0f;
    vert.u = 1.0; vert.v = 0.0; vert.argb = argb; vert.oargb = oargb;
    pvr_prim(&vert, sizeof(pvr_vertex_t));

}


class PvrGraphics
{
private:
    const float height = 256.0;
    const float width = 256.0;
    pvr_ptr_t textureData;

    pvr_poly_hdr_t hdr; // pvr mark strips direct

public:
    PvrGraphics()
    :   height(256.0),
        width(256.0),
        textureData(nullptr)
    {
        pvr_init_params_t params = {
        // Enable opaque and translucent polygons with size 16
        { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16 },
        //{ PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0 },
        // Vertex buffer size 512K
        512*1024
        };
        pvr_init(&params);

        // pvr_init_defaults();

        // pvr_set_bg_color(0.5f,0.5f,0.1f);
        // vid_border_color(255, 0, 0);

        //textureData = pvr_mem_malloc(width * height * sizeof(InternalPixel));
        //drawOnTexture();


        // PVR MARK STRIPS DIRECT
        // pvr_init_params_t pvr_params = {
        //     { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0 },
        //     512 * 1024
        // };


        // pvr_init(&pvr_params);
        pvr_set_bg_color(0, 0, 0);

        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
        cxt.gen.shading = PVR_SHADE_FLAT;
        pvr_poly_compile(&hdr, &cxt);
    }

    ~PvrGraphics()
    {
        //pvr_mem_free(textureData);
    }

    void do_frame() {
        int polycnt = 1000;

        pvr_vertex_t vert;
        int x, y, z;
        int size;
        int i, col;

        vid_border_color(0, 0, 0);
        pvr_wait_ready();
        vid_border_color(255, 0, 0);
        pvr_scene_begin();
        pvr_list_begin(PVR_LIST_OP_POLY);
        pvr_prim(&hdr, sizeof(hdr));

        x = rand() % 640;
        y = rand() % 480;
        z = rand() % 100 + 1;
        col = rand() % 256;

        vert.flags = PVR_CMD_VERTEX;
        vert.x = x;
        vert.y = y;
        vert.z = z;
        vert.u = vert.v = 0.0f;
        vert.argb = col | (col << 8) | (col << 16) | 0xff000000;
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        for(i = 0; i < polycnt; i++) {
            x = (x + ((rand() % 50) - 25)) % 640;
            y = (y + ((rand() % 50) - 25)) % 480;
            col = rand() % 256;
            vert.x = x;
            vert.y = y;
            vert.argb = col | (col << 8) | (col << 16) | 0xff000000;

            if(i == (polycnt - 1))
                vert.flags = PVR_CMD_VERTEX_EOL;

            pvr_prim(&vert, sizeof(vert));
        }

        pvr_list_finish();
        pvr_scene_finish();
        vid_border_color(0, 255, 0);
    }

    void drawOnTexture()
    {
        Lp3::UInt16 blah = 0;
        for (size_t i = 0; i < width * height * sizeof(InternalPixel); i ++) {
            ((Lp3::UInt16 *) textureData)[i] = ++blah;
            if (blah > 30000)
            {
                blah = 0;
            }
        }
    }

    void drawTriangle() {
        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
        pvr_poly_hdr_t hdr;
        pvr_poly_compile(&hdr, &cxt);
        pvr_prim(&hdr, sizeof(hdr));

        int x1 = 320;
        int y1 = 0;

        int x3 = 0;
        int y3 = 480;

        int x2 = 640;
        int y2 = 480;

        pvr_vertex_t vert;
        vert.flags = PVR_CMD_VERTEX;
        vert.x = x1;
        vert.y = y1;
        vert.z = 5.0f;
        vert.u = 0;
        vert.v = 0;
        Lp3::UInt8 a = 255;
        Lp3::UInt8 r = 255;
        Lp3::UInt8 g = 0;
        Lp3::UInt8 b = 255;
        vert.argb = PVR_PACK_COLOR(a / 255, r / 255, g / 255, b / 255);
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        vert.x = x2;
        vert.y = y2;
        pvr_prim(&vert, sizeof(vert));

        vert.x = x3;
        vert.y = y3;
        vert.flags = PVR_CMD_VERTEX_EOL;
        pvr_prim(&vert, sizeof(vert));

    }

    void hline_tr(float x1, float x2, float y, uint8 a, uint8 r, uint8 g, uint8 b) {
           pvr_poly_hdr_t hdr;
           pvr_poly_cxt_t cxt;
           pvr_vertex_t vert;
           float cx;
           if(x1 > x2) {
                   cx = x1;
                   x1 = x2;
                   x2 = cx;
           }

           pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
           pvr_poly_compile(&hdr, &cxt);

           pvr_prim(&hdr, sizeof(hdr));
           vert.flags = PVR_CMD_VERTEX;
           vert.x = x1;
           vert.y = y + 1;
           vert.z = 5.0f;
           vert.u = 0;
           vert.v = 0;
           vert.argb = PVR_PACK_COLOR(a / 255, r / 255, g / 255, b / 255);
           vert.oargb = 0;
           pvr_prim(&vert, sizeof(vert));

           vert.y = y;
           pvr_prim(&vert, sizeof(vert));

           vert.x = x2;
           vert.y = y + 1;
           pvr_prim(&vert, sizeof(vert));

           vert.flags = PVR_CMD_VERTEX_EOL;
           vert.y = y;
           pvr_prim(&vert, sizeof(vert));
    }

    void line_tr(float x1, float y1, float x2, float y2, uint8 a, uint8 r, uint8 g, uint8 b)   {
        pvr_poly_hdr_t hdr;
        pvr_poly_cxt_t cxt;
        pvr_vertex_t vert;

        pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
        pvr_poly_compile(&hdr, &cxt);
        pvr_prim(&hdr, sizeof(hdr));

        vert.flags = PVR_CMD_VERTEX;
        vert.x = x1;
        vert.y = y1;
        vert.z = 5.0f;
        vert.u = 0;
        vert.v = 0;
        vert.argb = PVR_PACK_COLOR(a / 255, r / 255, g / 255, b / 255);
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        vert.x = x2;
        vert.y = y2;
        pvr_prim(&vert, sizeof(vert));

        vert.x = x1 + 1;
        vert.y = y1;
        pvr_prim(&vert, sizeof(vert));

        vert.flags = PVR_CMD_VERTEX_EOL;
        vert.x = x2 + 1;
        vert.y = y2;
        pvr_prim(&vert, sizeof(vert));
    }

    void render()
    {
        pvr_wait_ready();

        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);

        pvr_prim(&hdr, sizeof(hdr));

        renderOpaque();// Do opaque drawing here!

        pvr_list_finish();

        // pvr_list_begin(PVR_LIST_TR_POLY);

        // renderTranslucent();

        // pvr_list_finish();
        pvr_scene_finish();
    }

    void renderOpaque()
    {
        for (float y = 10; y < 480; y += 10)
        {
            hline_tr(0, 640, y, 0, 255, 0, 0);
        }
        line_tr(32, 48, 48, 32, 255, 255, 255, 255);
        line_tr(48, 288, 32, 272, 255, 255, 255, 255);

        drawTriangle();
    }

    void renderTranslucent()
    {
        for (float y = 10; y < 480; y += 10)
        {
            hline_tr(0, 640, y, 0, 255, 0, 0);
        }
        line_tr(32, 48, 48, 32, 255, 255, 255, 255);
        line_tr(48, 288, 32, 272, 255, 255, 255, 255);
    }

    void vline_tr(float x, float y1, float y2, uint8 a, uint8 r, uint8 g, uint8 b) {
       pvr_poly_hdr_t hdr;
       pvr_poly_cxt_t cxt;
       pvr_vertex_t vert;
       float cy;
       if(y1 > y2) {
               cy = y1;
               y1 = y2;
               y2 = cy;
       }

       pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
       pvr_poly_compile(&hdr, &cxt);

       pvr_prim(&hdr, sizeof(hdr));
       vert.flags = PVR_CMD_VERTEX;
       vert.x = x /* - 1 */;
       vert.y = y2;
       vert.z = 5.0f;
       vert.u = 0;
       vert.v = 0;
       vert.argb = PVR_PACK_COLOR(a / 255, r / 255, g / 255, b / 255);
       vert.oargb = 0;
       pvr_prim(&vert, sizeof(vert));

       vert.y = y1;
       pvr_prim(&vert, sizeof(vert));

       vert.x = x + 1;
       vert.y = y2;
       pvr_prim(&vert, sizeof(vert));

       vert.flags = PVR_CMD_VERTEX_EOL;
       vert.y = y1;
       pvr_prim(&vert, sizeof(vert));
    }

};

int main(int argc, char **argv)
{
    LP3_LOG_DEBUG("PVR demo fun.");
    {
        PvrGraphics gfx;
        LP3_LOG_DEBUG("Press START on the controller to continue!");
        while(!Lp3::Engine::DemoUtils::DreamcastStartIsPressed()) {
            gfx.render();
        }
        LP3_LOG_DEBUG("Going to shut down PVR through miracle of RAII.");
    }



    //LP3_LOG_DEBUG("Init successful, trying to malloc texture memory.");

    /** Set up the texures. */
    //Lp3::Int32 fmt1 = PVR_TXRFMT_RGB565 | PVR_TXRFMT_TWIDDLED;

    // pvr_poly_cxt_txr_mod(
    //     &cxt, PVR_LIST_OP_POLY, fmt1, width, height,
    //     tex_data, PVR_FILTER_BILINEAR,
    //     fmt2, w2, h2, txr2, PVR_FILTER_NONE);
    // pvr_poly_mod_compile(&phdr, &cxt);


//     const Lp3::UInt32 argb = 255 | (255 << 8) | (255 << 16) | 0xff000000;
//     const Lp3::UInt32 oargb = 0;

// // Set default polygon context information up here.
//     pvr_poly_cxt_t cxt;
//     // pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);

//     // cxt.gen.shading = PVR_SHADE_FLAT;
//     // cxt.gen.alpha = PVR_ALPHA_ENABLE;
//     // cxt.txr.env = PVR_TXRENV_MODULATE;
//     // cxt.txr.enable = PVR_TEXTURE_ENABLE;
//     // cxt.txr.mipmap = PVR_MIPMAP_DISABLE;
//     // cxt.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;
//     // cxt.txr.filter = PVR_FILTER_NONE; //Try bilinear
//     // // cxt.txr.filter = PVR_FILTER_BILINEAR; // <-NEW!!!
//     // cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
//     // cxt.txr.uv_flip = PVR_UVFLIP_NONE;
//     // cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;
//     // cxt.txr.format = PVR_TXRFMT_NONE;

//     pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY,
//                      PVR_TXRFMT_ARGB4444, //PVR_TXRFMT_RGB565 | PVR_TXRFMT_TWIDDLED,
//                      width, height, tex_data, PVR_FILTER_NONE);

//     // Skip all that and do it the dumb way without textures:
//     pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
//     cxt.gen.shading = PVR_SHADE_FLAT;

//     // CULL NOTHING!
//     cxt.gen.culling = PVR_CULLING_NONE;

//     LP3_LOG_DEBUG("COMPILEZ");
//     pvr_poly_hdr_t hdr;
//     pvr_poly_compile(&hdr, &cxt);
// // End of polygon context.

//     LP3_LOG_DEBUG("Going to draw.");

//     for (auto i = 0; i < 10 * 5; i ++)
//     {
//         vid_border_color(255, 0, 0);
//         pvr_set_bg_color(0.1, 0.1, 0.1);  // Set to a gray.


//         pvr_wait_ready();
//         pvr_scene_begin();





//         // cxt.txr.base = tex_data;
//         // cxt.txr.width = width;
//         // cxt.txr.height = height;
//         // cxt.txr.format = PVR_TXRFMT_ARGB4444;




//         pvr_list_t list = PVR_LIST_OP_POLY;
//         pvr_list_begin(list);

//         pvr_prim(&hdr, sizeof(hdr));

//         //pvr_poly_compile(&hdr, &cxt);
//         //pvr_prim(&hdr, sizeof(hdr));

//         // LP3_LOG_DEBUG("dotz...");
//         // {
//         //     const Lp3::UInt32 oargb = 0;
//         //     const Lp3::UInt32 argb = 255 | (255 << 8) | (255 << 16) | 0xff000000;
//         //     pvr_vertex_t vert[4] = {
//         //         { PVR_CMD_VERTEX, 10.0f, 150.0f, -1.0f, 0,
//         //             height, argb, oargb },
//         //         { PVR_CMD_VERTEX, 10.0f,  50.0f, -1.0f,
//         //             0.0f, 0.0f, argb, oargb },
//         //         { PVR_CMD_VERTEX, 80.0f, 150.0f, -1.0f,
//         //             width, height, argb, oargb },
//         //         { PVR_CMD_VERTEX, 80.0f,  50.0f, -1.0f,
//         //             width, 0.0f, argb, oargb },
//         //     };
//         //     pvr_prim(vert, sizeof(vert) * 4);
//         // }

//        //LP3_LOG_DEBUG("frame");
//        quad(10.0f, 10.0f, 0.0f, 50.0f, 50.0f, 0, 0);

//        for (size_t i = 0; i < 100; i ++)
//        {
//             quad(rand() % 559, rand() % 399, rand() % 100 + 1,
//                 rand() % 80, rand() % 80, 0, 0);
//        }

//         pvr_list_finish();
//         pvr_scene_finish();
//         //usleep(1000);
//     }
//     LP3_LOG_DEBUG("Deleting texture memory.");
//     // pvr_poly_hdr_t hdr;

    // pvr_poly_cxt_t cxt;

    // pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
    // cxt.gen.shading = PVR_SHADE_FLAT;
    // cxt.gen.alpha = PVR_ALPHA_ENABLE;
    // cxt.txr.env = PVR_TXRENV_MODULATE;
    // cxt.txr.enable = PVR_TEXTURE_ENABLE;
    // cxt.txr.mipmap = PVR_MIPMAP_DISABLE;
    // cxt.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;
    // cxt.txr.filter = PVR_FILTER_NONE; //Try bilinear
    // cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
    // cxt.txr.uv_flip = PVR_UVFLIP_NONE;
    // cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;
    // cxt.txr.format = PVR_TXRFMT_NONE;
    // cxt.txr.base = NULL;
    // cxt.txr.width = -1;
    // cxt.txr.height = -1;

    // pvr_poly_cxt_t cxtTR;
    // pvr_poly_cxt_col(&cxtTR, PVR_LIST_TR_POLY);

    // cxtTR.gen.shading = PVR_SHADE_FLAT;
    // cxtTR.txr.env = PVR_TXRENV_MODULATE;
    // cxtTR.txr.enable = PVR_TEXTURE_ENABLE;
    // cxtTR.txr.mipmap = PVR_MIPMAP_DISABLE;
    // cxtTR.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;
    // cxtTR.txr.filter = PVR_FILTER_NONE; //Try bilinear
    // // cxt.txr.filter = PVR_FILTER_BILINEAR; // <-NEW!!!
    // cxtTR.txr.alpha = PVR_TXRALPHA_ENABLE;
    // cxtTR.txr.uv_flip = PVR_UVFLIP_NONE;
    // cxtTR.txr.uv_clamp = PVR_UVCLAMP_NONE;
    // cxtTR.txr.format = PVR_TXRFMT_NONE;
    // cxtTR.txr.base = NULL;
    // cxtTR.txr.width = -1;
    // cxtTR.txr.height = -1;

    // pvr_vertex_t vert;

    //   // if (triangleIndex == 0)
    //   //     vert.flags = PVR_CMD_VERTEX;
    //   // else if (triangleIndex == 2)
    //   //     vert.flags = PVR_CMD_VERTEX_EOL;

    // vert.flags = PVR_CMD_VERTEX;
    // vert.x = fx;//spriteQuad.x1;
    // vert.y = fy;//spriteQuad.y2;
    // vert.z = fz;//spriteQuad.z;//50;//z;
    // vert.u = tu;//0.0f;//SpriteQuads[j].srcX1;
    // vert.v = tv;//1.0f;//SpriteQuads[j].srcY2;
    // vert.argb = 255 | (255 << 8) | (255 << 16) | 0xff000000;
    // vert.oargb = 0;


    // pvr_prim(&vert, sizeof(vert));

    // pvr_scene_begin();
    // pvr_list_begin(PVR_LIST_TR_POLY);//PT_POLY);
    // pvr_poly_compile(&hdr, &cxt);
    // pvr_prim(&hdr, sizeof(hdr));


    // pvr_mem_free(tex_data);
    // LP3_LOG_DEBUG("Finished, goodbye!");

}

#endif // LP3_COMPILE_TARGET_DREAMCAST
