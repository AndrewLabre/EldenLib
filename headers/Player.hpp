#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>

#include "Entity.hpp"


class Player;

class PlayerState {
public:
    virtual ~PlayerState() {}
    virtual void Enter(Player& player) = 0;
    virtual void Update(Player& player, float delta_time) = 0;
};

class PlayerIdle : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerMoving : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerAttacking : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerBlocking : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerDodging : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class Player : public Entity {
public:
    float radius;
    Color color;

    Vector2 velocity;
    float speed;
    float atk_rad;

    float active_time = 0.0f;
    Vector2 direction = Vector2Zero();

    float iFrames = 0.5f;
    float iFrameTimer = 0.0f;

    float dodging_cooldown = 0.0f;

    std::vector<Entity*> *entities;

    PlayerIdle idle;
    PlayerMoving moving;
    PlayerAttacking attacking;
    PlayerBlocking blocking;
    PlayerDodging dodging;

    Player(Vector2 pos, float rad, float spd, float h, float dmg, float atk_r, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(PlayerState* new_state);
    void TakeDmg();
    PlayerState* GetState();

private:
    PlayerState* current_state;
};

#endif