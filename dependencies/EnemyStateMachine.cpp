#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iomanip>
#include <sstream>

#include "../headers/Enemy.hpp"
#include "../headers/BezierAttack.hpp"


// UTILS
float DegreesToRadians(float degrees)
{
    return degrees * (PI / 180);
}

float RadiansToDegrees(float radians)
{
    return radians * (180 / PI);
}

void CheckCollision(Enemy& enemy)
{
    for (int i=0; i<enemy.entities->size(); i++)
    {
        if(enemy.entities -> at(i) -> entity_type == "Enemy")
        {
            Rectangle enemy_rec_a = {enemy.position.x,
                                    enemy.position.y,
                                    enemy.size.x,
                                    enemy.size.y};
            Rectangle enemy_rec_b = {enemy.entities -> at(i) -> position.x,
                                    enemy.entities -> at(i) -> position.y,
                                    enemy.entities -> at(i) -> size.x,
                                    enemy.entities -> at(i) -> size.y};
            if(CheckCollisionRecs(enemy_rec_a, enemy_rec_b))
            {
               enemy.direction = Vector2Scale(Vector2Normalize(Vector2Subtract(enemy.entities -> at(i) -> position, Vector2Add(enemy.position,{enemy.size.x/2, enemy.size.y/2}))), -1.0f);
            }
        }

        if(enemy.entities -> at(i) -> entity_type == "HP_Boost")
        {
            Rectangle enemy_rec_a = {enemy.position.x,
                                    enemy.position.y,
                                    enemy.size.x,
                                    enemy.size.y};
            if(CheckCollisionCircleRec(enemy.entities -> at(i) -> position, enemy.entities -> at(i) -> size.y, enemy_rec_a))
            {
                enemy.hp += enemy.entities -> at(i) -> hp;
            }
        }

        if(enemy.entities -> at(i) -> entity_type == "ATK_Boost")
        {
            Rectangle enemy_rec_a = {enemy.position.x,
                                    enemy.position.y,
                                    enemy.size.x,
                                    enemy.size.y};
            if(CheckCollisionCircleRec(enemy.entities -> at(i) -> position, enemy.entities -> at(i) -> size.y, enemy_rec_a))
            {
                enemy.basic_dmg += enemy.entities -> at(i) -> basic_dmg;
            }
        }
    }
}


// ENEMY
void Enemy::Update(float delta_time) {
    previousPosition = position;
    
    current_state->Update(*this, delta_time);

    if(iFrameTimer > 0.0f)
    {
        iFrameTimer -= delta_time;
    }

    incoming_dmg = entities -> at(0) -> basic_dmg;

    if (GetState() == &readying)
    {
        color.r += 100*delta_time;
        color.g += 100*delta_time;
        color.b += 100*delta_time;
    }
}
void Enemy::Draw() {
    if(CheckCollisionPointCircle(entities -> at(0) -> position, {position.x + size.x/2, position.y + size.y/2}, detection_rad))
    {
        DrawRectanglePro({position.x+size.x/2, position.y+size.y/2, size.x, size.y}, {size.x/2, size.y/2}, RadiansToDegrees(Vector2Angle(direction, Vector2Subtract({entities->at(0)->position}, {position.x+size.x/2, position.y+size.y/2}))), RED);
    }
    else
    {
        DrawRectangleV(position, size, color);
    }
    DrawCircleLines(position.x + size.x/2, position.y + size.y/2, detection_rad, ORANGE);
    DrawCircleLines(position.x + size.x/2, position.y + size.y/2, aggro_rad, RED);
    DrawCircleLines(position.x + size.x/2, position.y + size.y/2, attack_rad, YELLOW);

    DrawText(name.c_str(), position.x - (MeasureText(name.c_str(), 20.0f) / 2.0f), position.y + size.y + 10.0f, 20.0f, RED);

    std::stringstream enemy_healthstream;
    enemy_healthstream << std::fixed << std::setprecision(2) << hp;
    std::string enemy_health = enemy_healthstream.str();
    DrawText(enemy_health.c_str(), position.x - (MeasureText(enemy_health.c_str(), 20.0f) / 2.0f), position.y - 20.0f, 20.0f, RED);
}
void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}
EnemyState* Enemy::GetState() {
    return current_state;
}
void Enemy::TakeDmg() {
    if(iFrameTimer <= 0.0f)
    {
        hp -= incoming_dmg;
        iFrameTimer = iFrames;
    }
}
Enemy::Enemy(std::string i, Vector2 pos, Vector2 s, float spd, float h, float dmg, float det, float agg, float atk, std::vector<Entity*>* ent) {
    nme = i;
    name = i;
    position = pos;
    size = s;
    speed = spd;
    hp = h;
    basic_dmg = dmg;
    detection_rad = det;
    aggro_rad = agg;
    attack_rad = atk;
    entities = ent;
    entity_type = "Enemy";
    SetState(&wandering);
}


