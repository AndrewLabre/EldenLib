#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "../headers/HP_Item.hpp"


// GLOBALS
std::string item_state_name;

// UTILS
void CheckCollision(HP_Item& hp_item)
{
    for (int i=0; i<hp_item.entities->size(); i++)
    {
        if(hp_item.entities -> at(i) -> entity_type == "Enemy")
        {
            Rectangle enemy = {hp_item.entities -> at(i) -> position.x,
                               hp_item.entities -> at(i) -> position.y,
                               hp_item.entities -> at(i) -> size.x,
                               hp_item.entities -> at(i) -> size.y};
            if(CheckCollisionCircleRec(hp_item.position, hp_item.radius, enemy))
            {
                hp_item.entities -> at(i) -> hp += hp_item.hp;
                hp_item.TakeDmg();
            }
        }

        if(hp_item.entities -> at(i) -> entity_type == "Player")
        {
            if(CheckCollisionCircles(hp_item.position, hp_item.radius, hp_item.entities -> at(i) -> position, hp_item.entities -> at(i) -> size.y))
            {
                hp_item.entities -> at(i) -> hp += hp_item.hp;
                hp_item.TakeDmg();
            }
        }
    }
}

// ITEM
void HP_Item::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}
void HP_Item::Draw() {
    DrawCircleV(position, radius, color);
    DrawText(item_state_name.c_str(), position.x - (MeasureText(item_state_name.c_str(), 20.0f) / 2.0f), position.y - radius - 20.0f, 20.0f, GREEN);
}
void HP_Item::SetState(HP_ItemState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}
void HP_Item::TakeDmg() {
    hp = 0.0f;
}
HP_ItemState* HP_Item::GetState() {
    return current_state;
}
HP_Item::HP_Item(std::string i, Vector2 pos, float rad, float h, std::vector<Entity*>* ent) {
    nme = i;
    name = i;
    position = pos;
    radius = rad;
    hp = h;
    basic_dmg = 0.0f;
    entity_type = "Item";
    entities = ent;
    size.x = 0.0f;
    size.y = rad;
    SetState(&item_stat);
}


// STATES
void HP_ItemStatic::Enter(HP_Item& hp_item) {
    hp_item.color = PINK;
    hp_item.active_time = 0.0f;
    item_state_name = "< HP >";
}


void HP_ItemStatic::Update(HP_Item& hp_item, float delta_time) {
    CheckCollision(hp_item);
    float spawn_time = 8.0f;

    if((spawn_time - hp_item.active_time/1) < 1.0f)
    {
        hp_item.color.a -= 1000*delta_time;
    }

    if (hp_item.active_time > spawn_time) {
        hp_item.TakeDmg();
    }
    else {
        hp_item.active_time += delta_time;
    }
}