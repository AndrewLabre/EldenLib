#ifndef ENEMY
#define ENEMY

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Entity.hpp"

class Enemy;

class EnemyState {
public:
    virtual ~EnemyState() {}
    virtual void Enter(Enemy& enemy) = 0;
    virtual void Update(Enemy& enemy, float delta_time) = 0;
};

class EnemyWandering : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyChasing : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyReadying : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyAttacking : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class Enemy : public Entity {
public:
    std::string name;
    Color color;

    Vector2 velocity;
    float speed;

    float detection_rad;
    float aggro_rad;
    float attack_rad;

    float active_time = 0.0f;
    Vector2 direction = Vector2Zero();

    float iFrames = 0.5f;
    float iFrameTimer = 0.0f;

    float wandering_timer = 0.0f;

    std::vector<Entity*> *entities;

    EnemyWandering wandering;
    EnemyChasing chasing;

    //Enemy attacks
    EnemyReadying readying;
    EnemyAttacking attacking;

    Enemy(std::string id, Vector2 pos, Vector2 size, float spd, float hp, float basic_dmg, float det, float agg, float atk, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(EnemyState* new_state);
    void TakeDmg();
    EnemyState* GetState();

    float RandomNumber(float Min, float Max)
    {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }

    int RandomInt(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

private:
    EnemyState* current_state;
};

#endif