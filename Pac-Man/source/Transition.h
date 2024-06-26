#pragma once
#include "Globals.h"
//#include "Game.h"
#include <raylib.h>

//fade out: gradual decrease in visibility
//fade in: gradual increase in visibility
enum class TransitionState { OUT, IN };

class Transition
{
public:
    Transition();
    ~Transition();

    //Set a fade-in effect to a target state in a number of frames
    void Set(GameState to, int frames_to, const Rectangle& rect);

    //Set a fade-out and fade-in effect from a source state to a target state over in a numbers of frames
    void Set(GameState from, int frames_from, GameState to, int frames_to, const Rectangle& rect);

    // For scenes
    void SetScene(int to, int frames_from, int frames_to);
    void SetScene(int from, int to, int frames_from, int frames_to);

    //Check if the fade effect is currently active
    bool IsActive();

    //Update the fade effect over time and return the current state after the update
    GameState Update();

    //Render the fade effect
    void Render();

private:
    //Flag indicating whether the fade effect is active
    bool is_active;

    //Source and target states of the fade effect
    GameState from, to;

    //Number of frames for fading from and to the target state
    int frames_from, frames_to;

    //Counter to track the current frame during the fade effect
    int frames_counter;

    int toScene;

    //Rectangle defining area where fade effect applied
    Rectangle rect;

    //Current state of effect
    TransitionState state;




    Rectangle dst;
    float w, h;
};
