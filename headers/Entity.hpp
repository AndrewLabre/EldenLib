//BASE CLASS FOR ALL ENTITY TYPES

#ifndef ENTITY
#define ENTITY

#include <raylib.h>
#include <raymath.h>
#include <iostream>

class Entity {
public:
    std::string nme;
    Vector2 position;
    Vector2 previousPosition;
    Vector2 size;
    float hp;
    float basic_dmg;
    float incoming_dmg;
    virtual ~Entity() {}
    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;
    virtual void TakeDmg() = 0;
    std::string entity_type;

    // Sprite
    Texture2D texture;
    std::vector<std::vector<Rectangle>> animations;
    int animation_index = 0, animation_frame = 0, previous_animation_index = 0;
    float animation_frame_timer = 0.14f;
    float animation_timer = 0.0f;

    //Sounds
    std::vector<Sound> sounds;

    void AssignTexture(std::string filename)
    {
        texture = LoadTexture(filename.c_str());
    }

    void UnassignTexture()
    {
        UnloadTexture(texture);
    }

    void AddSound(std::string filename)
    {
        Sound new_sound;

        new_sound = LoadSound(filename.c_str());

        sounds.push_back(new_sound);
    }

    void UnloadSounds()
    {
        for(int i = 0; i < sounds.size(); i++)
        {
            UnloadSound(sounds[i]);
        }
    }

    void PlaySoundInList(int index)
    {
        PlaySound(sounds[index]);
    }

    void StopSoundInList(int index)
    {
        StopSound(sounds[index]);
    }
};

#endif