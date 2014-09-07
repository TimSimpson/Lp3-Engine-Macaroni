// //
// //  THIS NO WORKY!
// //        Delete it if you must.
// //
// #include <Lp3/LE.h>
// #ifndef LP3_COMPILE_TARGET_DREAMCAST
//     #define PLATFORM_NAME "Dreamcast"
//     #include <Lp3/Engine/NoOptMain.h>
// #else

// #include <Lp3/Log.h>
// #include <Lp3/Engine/DemoUtils.h>
// #include <kos.h>
// #include <Lp3/Engine/Gfx/InternalPixel.h>

// using namespace Lp3::Engine::Gfx;

// struct Point2D
// {
//     float X;
//     float Y;
// };

// struct Triangle
// {
//     Point2D P1;
//     Point2D P2;
//     Point2D P3;
// };

// struct Quad
// {
//     Point2D P1;
//     Point2D P2;
//     Point2D P3;
//     Point2D P4;
// };

// struct Box
// {
//     Point2D Location;
//     float Width;
//     float Height;
// };

// struct Color
// {
//     Lp3::UInt8 A;
//     Lp3::UInt8 R;
//     Lp3::UInt8 G;
//     Lp3::UInt8 B;

//     inline operator Lp3::UInt32() const
//     {
//         return PVR_PACK_COLOR(A / 255, R / 255, G / 255, B / 255);
//     }
// };

// class PvrGraphics
// {
// private:
//     pvr_poly_hdr_t hdr;
//     pvr_ptr_t textureData;
// public:
//     PvrGraphics()
//     {
//         pvr_init_params_t params = {
//         // Enable opaque and translucent polygons with size 16
//         { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16 },
//         //{ PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0 },
//         // Vertex buffer size 512K
//         512*1024
//         };
//         pvr_init(&params);

//         pvr_set_bg_color(0, 0, 0);

//         pvr_poly_cxt_t cxt;
//         pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
//         cxt.gen.shading = PVR_SHADE_FLAT;
//         pvr_poly_compile(&hdr, &cxt);

//         textureData = pvr_mem_malloc(256 * 256 * 2);
//     }

//     ~PvrGraphics()
//     {
//         pvr_mem_free(textureData);
//     }

//     void DrawTriangle(const Triangle & triangle, const Color & color)
//     {
//         pvr_poly_cxt_t cxt;
//         pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
//         pvr_poly_hdr_t hdr;
//         pvr_poly_compile(&hdr, &cxt);
//         pvr_prim(&hdr, sizeof(hdr));

//         // TODO: Add debug only checks to make sure we aren't submitting
//         //       counterclockwise shapes.

//         pvr_vertex_t vert;
//         vert.flags = PVR_CMD_VERTEX;
//         vert.x = triangle.P1.X;
//         vert.y = triangle.P1.Y;
//         vert.z = 5.0f;
//         vert.u = 0;
//         vert.v = 0;
//         vert.argb = color;
//         vert.oargb = 0;
//         pvr_prim(&vert, sizeof(vert));

//         vert.x = triangle.P2.X;
//         vert.y = triangle.P2.Y;
//         pvr_prim(&vert, sizeof(vert));

//         vert.x = triangle.P3.X;
//         vert.y = triangle.P3.Y;
//         vert.flags = PVR_CMD_VERTEX_EOL;
//         pvr_prim(&vert, sizeof(vert));
//     }

//     void DrawQuad(const Quad & quad, const Color & color)
//     {
//         pvr_sprite_cxt_t cxt;
//         pvr_sprite_cxt_col(&cxt, PVR_LIST_TR_POLY);

//         pvr_sprite_hdr_t hdr;
//         pvr_sprite_compile(&hdr, &cxt);

//         pvr_prim(&hdr, sizeof(hdr));

//         pvr_sprite_col_t sprite;
//         sprite.flags = PVR_CMD_VERTEX_EOL;
//         sprite.ax = quad.P1.X;
//         sprite.ay = quad.P1.Y;
//         sprite.bx = quad.P2.X;
//         sprite.by = quad.P2.Y;
//         sprite.cx = quad.P3.X;
//         sprite.cy = quad.P3.Y;
//         sprite.dx = quad.P4.X;
//         sprite.dy = quad.P4.Y;

//         pvr_prim(&sprite, sizeof(sprite));
//     }

//     void DrawBox(const Box & box, const Color & color)
//     {
//         pvr_poly_cxt_t cxt;
//         pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
//         pvr_poly_hdr_t hdr;
//         pvr_poly_compile(&hdr, &cxt);
//         pvr_prim(&hdr, sizeof(hdr));

//         // TODO: Add debug only checks to make sure we aren't submitting
//         //       counterclockwise shapes.

//         pvr_vertex_t vert;
//         vert.flags = PVR_CMD_VERTEX;
//         vert.x = box.Location.X;
//         vert.y = box.Location.Y + box.Height;
//         vert.z = 5.0f;
//         vert.u = 0;
//         vert.v = 0;
//         vert.argb = color;
//         vert.oargb = 0;
//         pvr_prim(&vert, sizeof(vert));

//         pvr_vertex_t vert2 = vert;
//         vert2.x = box.Location.X;
//         vert2.y = box.Location.Y;
//         pvr_prim(&vert2, sizeof(vert));

