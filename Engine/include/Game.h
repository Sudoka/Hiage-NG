#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <boost/filesystem.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Display.h"
#include "GameState.h"
#include "Timer.h"

namespace Hiage
{
    class InputManager;
    class AudioManager;
    class ResourceManager;
    class Log;

    class Game
    {
    public:
        Game(Log& log, Display& display, InputManager& inputManager, AudioManager& audioManager, ResourceManager& resourceManager);
        ~Game();

        void Initialize(int width, int height, bool fullscreen, std::string windowTitle);
        void PushState(std::shared_ptr<GameState> state);
        std::shared_ptr<GameState> PopState();
        void Run();
        void Shutdown();

        void setSimulationSpeed(double speed);
    protected:
    private:
		Display& 			display;
		ResourceManager& 	resourceManager;
		InputManager&		inputManager;
		AudioManager&		audioManager;
		std::vector<std::shared_ptr<GameState> > 	gameStates;
		Timer 			    frameTimer;
		int 				fps = 60;
		double 				currentFps = 0;
		double				lastFrameTime = 0;
		bool 				done = false;
        double              simulationSpeed;
        double              gameTime = 0;

		Log& log;
    };
}

#endif // GAME_H_INCLUDED
