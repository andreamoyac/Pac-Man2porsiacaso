#pragma once
#include "Globals.h"
#include "Scene.h"
#include "Transition.h"

enum class GameState { MAIN_MENU, PLAYING, CREDITS, INTRO, GAME_OVER, END };

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
    const Texture2D* credits_img;
    const Texture2D* intro_img;
    const Texture2D* ending_img;

    bool animationFinished=false;
    bool animationFinished2=false;


    Music IntermissionPacman;
    Music GameOverPacman;
    Music ThemePacman;
    Sound IntermissionPacmanSound;
    Sound GameOverPacmanSound;
    Sound ThemePacmanSound;

    int timerIntro;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;

    //TRANSITION
    Transition transition;
};