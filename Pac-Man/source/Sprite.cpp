#include "Sprite.h"

Sprite::Sprite(const Texture2D *texture)
{
    img = texture;
    AnimationFinished = false;
    current_anim = -1;
    current_frame = 0;
    current_delay = 0;
    mode = AnimMode::AUTOMATIC;
    completed_animation = false;
}
Sprite::~Sprite()
{
    Release();
}
void Sprite::SetNumberAnimations(int num)
{
    animations.clear();
    animations.resize(num);
}
void Sprite::SetAnimationDelay(int id, int delay)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].delay = delay;
    }
}
void Sprite::AddKeyFrame(int id, const Rectangle& rect)
{
    if (id >= 0 && id < animations.size())
    {
        animations[id].frames.push_back(rect);
    }
}
void Sprite::SetAnimation(int id)
{
    if (id >= 0 && id < animations.size())
    {
        current_anim = id;
        current_frame = 0;
        current_delay = animations[current_anim].delay;
        completed_animation = false;
    }
}
int Sprite::GetAnimation()
{
    return current_anim;
}
bool Sprite::GetAnimationFinished() const
{
    return AnimationFinished;
}
void Sprite::SetAnimationFinished(bool value)
{
    AnimationFinished = value;
}
void Sprite::SetManualMode()
{
    mode = AnimMode::MANUAL;
}
void Sprite::SetAutomaticMode()
{
    mode = AnimMode::AUTOMATIC;
}
bool Sprite::AnimationCompleted() const
{
    return completed_animation;
}
void Sprite::RepeatOnceMore()
{
    mode = AnimMode::PLAYAGAIN;
    SetAnimationFinished(false);
}
void Sprite::Update()
{
   // both animation modes (automatic and manual) are carry out with animation delay
    if (current_delay > 0)
    {
        current_delay--;
        if (current_delay == 0)
        {
            //only automatic animation mode advances next frame
            if (mode == AnimMode::AUTOMATIC)
            {
                current_frame++;
                current_frame %= animations[current_anim].frames.size();
                current_delay = animations[current_anim].delay;

                //Animation is complete when we repeat from the first frame
                completed_animation = (current_frame == 0);
            }
            if (mode == AnimMode::PLAYAGAIN)
            {

                current_frame++;
                //current_delay = animations[current_anim].delay;
                current_delay = animations[current_anim].delay;
                //current_delay = animations[current_anim].delay;
                //if (current_frame == animations[current_anim].frames.size() - 1)
                if (current_frame == animations[current_anim].frames.size())
                {
                    SetAnimationFinished(true);
                    mode = AnimMode::AUTOMATIC;
                }
            }
        }
    }
}
void Sprite::NextFrame()
{
    //Next frame is only available in manual animation mode
    if (mode == AnimMode::MANUAL)
    {
        current_delay--;
        if (current_delay <= 0)
        {
            current_frame++;
            current_frame %= animations[current_anim].frames.size();
            current_delay = animations[current_anim].delay;
        }
    }   
}
void Sprite::PrevFrame()
{
    //Previous frame is only available in manual animation mode
    if (mode == AnimMode::MANUAL)
    {
        current_delay--;
        if (current_delay <= 0)
        {
            current_frame--;
            current_frame %= animations[current_anim].frames.size();
            current_delay = animations[current_anim].delay;
        }
    }
}
void Sprite::Draw(int x, int y) const
{
    DrawTint(x, y, WHITE);
}
void Sprite::DrawTint(int x, int y, const Color& col) const
{
    if (current_anim >= 0 && current_anim < animations.size())
    {
        
        int n = animations[current_anim].frames.size();
        if (current_frame >= 0 && current_frame < n)
        {
            Rectangle rect = animations[current_anim].frames[current_frame];
            DrawTextureRec(*img, rect, { (float)x, (float)y }, col);
        }
    }
}
void Sprite::Release()
{
    //Release each animation's frames
    for (auto& animation : animations)
    {
        animation.frames.clear();
    }
    //Clear the animations vector
    animations.clear();
}
