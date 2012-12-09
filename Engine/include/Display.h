#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <SDL/SDL.h>
#include "Vector.h"

namespace Hiage
{
    class ResourceManager;
    class Renderer;

    class Display
    {
        public:
            Display(const Renderer& renderer);
            virtual ~Display();

            void Initialize(int width, int height, const ResourceManager& resourceManager, std::string windowTitle);
            void PrepareViewport();

            /// Prepare the window for rendering, like clearing the screen and so on.
            void PrepareRender();

            /// Flip the surface, the buffer or whatever to make the good stuff show.
            void Render() const;

            /// Check wether or not a point (x,y) is inside the viewport or not.
            bool InViewport(double x, double y) const;
            Vector getDisplayToWorldCoordinates(double x, double y) const;
            double getViewTop() const;
            double getViewBottom() const;
            double getViewLeft() const;
            double getViewRight() const;
            double getAspectRatio() const;

            double getCameraX() const;
            void setCameraX(double value);

            double getCameraY() const;
            void setCameraY(double value);

            /// The camera coordinates as they were when the scene was rendered
            double getRenderedCameraX() const;
            double getRenderedCameraY() const;

            bool getFullscreen() const;
            bool setFullscreen(bool value);

            //// <value>
            /// Get a reference to the renderer.
            /// </value>
            const Renderer& getRenderer() const;

            /// Height of drawing area.
            int getViewportHeight() const;
            /// Width of the drawing area. I.e. if ViewportWidth == 100, a square with width 100 will reach across the screen.
            int getViewportWidth() const;

            //// <value>
            /// Zoom level. How much zoomage.
            /// </value>
            double getZoom() const;
            double setZoom(double value);

            double getRenderedZoom() const;

            int getWindowWidth() const;
            int getWindowHeight() const;

        protected:
        private:
            SDL_Surface* screen;
            int width, height;
            double cameraX, cameraY, renderedCameraX, renderedCameraY;
            double zoom, renderedZoom;
            const Renderer& renderer;
            bool fullscreen;
    };
}

#endif // DISPLAY_H
