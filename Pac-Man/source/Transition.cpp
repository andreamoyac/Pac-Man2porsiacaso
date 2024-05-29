#include "Transition.h"

Transition::Transition()
{
    is_active = false;
    from = GameState::MAIN_MENU;
    to = GameState::MAIN_MENU;
    frames_from = 0;
    frames_to = 0;
    frames_counter = 0;
    rect = {};
    toScene = 1;
    state = TransitionState::IN;

    dst = {};
}
Transition::~Transition()
{
}
void Transition::Set(GameState to, int frames_to, const Rectangle& rect)
{
    is_active = true;

    this->to = to;
    this->frames_to = frames_to;
    frames_counter = 0;
    this->rect = rect;

    state = TransitionState::IN;
}
void Transition::Set(GameState from, int frames_from, GameState to, int frames_to, const Rectangle& rect)
{
    is_active = true;

    this->from = from;
    this->to = to;
    this->frames_from = frames_from;
    this->frames_to = frames_to;
    frames_counter = 0;
    this->rect = rect;

    state = TransitionState::OUT;
}
void Transition::SetScene(int to, int frames_from, int frames_to)
{
    is_active = true;

    w = WINDOW_WIDTH * GAME_SCALE_FACTOR;
    h = WINDOW_HEIGHT * GAME_SCALE_FACTOR;
    dst = { 0, 0, w, h };

    this->toScene = to;
    this->frames_from = frames_from;
    this->frames_to = frames_to;
    this->rect = dst;

    state = TransitionState::IN;
}

void Transition::SetScene(int from, int to, int frames_from, int frames_to)
{
    is_active = true;

    w = WINDOW_WIDTH * GAME_SCALE_FACTOR;
    h = WINDOW_HEIGHT * GAME_SCALE_FACTOR;
    dst = { 0, 0, w, h };

    this->toScene = to;
    this->frames_from = frames_from;
    this->frames_to = frames_to;
    this->rect = dst;

    state = TransitionState::OUT;
}

bool Transition::IsActive()
{
    return is_active;
}
GameState Transition::Update()
{
    if (is_active)
    {
        frames_counter++;

        //Update the fade state based on the current state
        if (state == TransitionState::OUT)
        {
            //Check if it has reached the end
            if (frames_counter >= frames_from)
            {
                state = TransitionState::IN;
                frames_counter = 0;
            }
        }
        else if (state == TransitionState::IN)
        {
            //Check if it has reached the end
            if (frames_counter >= frames_to)
            {
                is_active = false; // Transition complete
            }
        }
    }
    //If the fade effect is currently in the "fade-out" state,
    //return the source state (from), otherwise return the target state (to).
    return state == TransitionState::OUT ? from : to;
}
void Transition::Render()
{
    float alpha;

    if (is_active)
    {
        if (state == TransitionState::OUT)
        {
            //Calculate the alpha value based on the current frame and total frames
            alpha = (float)frames_counter / (float)frames_from;

            //Black rectangle with alpha value from 0 to 1
            //DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(BLACK, alpha));
            //Color color = { 0,0,0,(unsigned char)(alpha * 255.0f) };
            Color color = Fade(BLACK, 255/*alpha*/);
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
        }
        else if (state == TransitionState::IN)
        {
            //Calculate the alpha value based on the current frame and total frames
            alpha = (float)frames_counter / (float)frames_to;

            //Black rectangle with alpha value from 1 to 0
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(BLACK, 255/*1-alpha*/));
        }
    }
}
