#include <GL/gl.h>
#include "Display.h"

using namespace Hiage;
using namespace std;

Display::Display(const Renderer& renderer) : renderer(renderer)
{
    fullscreen = false;
    cameraX = 0;
    cameraY = 0;
    zoom = 100;
    renderedZoom = zoom;
}

Display::~Display()
{
    //If fullscreen, reset video mode.
    if (fullscreen)
        setFullscreen(false);
}

/// <summary>
/// Initialize the display, i.e. set up OpenGL, SDL, Direct3D, whatever and create a window.
/// </summary>
/// <param name="width">
/// A <see cref="System.Int32"/>
/// </param>
/// <param name="height">
/// A <see cref="System.Int32"/>
/// </param>
void Display::Initialize(int width, int height, const ResourceManager& resourceManager, string windowTitle)
{
    //Initialize variables
    this->width = width;
    this->height = height;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    //Create the window
    screen = SDL_SetVideoMode(width, height, 32, SDL_RESIZABLE | SDL_OPENGL | SDL_HWSURFACE);
    SDL_WM_SetCaption(windowTitle.c_str(), NULL);

//TODO: move into renderer?
    PrepareViewport();
//TODO
//    Events.VideoResize += new EventHandler<VideoResizeEventArgs>(OnResize);
}

void Display::PrepareViewport()
{
    if (height == 0)
        height = 1;

    if (width == 0)
        width = 1;

    //Set the viewport dimensions, and store the current matrix
    glPushMatrix();

    //Set the matrix mode to modify the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    //Set the perspective
    if (width > height)
        glOrtho(-zoom * getAspectRatio(), zoom * getAspectRatio(), -zoom, zoom, -1.0, 1000.0);
    else
        glOrtho(-zoom, zoom, -zoom / getAspectRatio(), zoom / getAspectRatio(), -1.0, 1000.0);

    //Restore previous matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/// <summary>
/// Prepare the window for rendering, like clearing the screen and so on.
/// </summary>
void Display::PrepareRender()
{
    if (renderedZoom != zoom)
    {
        PrepareViewport();
        renderedZoom = zoom;
    }

    glLoadIdentity();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslated(-cameraX, -cameraY, 0);
    renderedCameraX = cameraX;
    renderedCameraY = cameraY;
}

/// <summary>
/// Flip the surface, the buffer or whatever to make the good stuff show.
/// </summary>
void Display::Render() const
{
    SDL_GL_SwapBuffers();
}

/// <summary>
/// Check wether or not a point (x,y) is inside the viewport or not.
/// </summary>
/// <param name="x">
/// A <see cref="System.Double"/>
/// </param>
/// <param name="y">
/// A <see cref="System.Double"/>
/// </param>
/// <returns>
/// A <see cref="System.Boolean"/>
/// </returns>
bool Display::InViewport(double x, double y) const
{
    return (x >= (cameraX-renderedZoom) && x <= (cameraX+renderedZoom) && y <= (cameraY+renderedZoom) && y >= (cameraY-renderedZoom));
}

Vector Display::getDisplayToWorldCoordinates(double x, double y) const
{
    return Vector(renderedCameraX - getViewportWidth()/2 + x, renderedCameraY - getViewportHeight()/2 + y);
}

double Display::getViewTop() const
{
    return renderedCameraY + getViewportHeight()/2;
}

double Display::getViewBottom() const
{
    return renderedCameraY - getViewportHeight()/2;
}
double Display::getViewLeft() const
{
    return renderedCameraX - getViewportWidth()/2;
}

double Display::getViewRight() const
{
    return renderedCameraX + getViewportWidth()/2;
}

/// Handle resizes
//void Display::OnResize(object sender, VideoResizeEventArgs args)
//{
//    width = args.Width;
//    height = args.Height;
//
//    PrepareViewport();
//    glViewport(0, 0, width, height);
//    //screen = Video.SetVideoMode(width, height, 32, true, true, false);
//    screen = SDL_SetVideoMode(width, height, 32, SDL_RESIZABLE | SDL_OPENGL | SDL_HWSURFACE);
//}

double Display::getAspectRatio() const
{
    return (double)width / (double)height;
}

//// <value>
/// Camera X position.
/// </value>
double Display::getCameraX() const
{
    return cameraX;
}

void Display::setCameraX(double value)
{
    cameraX = value;
}

//// <value>
/// Camera Y position.
/// </value>
double Display::getCameraY() const
{
    return cameraY;
}

void Display::setCameraY(double value)
{
    cameraY = value;
}

/// <summary>
/// The camera values as they were when the scene was rendered
/// </summary>
double Display::getRenderedCameraX() const
{
    return renderedCameraX;
}

/// <summary>
/// The camera values as they were when the scene was rendered
/// </summary>
double Display::getRenderedCameraY() const
{
    return renderedCameraY;
}

//// <value>
/// Set/get fullscreen
/// </value>
bool Display::getFullscreen() const
{
    unsigned int flags = screen->flags;

    return flags & SDL_FULLSCREEN;
}

bool Display::setFullscreen(bool value)
{
    unsigned int flags = SDL_RESIZABLE | SDL_OPENGL | SDL_HWSURFACE;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;

    screen = SDL_SetVideoMode(width, height, 32, flags);
}

//// <value>
/// Get a reference to the renderer.
/// </value>
const Renderer& Display::getRenderer() const
{
    return renderer;
}

//// <value>
/// Height of drawing area.
/// </value>
int Display::getViewportHeight() const
{
    if (height > width)
        return (int)(renderedZoom*2*getAspectRatio());

    return (int)renderedZoom*2;
}

//// <value>
/// Width of the drawing area. I.e. if ViewportWidth == 100, a square with width 100 will reach across the screen.
/// </value>
int Display::getViewportWidth() const
{

    if (width>height)
    {
        return (int)(renderedZoom*2*getAspectRatio());
    }
    return (int)renderedZoom*2;
}

//// <value>
/// Zoom level. How much zoomage.
/// </value>
double Display::getZoom() const
{
    return zoom;
}

double Display::setZoom(double value)
{
    if (zoom > 0)
        zoom = value;
    else
        zoom = 1;
}

double Display::getRenderedZoom() const
{
    return renderedZoom;
}

int Display::getWindowWidth() const
{
    return width;
}

int Display::getWindowHeight() const
{
    return height;
}
