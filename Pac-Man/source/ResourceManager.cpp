#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
}
ResourceManager::~ResourceManager()
{
    Release();
}

AppStatus ResourceManager::LoadTexture(Resource id, const std::string& file_path)
{
    Texture2D texture = ::LoadTexture(file_path.c_str());
    if (texture.id == 0)
    {
        LOG("Failed to load texture ", file_path);
        return AppStatus::ERROR;
    }
    
    textures[id] = texture;
    return AppStatus::OK;
}

void ResourceManager::ReleaseTexture(Resource id)
{
    auto it = textures.find(id);

    if (it != textures.end())
    {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

void ResourceManager::LoadSounds()
{
    sounds[0] = LoadSound("Sound/FX/credit.wav");
    sounds[1] = LoadSound("Sound/FX/death_1.wav");
    sounds[2] = LoadSound("Sound/FX/death_2.wav");
    sounds[3] = LoadSound("Sound/FX/eat_fruit.wav");
    sounds[4] = LoadSound("Sound/FX/eat_ghost.wav");
    sounds[5] = LoadSound("Sound/FX/extend.wav");
    sounds[6] = LoadSound("Sound/FX/munch_1.wav");
    sounds[7] = LoadSound("Sound/FX/munch_2.wav");
    sounds[8] = LoadSound("Sound/FX/power_pellet.wav");
    sounds[9] = LoadSound("Sound/FX/retreating.wav");
    sounds[10] = LoadSound("Sound/FX/siren_1.wav");
    sounds[11] = LoadSound("Sound/FX/siren_2.wav");
    sounds[12] = LoadSound("Sound/FX/siren_3.wav");
    sounds[13] = LoadSound("Sound/FX/siren_4.wav");
    sounds[14] = LoadSound("Sound/FX/siren_5.wav");
}
void ResourceManager::ReleaseSounds()
{
    for (int i = 0; i < 14; i++)
    {
        UnloadSound(sounds[i]);
    }
}




const Texture2D* ResourceManager::GetTexture(Resource id) const
{
    auto it = textures.find(id);
    if (it != textures.end())   return &(it->second);

    return nullptr;
}

void ResourceManager::Release()
{
    for (auto& pair : textures)
    {
        UnloadTexture(pair.second);
    }
    textures.clear();
}