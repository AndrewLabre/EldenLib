#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "../headers/ATK_Item.hpp"


// GLOBALS
std::string item2_state_name;

// UTILS
void CheckCollision(ATK_Item& atk_item)
{
    for (int i=0; i<atk_item.entities->size(); i++)
    {
        if(atk_item.entities -> at(i) -> entity_type == "Enemy")
        {
            Rectangle enemy = {atk_item.entities -> at(i) -> position.x,
                               atk_item.entities -> at(i) -> position.y,
                               atk_item.entities -> at(i) -> size.x,
                               atk_item.entities -> at(i) -> size.y};
            if(CheckCollisionCircleRec(atk_item.position, atk_item.radius, enemy))
            {
                atk_item.entities -> at(i) -> basic_dmg += atk_item.basic_dmg;
                atk_item.TakeDmg();
            }
        }

        if(atk_item.entities -> at(i) -> entity_type == "Player")
        {
            if(CheckCollisionCircles(atk_item.position, atk_item.radius, atk_item.entities -> at(i) -> position, atk_item.entities -> at(i) -> size.y))
            {
                atk_item.entities -> at(i) -> basic_dmg += atk_item.basic_dmg;
                atk_item.TakeDmg();
            }
        }
    }
}

// ITEM
void ATK_Item::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}
void ATK_Item::Draw() {
    DrawCircleV(position, radius, color);
    DrawText(item2_state_name.c_str(), position.x - (MeasureText(item2_state_name.c_str(), 20.0f) / 2.0f), position.y - radius - 20.0f, 20.0f, GREEN);
}
void ATK_Item::SetState(ATK_ItemState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}
void ATK_Item::TakeDmg() {
    hp = 0.0f;
}
ATK_ItemState* ATK_Item::GetState() {
    return current_state;
}
ATK_Item::ATK_Item(std::string i, Vector2 pos, float rad, float dmg, std::vector<Entity*>* ent) {
    nme = i;
    name = i;
    position = pos;
    radius = rad;
    hp = 10.0f;
    basic_dmg = dmg;
    entity_type = "Item";
    entities = ent;
    size.x = 0.0f;
    size.y = rad;
    SetState(&item_stat);
}


// STATES
void ATK_ItemStatic::Enter(ATK_Item& atk_item) {
    atk_item.color = PINK;
    atk_item.active_time = 0.0f;
    item2_state_name = "< ATK >";
}


void ATK_ItemStatic::Update(ATK_Item& atk_item, float delta_time) {
    CheckCollision(atk_item);
    float spawn_time = 8.0f;

    if((spawn_time - atk_item.active_time/1) < 1.0f)
    {
        atk_item.color.a -= 1000*delta_time;
    }

    if (atk_item.active_time > spawn_time) {
        atk_item.TakeDmg();
    }
    else {
        atk_item.active_time += delta_time;
    }
}