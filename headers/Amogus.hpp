#ifndef AMOGUS
#define AMOGUS

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Entity.hpp"

class Amogus;

class AmogusState {
public:
    virtual ~AmogusState() {}
    virtual void Enter(Amogus& enemy) = 0;
    virtual void Update(Amogus& enemy, float delta_time) = 0;
};

class AmogusWandering : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class AmogusChasing : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class AmogusReadying : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class AmogusAttacking : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class AmogusChasing2 : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class AmogusReadying2 : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class AmogusAttacking2 : public AmogusState {
public:
    void Enter(Amogus& enemy);
    void Update(Amogus& enemy, float delta_time);
};

class Amogus : public Entity {
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

    AmogusWandering wandering;
    AmogusChasing chasing;

    //Amogus attacks
    AmogusReadying readying;
    AmogusAttacking attacking;

    //Amogus Unique States
    AmogusChasing2 chasing2;
    // AmogusReadying2 readying2;
    // AmogusAttacking2 attacking2;
    

    // //Amogus Unique attack
    std::vector<Vector2> attackPath;
    int flurryCounter;

    Amogus(std::string id, Vector2 pos, Vector2 size, float spd, float hp, float basic_dmg, float det, float agg, float atk, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(AmogusState* new_state);
    void TakeDmg();
    AmogusState* GetState();

    float RandomNumber(float Min, float Max)
    {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }

    int RandomInt(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

private:
    AmogusState* current_state;
};

#endif