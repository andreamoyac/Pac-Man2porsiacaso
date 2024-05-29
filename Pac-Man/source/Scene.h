#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "EnemyManager.h"
#include "Text.h"
#include "Transition.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

    void ScreenReset();
    void TimerReset();
    bool GameOver();
    bool End();

private:
    AppStatus LoadLevel(int stage);
    
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    unsigned int timer;
    unsigned int timerComparision;
    bool dyingTimer;
    Player* player;
    Enemy* enemy;
    int currentLevel;
    
    
    TileMap *level;
    TileMap *InteractableLevel;
    
    
    std::vector<Object*> objects;

    EnemyManager* enemies;

    bool ghost;
    bool ghostie;
    //Enemy* ghostsLeft[2];
    Camera2D camera;
    DebugMode debug;


    GameState state;

    Text* font;
    Transition transition;
};

