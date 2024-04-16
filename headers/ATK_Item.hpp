#ifndef ATK_ITEM
#define ATK_ITEM

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Entity.hpp"

class ATK_Item;

class ATK_ItemState {
public:
    virtual ~ATK_ItemState() {}
    virtual void Enter(ATK_Item& atk_item) = 0;
    virtual void Update(ATK_Item& atk_item, float delta_time) = 0;
};

class ATK_ItemStatic : public ATK_ItemState {
public:
    void Enter(ATK_Item& atk_item);
    void Update(ATK_Item& atk_item, float delta_time);
};


class ATK_Item : public Entity {
public:
    std::string name;
    float radius;
    Color color;

    float active_time = 0.0f;

    std::vector<Entity*> *entities;

    ATK_ItemStatic item_stat;

    ATK_Item(std::string id, Vector2 pos, float radius, float dmg, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(ATK_ItemState* new_state);
    void TakeDmg();
    ATK_ItemState* GetState();

private:
    ATK_ItemState* current_state;
};

#endif