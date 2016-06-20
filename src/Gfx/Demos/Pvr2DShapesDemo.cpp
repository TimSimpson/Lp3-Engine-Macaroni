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

class PvrGraphics
{
private:
    pvr_poly_hdr_t hdr;

public:
    PvrGraphics()
    {
        pvr_init_params_t params = {
        // Enable opaque and translucent polygons with size 16
        { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16 },
        //{ PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0, PVR_BINSIZE_0 },
        // Vertex buffer size 512K
        512*1024
        };
        pvr_init(&params);

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

    void DrawTriangle(const Triangle & triangle, const Color & color)
    {
        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
        pvr_poly_hdr_t hdr;
        pvr_poly_compile(&hdr, &cxt);
        pvr_prim(&hdr, sizeof(hdr));

        // TODO: Add debug only checks to make sure we aren't submitting
        //       counterclockwise shapes.

        pvr_vertex_t vert;
        vert.flags = PVR_CMD_VERTEX;
        vert.x = triangle.P1.X;
        vert.y = triangle.P1.Y;
        vert.z = 5.0f;
        vert.u = 0;
        vert.v = 0;
        vert.argb = color;
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        vert.x = triangle.P2.X;
        vert.y = triangle.P2.Y;
        pvr_prim(&vert, sizeof(vert));

        vert.x = triangle.P3.X;
        vert.y = triangle.P3.Y;
        vert.flags = PVR_CMD_VERTEX_EOL;
        pvr_prim(&vert, sizeof(vert));
    }

    void DrawQuad(const Quad & quad, const Color & color)
    {
        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
        pvr_poly_hdr_t hdr;
        pvr_poly_compile(&hdr, &cxt);
        pvr_prim(&hdr, sizeof(hdr));

        // TODO: Add debug only checks to make sure we aren't submitting
        //       counterclockwise shapes.

        pvr_vertex_t vert;
        vert.flags = PVR_CMD_VERTEX;
        vert.x = quad.P4.X;
        vert.y = quad.P4.Y;
        vert.z = 5.0f;
        vert.u = 0;
        vert.v = 0;
        vert.argb = color;
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        vert.x = quad.P1.X;
        vert.y = quad.P1.Y;
        pvr_prim(&vert, sizeof(vert));

        vert.x = quad.P3.X;
        vert.y = quad.P3.Y;
        pvr_prim(&vert, sizeof(vert));

        vert.x = quad.P2.X;
        vert.y = quad.P2.Y;
        vert.flags = PVR_CMD_VERTEX_EOL;
        pvr_prim(&vert, sizeof(vert));
    }

    void DrawBox(const Box & box, const Color & color)
    {
        pvr_poly_cxt_t cxt;
        pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
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
        vert.u = 0;
        vert.v = 0;
        vert.argb = color;
        vert.oargb = 0;
        pvr_prim(&vert, sizeof(vert));

        vert.x = box.Location.X;
        vert.y = box.Location.Y;
        pvr_prim(&vert, sizeof(vert));

        vert.x = box.Location.X + box.Width;
        vert.y = box.Location.Y + box.Height;
        pvr_prim(&vert, sizeof(vert));

        vert.x = box.Location.X + box.Width;
        vert.y = box.Location.Y;
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

        pvr_list_finish();

        // pvr_list_begin(PVR_LIST_TR_POLY);

        // renderTranslucent();

        // pvr_list_finish();
        pvr_scene_finish();
    }

    void renderOpaque()
    {
        const Color blue = { 255, 0, 0, 255 };
        const Color red = { 255, 255, 0, 0 };
        const Color purple = { 255, 255, 0, 255 };
        const Color yellow = { 255, 255, 255, 0 };
        DrawTriangle({{320.0, 0.0}, {640.0, 480.0f}, {0.0, 480.0}},
                     purple);

        DrawTriangle({{0.0, 0.0}, {64.0, 0.0f}, {0.0, 64.0}},
                     yellow);
        DrawTriangle({{576.0, 0.0}, {640.0, 0.0f}, {640.0, 64.0}},
                     yellow);
        DrawTriangle({{0.0, 414.0}, {64.0, 480.0f}, {0.0, 480.0}},
                     yellow);
        DrawTriangle({{640.0, 414.0}, {640.0, 480.0f}, {576.0, 480.0}},
                     yellow);

        DrawBox({{160, 120}, 320, 240}, blue);

        DrawQuad({{0.0, 0.0}, {640.0, 0.0}, {640.0, 480.0}, {0.0, 480.0}},
                 red);
    }

    void renderTranslucent()
    {
    }


};

int main(int argc, char **argv)
{
    LP3_LOG_DEBUG("PVR demo fun.");
    {
        PvrGraphics gfx;
        LP3_LOG_DEBUG("Press START on the controller to exit.");
        while(!Lp3::Engine::DemoUtils::DreamcastStartIsPressed()) {
            gfx.render();
        }
        LP3_LOG_DEBUG("Going to shut down PVR through miracle of RAII.");
    }
}

#endif // LP3_COMPILE_TARGET_DREAMCAST
