//BASE CLASS FOR ALL ENTITY TYPES

#ifndef ENTITY
#define ENTITY

#include <raylib.h>
#include <raymath.h>
#include <iostream>

class Entity {
public:
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
};

#endif