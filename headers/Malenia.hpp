#ifndef MALENIA
#define MALENIA

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Entity.hpp"

class Malenia;

class MaleniaState {
public:
    virtual ~MaleniaState() {}
    virtual void Enter(Malenia& enemy) = 0;
    virtual void Update(Malenia& enemy, float delta_time) = 0;
};

class MaleniaWandering : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaChasing : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaReadying : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaAttacking : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaReadying2 : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaAttacking2 : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaReadying3 : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class MaleniaAttacking3 : public MaleniaState {
public:
    void Enter(Malenia& enemy);
    void Update(Malenia& enemy, float delta_time);
};

class Malenia : public Entity {
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

    MaleniaWandering wandering;
    MaleniaChasing chasing;

    //Malenia attacks
    MaleniaReadying readying;
    MaleniaAttacking attacking;

    //Malenia Unique States
    MaleniaReadying2 readying2;
    MaleniaAttacking2 attacking2;

    MaleniaReadying3 readying3;
    MaleniaAttacking3 attacking3;
    

    // //Malenia Unique attack
    std::vector<Vector2> attackPath;
    int flurryCounter;

    Malenia(std::string id, Vector2 pos, Vector2 size, float spd, float hp, float basic_dmg, float det, float agg, float atk, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(MaleniaState* new_state);
    void TakeDmg();
    MaleniaState* GetState();

    float RandomNumber(float Min, float Max)
    {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }

    int RandomInt(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

private:
    MaleniaState* current_state;
};

#endif