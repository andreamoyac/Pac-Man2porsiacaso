#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::CREDITS;
    scene = nullptr;
    img_menu = nullptr;
    maze_img = nullptr;
    creditsimg = nullptr;
    intro_img = nullptr;
    ending_img = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man");

    InitAudioDevice();

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }


    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);


    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/MAINMENU.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    if (data.LoadTexture(Resource::MAZE_IMG, "images/Pac-Man.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    maze_img = data.GetTexture(Resource::MAZE_IMG);
    if (data.LoadTexture(Resource::IMG_CREDITS, "images/CREDITS.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    creditsimg = data.GetTexture(Resource::IMG_CREDITS);
    if (data.LoadTexture(Resource::IMG_INTRO, "images/INTRO.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    intro_img = data.GetTexture(Resource::IMG_INTRO);
    if (data.LoadTexture(Resource::IMG_ENDING, "images/END.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ending_img = data.GetTexture(Resource::IMG_ENDING);

    data.LoadSounds();

    IntermissionPacman = LoadMusicStream("Sound/Music/intermission_music_ogg2.ogg");
    GameOverPacman = LoadMusicStream("Sound/Music/Pacman_Game_over.ogg");
    ThemePacman = LoadMusicStream("Sound/Music/Pacman_theme_OGG.ogg");
    ThemePacman.looping = true;
    SetMusicVolume(ThemePacman, 0.5);


    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();

    PlayMusicStream(ThemePacman);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    StopMusicStream(ThemePacman);
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;
    //if (transition.IsActive())
    //{
    //    GameState prev_frame = state;
    //    //state = transition.Update();

    //    //Start and finish delayed due to the fading transition
    //    if (prev_frame == GameState::MAIN_MENU && state == GameState::PLAYING)
    //    {
    //        if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
    //    }
    //    else if (prev_frame == GameState::PLAYING && state == GameState::MAIN_MENU)
    //    {
    //        FinishPlay();
    //    }
    //}
    //else
    //{
        switch (state)
        {
        case GameState::CREDITS:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::INTRO;
            }
            break;
        case GameState::INTRO:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::MAIN_MENU;
            }
            break;
        case GameState::MAIN_MENU:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
                transition.Set(GameState::MAIN_MENU, 5, GameState::PLAYING, 5, dst);
            }
            break;

        case GameState::PLAYING:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                //FinishPlay();
                state = GameState::MAIN_MENU;
               transition.Set(GameState::PLAYING, 5, GameState::MAIN_MENU, 5, dst);

                //StopMusicStream(Ost2PacMan);
            }
            /*else if (IsKeyPressed(KEY_F2))
            {
                state = GameState::GAME_OVER;
                StopMusicStream(Ost2PacMan);
            }
            else if (scene->GameOver() == true)
            {
                state = GameState::GAME_OVER;
                StopMusicStream(Ost2PacMan);
            }
            else if (IsKeyPressed(KEY_F3))
            {
                state = GameState::ENDING;
                StopMusicStream(Ost2PacMan);
            }
            else if (scene->GameEnd() == true)
            {
                state = GameState::ENDING;
                StopMusicStream(Ost2PacMan);
            }*/
            else
            {
                //Game logic
                scene->Update();
                UpdateMusicStream(ThemePacman);
                return AppStatus::OK;
            }
            break;
        
        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ESCAPE))
                return AppStatus::QUIT;
            else if (IsKeyPressed(KEY_ENTER))
                state = GameState::MAIN_MENU;
            break;
        case GameState::ENDING:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::MAIN_MENU;
            }
            break;
        }
        return AppStatus::OK;
    }

 /*   return AppStatus::OK;
}*/
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::CREDITS:
            DrawTexturePro(*creditsimg, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            //DrawTextureEx(*creditsimg, Vector2() , 0, 2, WHITE);
            break;

        case GameState::INTRO:
            //playCreditsAnimation();
            DrawTexturePro(*intro_img, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            break;

        case GameState::MAIN_MENU:
            DrawTexturePro(*img_menu, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT }, { 0,0 }, 0, WHITE);
            //DrawTextureEx(*img_menu, Vector2() , 0, 2, WHITE);
            break;
        case GameState::PLAYING:
            scene->Render();
            DrawTextureEx(*maze_img, Vector2(), 0, 2, { 255,255,255,150 });
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
  /*  if (transition.IsActive()) transition.Render();*/
    EndDrawing();
}
void Game::Cleanup()
{
    CloseAudioDevice();
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::MAZE_IMG);
    data.ReleaseTexture(Resource::IMG_CREDITS);
    data.ReleaseTexture(Resource::IMG_INTRO);
    data.ReleaseTexture(Resource::IMG_ENDING);
    data.ReleaseSounds();

    UnloadRenderTexture(target);
    UnloadMusicStream(ThemePacman);
}