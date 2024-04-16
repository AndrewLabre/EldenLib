#ifndef HP_ITEM
#define HP_ITEM

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Entity.hpp"

class HP_Item;

class HP_ItemState {
public:
    virtual ~HP_ItemState() {}
    virtual void Enter(HP_Item& hp_item) = 0;
    virtual void Update(HP_Item& hp_item, float delta_time) = 0;
};

class HP_ItemStatic : public HP_ItemState {
public:
    void Enter(HP_Item& hp_item);
    void Update(HP_Item& hp_item, float delta_time);
};


class HP_Item : public Entity {
public:
    std::string name;
    float radius;
    Color color;

    float active_time = 0.0f;

    std::vector<Entity*> *entities;

    HP_ItemStatic item_stat;

    HP_Item(std::string id, Vector2 pos, float radius, float hp, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(HP_ItemState* new_state);
    void TakeDmg();
    HP_ItemState* GetState();

private:
    HP_ItemState* current_state;
};

#endif