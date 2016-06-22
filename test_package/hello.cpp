#include <Lp3/LE.h>

#include <boost/format.hpp>
#include <Lp3/Log.h>
#include <Lp3/Engine/Gfx/Pixel.h>
#include <boost/shared_ptr.hpp>
#include <Lp3/Assert.h>
#include <Lp3/Engine/Resources/ReadStream.h>
#include <Lp3/Engine/Resources/ReadStreamPtr.h>
#include <Lp3/Engine/Gfx/GfxManager.hpp>
#include <Lp3/Engine/Gfx/Renderer.hpp>
#include <Lp3/Engine/Gfx/Geometry.hpp>
#include <Lp3/Engine/Gfx/ImageMemory.hpp>
#include <Lp3/Engine/Resources/ResettableReadStream.h>
#include <Lp3/Engine/Resources/InputFileStream.hpp>
#include <Lp3/Engine/Gfx/ScreenObject.h>
#include <Lp3/Engine/Gfx/ScreenObjectPtr.h>
#include <Lp3/Engine/Gfx/Screen.h>
#include <string>
#include <Lp3/Engine/Gfx/Texture.hpp>
#include <Lp3/Engine/Gfx/TexturePtr.hpp>
#include <Lp3/Engine/Gfx/TileSet.h>
#include <Lp3/Engine/Gfx/TileRenderer.h>
#include <vector>


using boost::format;
using Lp3::Engine::Gfx::GfxManager;
using Lp3::Engine::Gfx::ImageMemory;
using Lp3::Engine::Resources::InputFileStream;
using Lp3::Engine::Gfx::Pixel;
using Lp3::Engine::Resources::ReadStreamPtr;
using Lp3::Engine::Gfx::Renderer;
using Lp3::Engine::Gfx::Screen;
using Lp3::Engine::Gfx::ScreenObject;
using Lp3::Engine::Gfx::ScreenObjectPtr;
using std::string;
using Lp3::Engine::Gfx::Texture;
using Lp3::Engine::Gfx::TexturePtr;
using Lp3::Engine::Gfx::TileRenderer;
using Lp3::Engine::Gfx::TileSet;
using std::vector;


#define LP3_MICROMAIN_WITH_GFX_DEPS true
#include <Lp3/Engine/MicroMain.h>


GfxManager * manager;


using namespace Lp3::Engine::Gfx::Geometry;


class GraphicsObject : public ScreenObject
{
    TexturePtr textureText;
    std::unique_ptr<TileSet> tileSet;

    public: GraphicsObject(ImageMemory & memory)
    {
        LP3_LOG_DEBUG("Creating textureTexts...");
        ReadStreamPtr input(new InputFileStream("Engine/text.bmp"));
        const Pixel colorKey(0, 0, 0, 0);
        textureText = memory.LoadTexture(input, colorKey);
        tileSet.reset(new TileSet(textureText, {16, 16}, {16, 16}));
    }

    public: virtual void DrawOpaque(Renderer & renderer)
    {
    }

    public: virtual void DrawTranslucent(Renderer & renderer)
    {
        TileRenderer tr = tileSet->StartRenderer(renderer);
        tr.Write({32, 304 + 32}, 0.5f, "HELLO!");
    }
};


class Main
{
private:
    GfxManager manager;

public:
    Main(const vector<string> & arguments
         LP3_COMPILE_TARGET_WINDOWS_THEN(, HINSTANCE hInstance, HWND hWnd))
    :   manager(LP3_COMPILE_TARGET_WINDOWS_THEN(hWnd))
    {
        LP3_LOG_DEBUG("Initializing GFX...");
        Screen & screen = manager.GetScreen(0);
        ImageMemory & memory = screen.GetImageMemory();

        ScreenObjectPtr obj(new GraphicsObject(memory));
        screen.SetObject(obj);

                LP3_LOG_DEBUG("Finished initialization.");
    }

    ~Main()
    {
        LP3_LOG_DEBUG("Shutting down, good-bye.");
    }


    void Iterate()
    {
        manager.Render();
    }

    LP3_COMPILE_TARGET_WINDOWS_THEN(
        void WindowsMessage(HWND hWnd, UINT message, WPARAM wParam,
                            LPARAM lParam)
        {
            switch(message){
                case WM_SIZE:
                    /* Tell Gfx the size of our window. This
                     * does not affect the game's resolution. */
                    manager.Resize({ LOWORD(lParam), HIWORD(lParam) });
                    break;
                default:
                    break;
            }
        }
    )
};

LP3_MICROMAIN(Main, "Lp3::Engine::Gfx::ResizeWindow")
