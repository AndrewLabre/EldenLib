#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "../headers/Player.hpp"


// GLOBALS
std::string state_name;

// UTILS
void CheckCollision(Player& player, bool attacking)
{
    for (int i=0; i<player.entities->size(); i++)
    {
        if(player.entities -> at(i) -> entity_type == "Enemy")
        {
            Rectangle enemy = {player.entities -> at(i) -> position.x,
                               player.entities -> at(i) -> position.y,
                               player.entities -> at(i) -> size.x,
                               player.entities -> at(i) -> size.y};
            if(CheckCollisionCircleRec(player.position, player.radius, enemy))
            {
                player.incoming_dmg = player.entities -> at(i) -> basic_dmg;
                player.TakeDmg();
            }

            if(CheckCollisionCircleRec(player.position, player.radius + player.atk_rad, enemy) && attacking)
            {
                player.entities -> at(i) -> TakeDmg();
            }
        }

        if(player.entities -> at(i) -> entity_type == "HP_Boost")
        {
            if(CheckCollisionCircles(player.position, player.radius, player.entities -> at(i) -> position, player.entities -> at(i) -> size.y)) player.hp += player.entities -> at(i) -> hp;
        }

        if(player.entities -> at(i) -> entity_type == "ATK_Boost")
        {
            if(CheckCollisionCircles(player.position, player.radius, player.entities -> at(i) -> position, player.entities -> at(i) -> size.y)) player.basic_dmg += player.entities -> at(i) -> basic_dmg;
        }
    }
}

// PLAYER
void Player::Update(float delta_time) {
    previousPosition = position;

    // std::cout << position.x << " " << position.y << std::endl;
    current_state->Update(*this, delta_time);

    if(iFrameTimer > 0.0f)
    {
        iFrameTimer -= delta_time;
    }

    if(dodging_cooldown > 0.0f)
    {
        dodging_cooldown -= delta_time;
    }
}
void Player::Draw() {
    if(current_state == &attacking)
    {
        DrawCircleV(position, radius + atk_rad, BLUE);
    }

    DrawCircleV(position, radius, color);

    DrawText(state_name.c_str(), position.x - (MeasureText(state_name.c_str(), 20.0f) / 2.0f), position.y + radius + 10.0f, 20.0f, RAYWHITE);

    std::stringstream healthstream;
    healthstream << std::fixed << std::setprecision(2) << hp;
    std::string health = healthstream.str();
    DrawText(health.c_str(), position.x - (MeasureText(health.c_str(), 20.0f) / 2.0f), position.y - radius - 20.0f, 20.0f, GREEN);
}
void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}
void Player::TakeDmg() {
    if(iFrameTimer <= 0.0f)
    {
        if (current_state == &idle || current_state == &moving || current_state == &attacking)
        {
            hp -= incoming_dmg;
            std::cout << "PLAYER HIT" << std::endl;
            iFrameTimer = iFrames;
        }
        if (current_state == &blocking)
        {
            hp -= incoming_dmg/2;
            std::cout << "PLAYER BLOCKED" << std::endl;
            iFrameTimer = iFrames;
        }
    }
}
PlayerState* Player::GetState() {
    return current_state;
}
Player::Player(Vector2 pos, float rad, float spd, float h, float dmg, float atk_r, std::vector<Entity*>* ent) {
    position = pos;
    radius = rad;
    speed = spd;
    hp = h;
    basic_dmg = dmg;
    atk_rad = atk_r;
    entity_type = "Player";
    entities = ent;
    size.x = atk_rad;
    size.y = rad;
    SetState(&idle);
}


// STATES
void PlayerIdle::Enter(Player& player) {
    player.color = GREEN;
    state_name = "IDLE . . .";
}
void PlayerMoving::Enter(Player& player) {
    player.color = YELLOW;
    state_name = "MOVING ~";
}
void PlayerAttacking::Enter(Player& player) {
    player.color = RED;
    player.active_time = 0.0f;

    if(player.entities->size() == 0)
    {
        std::cout << "EMPTY" << std::endl;
    }

    for(int i = 0; i < player.entities -> size(); i++)
    {
        std::cout << "Entity: " << i << " =  " << player.entities -> at(i) -> entity_type << std::endl; 
    }
    state_name = "ATTACKING ! ! !";
}
void PlayerBlocking::Enter(Player& player) {
    player.color = WHITE;
    state_name = "BLOCKING X";
}
void PlayerDodging::Enter(Player& player) {
    player.color = BLUE;
    player.active_time = 0.0f;
    state_name = "DODGING >>";
}


void PlayerIdle::Update(Player& player, float delta_time) {
    CheckCollision(player, false);
    
    player.active_time = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
        player.SetState(&player.moving);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        player.SetState(&player.attacking);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        player.SetState(&player.blocking);
    }
}

void PlayerMoving::Update(Player& player, float delta_time) {
    CheckCollision(player, false);
    player.velocity = Vector2Zero();

    player.direction = Vector2Zero();
    if (IsKeyDown(KEY_W)) player.direction = Vector2Add(player.direction, {0.0f, -1.0f});
    else if (IsKeyDown(KEY_S)) player.direction = Vector2Add(player.direction, {0.0f, 1.0f});
    if (IsKeyDown(KEY_A)) player.direction = Vector2Add(player.direction, {-1.0f, 0.0f});
    else if (IsKeyDown(KEY_D)) player.direction = Vector2Add(player.direction, {1.0f, 0.0f});
    
    player.velocity = Vector2Add(player.velocity, Vector2Scale(player.direction, player.speed * delta_time));
    player.position = Vector2Add(player.position, player.velocity);

    if(Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    }
    if (IsKeyPressed(KEY_SPACE) && player.dodging_cooldown <= 0.0f) {
        player.SetState(&player.dodging);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        player.SetState(&player.attacking);
    }
}

void PlayerAttacking::Update(Player& player, float delta_time) {
    CheckCollision(player, true);
    float atk_duration = 0.25f;
    
    if (player.active_time > atk_duration) {
        player.SetState(&player.idle);
    }
    else {
        player.active_time += delta_time;
    }
}

void PlayerBlocking::Update(Player& player, float delta_time) {
    CheckCollision(player, false);
    if (IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) {
        player.SetState(&player.idle);
    }
}

void PlayerDodging::Update(Player& player, float delta_time) {
    CheckCollision(player, false);
    float dodge_duration = 0.05f;
    float dodge_speed = 4.0f;

    player.dodging_cooldown = 0.5f;
    player.iFrameTimer = player.iFrames;

    if (player.active_time > dodge_duration) {
        player.SetState(&player.idle);
    }
    else {
        player.active_time += delta_time;
        player.velocity = Vector2Add(player.velocity, Vector2Scale(player.direction, dodge_speed * player.speed * delta_time));
        player.position = Vector2Add(player.position, player.velocity);
    }
}