// STATES
void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = SKYBLUE;
    enemy.active_time = 0.0f;
    enemy.wandering_timer = enemy.RandomNumber(2.0f, 4.0f);
    enemy.direction = Vector2Normalize({enemy.RandomNumber(-1.0f, 1.0f), enemy.RandomNumber(-1.0f, 1.0f)});
}
void EnemyChasing::Enter(Enemy& enemy) {
    enemy.color = RED;
    enemy.active_time = 0.0f;
}
void EnemyReadying::Enter(Enemy& enemy) {
    enemy.color = RED;
    enemy.active_time = 0.0f;
}
void EnemyAttacking::Enter(Enemy& enemy) {
    enemy.color = VIOLET;
    enemy.active_time = 0.0f;
}

void EnemyWandering::Update(Enemy& enemy, float delta_time) {

    for(int i = 0; i < enemy.entities -> size(); i++)
    {
        if(enemy.entities -> at(i) -> entity_type == "Player")
        {
            if(CheckCollisionPointCircle(enemy.entities -> at(i) -> position, {enemy.position.x + enemy.size.x/2, enemy.position.y + enemy.size.y/2}, enemy.aggro_rad))
            {
                enemy.SetState(&enemy.chasing);
            }
        }
    }

    enemy.velocity = Vector2Zero();

    enemy.velocity = Vector2Add(enemy.velocity, Vector2Scale(enemy.direction, enemy.speed * delta_time));
    enemy.position = Vector2Add(enemy.position, enemy.velocity);

    enemy.active_time += delta_time;

    if(enemy.active_time >= enemy.wandering_timer)
    {
        enemy.SetState(&enemy.wandering);
    }
}

void EnemyChasing::Update(Enemy& enemy, float delta_time) {
    enemy.velocity = Vector2Zero();
    float chase_spd = enemy.speed * 1.5f;
    float local_spd = enemy.speed;

    for(int i = 0; i < enemy.entities -> size(); i++)
    {
        if(enemy.entities -> at(i) -> entity_type == "Player")
        {
            enemy.direction = Vector2Normalize(Vector2Subtract(enemy.entities -> at(i) -> position, Vector2Add(enemy.position,{enemy.size.x/2, enemy.size.y/2})));
 
            if(!CheckCollisionPointCircle(enemy.entities -> at(i) -> position, {enemy.position.x + enemy.size.x/2, enemy.position.y + enemy.size.y/2}, enemy.aggro_rad))
            {
                enemy.SetState(&enemy.wandering);
            }

            if(CheckCollisionPointCircle(enemy.entities -> at(i) -> position, {enemy.position.x + enemy.size.x/2, enemy.position.y + enemy.size.y/2}, enemy.attack_rad))
            {
                enemy.SetState(&enemy.readying);
            }

            if(CheckCollisionPointCircle(enemy.entities -> at(i) -> position, {enemy.position.x + enemy.size.x/2, enemy.position.y + enemy.size.y/2}, enemy.detection_rad))
            {
                local_spd = chase_spd;
            }
        }
    }

    enemy.velocity = Vector2Add(enemy.velocity, Vector2Scale(enemy.direction, local_spd * delta_time * 1.5f));
    enemy.position = Vector2Add(enemy.position, enemy.velocity);
}

void EnemyReadying::Update(Enemy& enemy, float delta_time) {
    float readying_duration = 0.5f;
    
    if (enemy.active_time > readying_duration) {
        enemy.color = WHITE;
        for(int i = 0; i < enemy.entities -> size(); i++)
        {
            if(enemy.entities -> at(i) -> entity_type == "Player")
            {
                enemy.direction = Vector2Normalize(Vector2Subtract(enemy.entities -> at(i) -> position, Vector2Add(enemy.position,{enemy.size.x/2, enemy.size.y/2})));
            }
        }
        enemy.SetState(&enemy.attacking);
    }
    else {
        enemy.active_time += delta_time;
    }
}

void EnemyAttacking::Update(Enemy& enemy, float delta_time) {
    float dodge_duration = 0.15f;
    float dodge_speed = 2.0f;

    float atk_duration = 0.2f;
    
    if (enemy.active_time > atk_duration) {
        enemy.SetState(&enemy.wandering);
    }
    else {
        enemy.active_time += delta_time;
        enemy.velocity = Vector2Add(enemy.velocity, Vector2Scale(enemy.direction, dodge_speed * enemy.speed * delta_time));
        enemy.position = Vector2Add(enemy.position, enemy.velocity);
    }
}