//         pvr_vertex_t vert3 = vert2;
//         vert3.x = box.Location.X + box.Width;
//         vert3.y = box.Location.Y + box.Height;
//         pvr_prim(&vert3, sizeof(vert));

//         pvr_vertex_t vert4 = vert;
//         vert4.x = box.Location.X + box.Width;
//         vert4.y = box.Location.Y;
//         vert4.flags = PVR_CMD_VERTEX_EOL;
//         pvr_prim(&vert4, sizeof(vert));
//     }


//     void render()
//     {
//         pvr_wait_ready();

//         pvr_scene_begin();

//         pvr_list_begin(PVR_LIST_OP_POLY);

//         pvr_prim(&hdr, sizeof(hdr));

//         renderOpaque();// Do opaque drawing here!

//         pvr_list_finish();

//         // pvr_list_begin(PVR_LIST_TR_POLY);

//         // renderTranslucent();

//         // pvr_list_finish();
//         pvr_scene_finish();
//     }

//     void renderOpaque()
//     {
//         const Color blue = { 255, 0, 0, 255 };
//         const Color red = { 255, 255, 0, 0 };
//         const Color purple = { 255, 255, 0, 255 };
//         const Color yellow = { 255, 255, 255, 0 };
//         // DrawTriangle({{320.0, 0.0}, {640.0, 480.0f}, {0.0, 480.0}},
//         //              purple);

//         // DrawTriangle({{0.0, 0.0}, {64.0, 0.0f}, {0.0, 64.0}},
//         //              yellow);
//         // DrawTriangle({{576.0, 0.0}, {640.0, 0.0f}, {640.0, 64.0}},
//         //              yellow);
//         // DrawTriangle({{0.0, 414.0}, {64.0, 480.0f}, {0.0, 480.0}},
//         //              yellow);
//         // DrawTriangle({{640.0, 414.0}, {640.0, 480.0f}, {576.0, 480.0}},
//         //              yellow);

//         //DrawBox({{160, 120}, 320, 240}, blue);

//          // DrawQuad({{160.0, 120.0}, {480.0, 120.0}, {480.0, 360.0}, {160.0, 360.0}},
//          //         red);

//          // DrawQuad({{0.0, 0.0}, {480.0, 120.0}, {480.0, 360.0}, {160.0, 360.0}},
//          //         red);
//         pvr_sprite_cxt_t op_poly_cxt;
//         pvr_sprite_cxt_col(&op_poly_cxt, PVR_LIST_OP_POLY);

//         pvr_sprite_hdr_t op_poly_hdr;
//         pvr_sprite_compile(&op_poly_hdr, &op_poly_cxt);

//         pvr_sprite_cxt_t pt_sprite_cxt;
//         vid_set_mode(DM_640x480, PM_RGB565);
//         pvr_sprite_cxt_txr(&pt_sprite_cxt, PVR_LIST_PT_POLY,
//             PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_NONTWIDDLED,
//             1024, 1024, textureData, PVR_FILTER_NONE);
//         pt_sprite_cxt.gen.culling = PVR_CULLING_NONE;

//         pvr_sprite_hdr_t pt_sprite_hdr;
//         pvr_sprite_compile(&pt_sprite_hdr, &pt_sprite_cxt);

//          pvr_list_prim(PVR_LIST_PT_POLY, &pt_sprite_hdr,
//                           sizeof(pvr_sprite_hdr_t));

//          pvr_sprite_txr_t sprite;


//     sprite.flags = PVR_CMD_VERTEX_EOL;
//     sprite.ax = 100;
//     sprite.ay = 100;
//     sprite.az = 10;

//     sprite.bx = 100 +200;
//     sprite.by = 100;
//     sprite.bz = 10;

//     sprite.cx = 100 +200;
//     sprite.cy = 100 + 100;
//     sprite.cz = 10;

//     sprite.dx = 100;
//     sprite.dy = 100 + 100;

//     sprite.auv = PVR_PACK_16BIT_UV(0.0f, 256.0f);
//     sprite.buv = PVR_PACK_16BIT_UV(0.0f, 256.0f);
//     sprite.cuv = PVR_PACK_16BIT_UV(256.0f, 256.0f);
//     pvr_list_prim( PVR_LIST_PT_POLY, &sprite, sizeof(sprite));
//         // pvr_list_t list;
//         // pvr_sprite_col_t spr;
//         // pvr_sprite_hdr_t *hdr;
//         // pvr_list_prim(list, &op_poly_hdr, sizeof(pvr_sprite_hdr_t));
//     }

//     void renderTranslucent()
//     {
//     }


// };

// int main(int argc, char **argv)
// {
//     LP3_LOG_DEBUG("PVR demo fun.");
//     {
//         PvrGraphics gfx;
//         LP3_LOG_DEBUG("Press START on the controller to exit.");
//         while(!Lp3::Engine::DemoUtils::DreamcastStartIsPressed()) {
//             gfx.render();
//         }
//         LP3_LOG_DEBUG("Going to shut down PVR through miracle of RAII.");
//     }
// }

// #endif // LP3_COMPILE_TARGET_DREAMCAST
