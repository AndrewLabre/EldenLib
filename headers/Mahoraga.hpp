#ifndef MAHORAGA
#define MAHORAGA

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Entity.hpp"

class Mahoraga;

class MahoragaState {
public:
    virtual ~MahoragaState() {}
    virtual void Enter(Mahoraga& enemy) = 0;
    virtual void Update(Mahoraga& enemy, float delta_time) = 0;
};

class MahoragaIdle : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaWandering : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaChasing : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaPhase1Readying : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaPhase1Attacking : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaPhase2Readying : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaPhase2Attacking : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaPhase3Readying : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaPhase3Attacking : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaBlocking : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaAdapting : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class MahoragaTeleporting : public MahoragaState {
public:
    void Enter(Mahoraga& enemy);
    void Update(Mahoraga& enemy, float delta_time);
};

class Mahoraga : public Entity {
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

    float shieldTimer = 0.0f;

    std::vector<Entity*> *entities;

    MahoragaIdle idle;
    MahoragaWandering wandering;
    MahoragaChasing chasing;
    MahoragaBlocking blocking;
    MahoragaAdapting adapting;
    MahoragaTeleporting teleporting;

    //Mahoraga attacks
    MahoragaPhase1Readying readying1;
    MahoragaPhase2Readying readying2;
    MahoragaPhase3Readying readying3;
    MahoragaPhase1Attacking attacking1;
    MahoragaPhase2Attacking attacking2;
    MahoragaPhase3Attacking attacking3;
    

    //Mahoraga Unique attacks
    std::vector<Vector2> attackPath;
    float burst_rad;
    float shield;

    Mahoraga(std::string id, Vector2 pos, Vector2 size, float spd, float hp, float basic_dmg, float det, float agg, float atk, std::vector<Entity*>* ent);

    void Update(float delta_time);
    void Draw();
    void SetState(MahoragaState* new_state);
    void TakeDmg();
    MahoragaState* GetState();

    float RandomNumber(float Min, float Max)
    {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }

    int RandomInt(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

private:
    MahoragaState* current_state;
};

#endif