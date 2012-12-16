#include <string>
#include "AudioManager.h"
#include "Game.h"
#include "Exceptions.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Log.h"

using namespace std;
using namespace Hiage;

Game::Game(Log& log, Display& display, InputManager& inputManager, AudioManager& audioManager, ResourceManager& resourceManager) 
    : display(display), resourceManager(resourceManager), inputManager(inputManager), audioManager(audioManager),  log(log)
{
//    Events.Quit += new EventHandler<QuitEventArgs>(OnQuit);
}

void Game::Initialize(int width, int height, bool fullscreen, string windowTitle)
{
    log.Write("Game", "Engine initializing");

    //Create resource manager and load resources from the main resource file
    resourceManager.LoadResources("data");
    //resourceManager.LoadResourceXML(ResourceManager.MainResourceFile);

    //Create the opengl display
    display.Initialize(width, height, resourceManager, windowTitle);
    if (fullscreen)
        display.setFullscreen(true);

    frameTimer.Start();
    setSimulationSpeed(-1);
}

/// <summary>
/// Push a new game state onto the state stack
/// </summary>
/// <param name="state">
/// A <see cref="IGameState"/>
/// </param>
/// <returns>
/// Previous <see cref="IGameState"/>
/// </returns>
void Game::PushState(shared_ptr<GameState> state)
{
    gameStates.push_back(state);
   //state.Initialize(this);

    gameStates.back()->IssueActivateEvent();
}

/// <summary>
/// Pop (remove) the top game state
/// </summary>
shared_ptr<GameState> Game::PopState()
{
    if (gameStates.size() == 0)
        throw IndexOutOfRangeException("PopState: No state to pop.");

    auto temp = gameStates.back();
    gameStates.pop_back();

    if (gameStates.size() > 0)
        gameStates.back()->IssueActivateEvent();

    return temp;
}

/// <summary>
/// Run the game states and renderer
/// </summary>
void Game::Run()
{
    while (!done)
    {
        SDL_Event event;
        
        //Poll for events (Close, resize etc.)
        while (SDL_PollEvent(&event))
        {
        }

        frameTimer.Reset();
        frameTimer.Start();

        display.PrepareRender();

        if (gameStates.size() > 0)
            gameStates.back()->Run(simulationSpeed < 0 ? lastFrameTime : simulationSpeed);

        display.Render();

        if (fps > 0) while (frameTimer.getElapsedMilliseconds() < (1/(double)fps*1000)) {}
        else while (frameTimer.getElapsedMilliseconds() == 0) {}

        lastFrameTime = (double)frameTimer.getElapsedMilliseconds() / 1000.0;
        //TODO: Remove the following line
        //lastFrameTime = 0.02;
        gameTime += lastFrameTime;
        currentFps = 1.0/lastFrameTime;
    }
}

/// <summary>
/// Quit the application.
/// </summary>
void Game::Shutdown()
{
    done = true;
    //TODO
    //Events.QuitApplication();
    audioManager.PlayMusic("foo");
    //Thread.CurrentThread.Abort();
}

/// <summary>
/// Handle closing of the window, causing the application to exit.
/// </summary>
/// <param name="sender">
/// A <see cref="System.Object"/>
/// </param>
/// <param name="args">
/// A <see cref="QuitEventArgs"/>
/// </param>
//void Game::OnQuit(object sender, QuitEventArgs args)
//{
//    Shutdown();
//}

//// <value>
/// FPS cap
/// </value>
//int MaxFPS
//{
//    get
//    {
//        return fps;
//    }
//    set
//    {
//        fps = value;
//    }
//}
//
/// <value>
/// Current FPS
/// </value>
//public double FPS
//{
//    get
//    {
//        return currentFps;
//    }
//}



void Game::setSimulationSpeed(double speed)
{
    simulationSpeed = speed;
}
