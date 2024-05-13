#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, CREDITS, INTRO };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    void playCreditsAnimation();
    void playIntroAnimation();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* maze_img;
    bool animationFinished;
    bool animationFinished2;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};