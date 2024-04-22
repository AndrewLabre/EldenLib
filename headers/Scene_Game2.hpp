#ifndef SCENE_GAME2
#define SCENE_GAME2

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <math.h>

class GameScene2 : public Scene {
public:
    void Begin() override
    {
        SetTargetFPS(FPS);
        
        // LEVELS
        std::filesystem::path dirpath = "levels/";

        std::vector<std::string> to_split;
        std::string temp_filename = "";
        if(std::filesystem::exists(dirpath) && std::filesystem::is_directory(dirpath))
        {
            for(const auto& i : std::filesystem::directory_iterator(dirpath))
            {
                temp_filename = i.path().string();
                to_split = StringSplit(temp_filename, "/");
                LEVEL_LIST.push_back(to_split[to_split.size()-1]);
            }
        }
        else
        {
            std::cerr << "Directory not found" << std::endl;
        }

        LevelLoader(LEVEL_NUMBER);

        // DEBUG (Input Check)
        std::cout << "Levels: " << std::endl;
        for(int i=0; i<LEVEL_LIST.size(); i++) {
            std::cout << "level: " << LEVEL_LIST[i] << std::endl;
        }
        std::cout << "Image: " << IMG_NAME << std::endl;
        std::cout << "No. of Tiles: " << NUM_TILES << std::endl;
        for(int i=0; i<TILES.size(); i++) {
            std::cout << "TILE " << i << ": ";
            for(int j=0; j<TILES[i].size(); j++) {
                std::cout << TILES[i][j] << " ";
            }
            std::cout << std::endl;
        }
        for(int i=0; i<GRID.size(); i++) {
            std::cout << "ROW " << i << ": ";
            for(int j=0; j<GRID[i].size(); j++) {
                std::cout << GRID[i][j] << " ";
            }
            std::cout << std::endl;
        }

        // SET TILES
        tile_list = new Tile[(int) NUM_TILES];
        for (int i=0; i<((int) NUM_TILES); i++)
        {
            tile_list[i].loc_in_sheet = {TILES[i][0], TILES[i][1], TILES[i][2], TILES[i][3]};
        }

        // SET CAMERA
        int cam_type;

        float EDGE_X[2], EDGE_Y[2];

        camera_view.target = player1 -> position;
        camera_view.offset = {WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f};
        camera_view.zoom = 1.0f;

        for(int i = 0; i < COLLISION_TILES_INDICES.size(); i++)
        {
            tile_list[COLLISION_TILES_INDICES[i]].has_collision = true;
        }

        // ADD OTHER ENTITIES
        item1 = new HP_Item("HP", {250, 250}, 10.0f, 10.0f, &entities);
        item2 = new ATK_Item("ATK", {450, 250}, 10.0f, 50.0f, &entities);

        entities.push_back(item1);
        entities.push_back(item2);

        // TRANSFORM MAP
        float x_scale = WINDOW_WIDTH / (GRID_SIZE[0]*tile_list[0].loc_in_sheet.width);
        float y_scale = WINDOW_HEIGHT / (GRID_SIZE[1]*tile_list[1].loc_in_sheet.height);
        // float scale = std::min(x_scale, y_scale);
        float scale = 4.0f;

        float x_offset = (WINDOW_WIDTH - (GRID_SIZE[0]*tile_list[0].loc_in_sheet.width*scale)) / 2.0f;
        float y_offset = (WINDOW_HEIGHT - (GRID_SIZE[1]*tile_list[1].loc_in_sheet.height*scale)) / 2.0f;

        // TILES TO GRID
        screen_rectangles = FillGrid(tile_list, x_offset, y_offset, scale);
    }   

    void End() override
    {
        // CLEANING
        UnloadTexture(tilesheet);
        delete[] tile_list;

        for(int i=0; i<entities.size(); i++)
        {
            entities[i]->UnassignTexture();
        }

        for(auto i=entities.begin(); i!=entities.end(); ++i)
        {
            delete *i;
        }
        entities.clear();
        collision_tiles.clear();
    }

    void Update() override 
    {
        float delta_time = GetFrameTime();
        camera_view.target = player1 -> position;
        
        // LOAD SAVEFILE
        if(IsKeyPressed(KEY_V))
        {
            std::filesystem::path dirpath = "saves/";

            std::vector<std::string> to_split;
            std::string filename = "";
            if(std::filesystem::exists(dirpath) && std::filesystem::is_directory(dirpath))
            {
                for(const auto& i : std::filesystem::directory_iterator(dirpath))
                {
                    filename = i.path().string();
                }
            }
            else
            {
                std::cerr << "Directory not found" << std::endl;
            }

            SaveLoader(filename);
        }

        // ITEM SPAWNING
        spawn_timer += delta_time;
        if(spawn_timer >= 10.0f)
        {
            item1 = new HP_Item("HP", {player1->position.x + RandomNumber(-200.0f, 200.0f), player1->position.y + RandomNumber(-200.0f, 200.0f)}, 10.0f, 10.0f, &entities);
            item2 = new ATK_Item("ATK", {player1->position.x + RandomNumber(-500.0f, 500.0f), player1->position.y + RandomNumber(-500.0f, 500.0f)}, 10.0f, 50.0f, &entities);
            entities.push_back(item1);
            entities.push_back(item2);
            spawn_timer = 0.0f;
        }

        // TOGGLE CAMERA BOUNDS
        if(IsKeyPressed(KEY_B))
        {
            if(toggle_bounds) toggle_bounds = false;
            else toggle_bounds = true;
        }

        // SET TO END STATE
        if(player1->hp <= 0)
        {
            game_state = 1;
        }

        // ENEMY STATUS
        enemy_alive = false;
        for(int i=0; i<entities.size(); i++)
        {
            if(entities[i]->hp <= 0.0f)
            {
                entities.erase(entities.begin()+i);
            }

            if(entities[i]->entity_type == "Enemy" && entities[i]->hp > 0.0f)
            {
                enemy_alive = true;
            }
        }
        if(!enemy_alive)
        {
            game_state = 1;
        }

        // CAMERA DRIFT SETTINGS
        // for(int i=0; i<collision_tiles.size(); i++)
        // {
        //     if(!player_colliding)
        //     {
        //         if(camera_view.target.x - player1->position.x + 10 >= cameraBoxDimensions.x/2 || camera_view.target.x - player1->position.x - 10 <= -cameraBoxDimensions.x/2)
        //         {
        //             camera_view.target = Vector2Add(camera_view.target, Vector2Scale(player1->direction, player1->speed * delta_time));
        //         }
        //         if(camera_view.target.y - player1->position.y + 10 >= cameraBoxDimensions.y/2 || camera_view.target.y - player1->position.y - 10 <= -cameraBoxDimensions.y/2)
        //         {
        //             camera_view.target = Vector2Add(camera_view.target, Vector2Scale(player1->direction, player1->speed * delta_time));
        //         }
        //     }
        //     else
        //     {
        //         camera_view.target = Vector2Add(camera_view.target, Vector2Scale(Vector2Normalize(Vector2Subtract(player1->position, camera_view.target)), 10 * delta_time));
        //     }
        // }

        // UPDATE
        for(int i=0; i<entities.size(); i++)
        {
            entities[i]->Update(delta_time);

            for(int j=0; j<collision_tiles.size(); j++)
            {
                CheckTileCollision(collision_tiles[j], entities[i], delta_time, &timer);
            }
        }

        // SET CAMERA DRIFT
        // if(player1->velocity.x == 0 && player1->velocity.y == 0 && Vector2Length(Vector2Subtract(player1->position, camera_view.target)) >= 5.0f)
        // {
        //     camera_view.target = Vector2Add(camera_view.target, Vector2Scale(Vector2Scale(Vector2Normalize(Vector2Subtract(player1->position, camera_view.target)), player1->speed * 0.90), delta_time));
        // }
    }

    void Draw() override
    {
        ClearBackground(BLACK);
        BeginMode2D(camera_view);
            for(int i=0; i<GRID_SIZE[1]; i++)
            {
                for(int j=0; j<GRID_SIZE[0]; j++)
                {
                    int tile_in_grid = (int) GRID[i][j];
                    DrawTexturePro(tilesheet, tile_list[tile_in_grid].loc_in_sheet, screen_rectangles[i][j], {0,0}, 0.0f, WHITE);
                }
            }

            for(int i=0; i<entities.size(); i++)
            {
                entities[i]->Draw();
            }
        EndMode2D();
            
        if(toggle_bounds)
        {
            DrawRectangleLines(WINDOW_WIDTH/2 - cameraBoxDimensions.x/2, WINDOW_HEIGHT/2 - cameraBoxDimensions.y/2, cameraBoxDimensions.x, cameraBoxDimensions.y, WHITE);
        }

        if(game_state == 1)
        {
            std::stringstream healthpoints;
            healthpoints << std::fixed << std::setprecision(2) << player1->hp;
            std::string healthp = healthpoints.str();

            DrawRectangleV({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}, RAYWHITE);
                
            if(!enemy_alive)
            {
                std::string sc = healthp + " / 100";
                DrawText("YOU WIN!", (WINDOW_WIDTH/2 - (MeasureText("YOU WIN!", 100))/2), (WINDOW_HEIGHT/2) - 100, 100, BLUE);
                DrawText(sc.c_str(), (WINDOW_WIDTH/2 - (MeasureText(sc.c_str(), 50))/2), (WINDOW_HEIGHT/2) + 100, 50, BLACK);
            }
            else if(player1->hp <= 0)
            {
                std::string sc = healthp + " / 100";
                DrawText("YOU LOSE", (WINDOW_WIDTH/2 - (MeasureText("YOU LOSE", 100))/2), (WINDOW_HEIGHT/2) - 100, 100, RED);
                DrawText(sc.c_str(), (WINDOW_WIDTH/2 - (MeasureText(sc.c_str(), 50))/2), (WINDOW_HEIGHT/2) + 100, 50, BLACK);
            }
        }
    }

private:
    // STRUCTS
    struct Tile
    {
        Rectangle loc_in_sheet;
        bool has_collision = false;
    };

    // GLOBALS
    const float WINDOW_WIDTH = 1280.0f;
    const float WINDOW_HEIGHT = 720.0f;
    const int FPS = 60;
    const std::string WINDOW_NAME = "Elden Lib";
    const int LEVEL_NUMBER = 2;

    std::string IMG_NAME;
    float NUM_TILES;
    std::vector<std::vector<float>> TILES;
    float GRID_SIZE[2];
    std::vector<std::vector<float>> GRID;
    std::vector<std::string> LEVEL_LIST;
    std::vector<int> COLLISION_TILES_INDICES;

    std::vector<Entity*> entities;
    std::vector<Rectangle> collision_tiles;
    Player* player1;
    std::vector<Enemy*> enemy_list;
    bool enemy_alive = false;
    bool player_colliding = false;

    // LOOP COUNTERS
    int game_state = 0;
    bool toggle_bounds = false;
    float spawn_timer = 0.0f;
    float timer = 0.0f;
    
    Camera2D camera_view = { 0.0f };
    Vector2 cameraBoxDimensions = {300.0f, 300.0f};

    // LOAD TEXTURE
    Texture2D tilesheet;
    
    // ADD OTHER ENTITIES
    HP_Item* item1;
    ATK_Item* item2;

    // SET TILES
    Tile* tile_list;

    // TILES TO GRID
    std::vector<std::vector<Rectangle>> screen_rectangles;


    // UTILS
    float RandomNumber(float Min, float Max)
    {
        return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    }

    std::vector<std::string> StringSplit(std::string s, std::string split_by = " ")
    {
        std::vector<std::string> str;
        std::string element = "";

        for(int i=0; i<s.length(); i++)
        {
            std::string comp;

            if(s[i] != split_by[0])
            {
                element += s[i];
            }
            else
            {
                str.push_back(element);
                element = "";
            }

            if(i == s.length()-1)
            {
                str.push_back(element);
                element = "";
            }
        }

        if(!element.empty())
        {
            str.push_back(element);
        }

        return str;
    }

    void CheckTileCollision(Rectangle rec, Entity* e, float delta_time, float *timer)
    {
        if(e->entity_type == "Player")
        {
            if(CheckCollisionCircleRec(e->position, e->size.y, rec))
            {
                *timer = 0.0f;
                player_colliding = true;
                // Tile edges
                int left = rec.x;
                int right = rec.x + rec.width;
                int top = rec.y;
                int bottom = rec.y + rec.height;

                int skinWidth = 5;

                if(e->previousPosition.x < left)
                {
                    e->position.x = left - e->size.y - skinWidth;
                }
                else if(e->previousPosition.x > right)
                {
                    e->position.x = right + e->size.y + skinWidth;
                }

                else if(e->previousPosition.y < top)
                {
                    e->position.y = top - e->size.y - skinWidth;
                }
                else if(e->previousPosition.y > bottom)
                {
                    e->position.y = bottom + e->size.y + skinWidth;
                }
            }
            else
            {
                if(*timer > 3.0f)
                {
                    player_colliding = false;
                }
                else
                {
                    *timer += delta_time;
                }
            }
        }
        else if(e->entity_type == "Enemy")
        {
            if(CheckCollisionRecs({e->position.x, e->position.y, e->size.x, e->size.y}, rec))
            {
                int left = rec.x;
                int right = rec.x + rec.width;
                int top = rec.y;
                int bottom = rec.y + rec.height;

                int skinWidth = 1;

                if(e->previousPosition.x < left)
                {
                    e->position.x = left - e->size.x - skinWidth;
                }
                else if(e->previousPosition.x > right)
                {
                    e->position.x = right + skinWidth;
                }

                if(e->previousPosition.y < top)
                {
                    e->position.y = top - e->size.y - skinWidth;
                }
                else if(e->previousPosition.y > bottom)
                {
                    e->position.y = bottom + skinWidth;
                }
            }
        }
        else if(e->entity_type == "Item")
        {
            if(CheckCollisionCircleRec(e->position, e->size.y, rec))
            {
                e->hp = 0.0f;
            }
        }
    }

    std::vector<std::vector<Rectangle>> FillGrid(Tile* tile_list, float x_offset, float y_offset, float scale)
    {
        std::vector<std::vector<Rectangle>> screen_recs;

        for(int i=0; i<GRID_SIZE[1]; i++)
        {
            std::vector<Rectangle> row_recs;

            for(int j=0; j<GRID_SIZE[0]; j++)
            {
                int tile_in_grid = (int) GRID[i][j];
                Rectangle destination;

                if(tile_in_grid>=0 && tile_in_grid<NUM_TILES)
                {
                    destination = {x_offset + j*tile_list[tile_in_grid].loc_in_sheet.width * scale,
                                    y_offset + i*tile_list[tile_in_grid].loc_in_sheet.height * scale,
                                    tile_list[tile_in_grid].loc_in_sheet.width * scale,
                                    tile_list[tile_in_grid].loc_in_sheet.height * scale};

                    row_recs.push_back(destination);

                    if(tile_list[tile_in_grid].has_collision)
                    {
                        collision_tiles.push_back(destination);
                    }
                }
            }

            screen_recs.push_back(row_recs);
        }

        return screen_recs;
    }

    // FILE LOADERS
    std::vector<std::vector<Rectangle>> AnimationLoader(std::string animationfile)
    {
        std::string filename = animationfile;
        std::vector<std::vector<Rectangle>> animations;
        std::vector<std::string> to_split;

        std::string animation_count_str, frame_count_str, rectangle_str;
        int animation_count = 0, frame_count = 0;

        std::fstream MyReadFile(filename);
            getline(MyReadFile, animation_count_str);
            to_split = StringSplit(animation_count_str);
            animation_count = std::stoi(to_split[1]);

            for(int i = 0; i < animation_count; i++)
            {
                std::vector<Rectangle> new_animation;

                getline(MyReadFile, frame_count_str);
                to_split = StringSplit(frame_count_str);
                frame_count = std::stoi(to_split[1]);

                for(int j = 0; j < frame_count; j++)
                {
                    Rectangle new_rectangle;
                    getline(MyReadFile, rectangle_str);
                    to_split = StringSplit(rectangle_str);

                    new_rectangle = {std::stof(to_split[0]), std::stof(to_split[1]), std::stof(to_split[2]), std::stof(to_split[3])};

                    new_animation.push_back(new_rectangle);
                }

                animations.push_back(new_animation);
            }

        MyReadFile.close(); 

        return animations;
    }

    void ParseFile(std::string filename)
    {
        std::cout << "PARSING FILE: " << filename << "===================================================" << std::endl;

        std::string img, tile, grid, tile_coords, grid_coords, player_values, enemy_type_string, enemy_values, enemy_texture, collision_tiles_str;
        std::vector<std::string> grid_full, tile_loc, to_split;
        int enemy_type = 0;
        

        std::cout << "WORKS" << std::endl;

        std::fstream MyReadFile;
        MyReadFile.open(filename);
        if(MyReadFile.fail())
        {
            std::cout << "AAAAAAAAAAAAAAAAAAAAA" << std::endl;
        }
        std::cout << "WORKS" << std::endl;
            getline(MyReadFile, img);
            std::cout << "WORKS " << img << std::endl;
            to_split = StringSplit(img);
            std::cout << "WORKS " << to_split.size() << std::endl;
            IMG_NAME = to_split[1];
            std::cout << "WORKS" << std::endl;

            std::cout << "WORKS" << std::endl;
            tilesheet = ResourceLoader();

            std::cout << "WORKS" << std::endl;

            getline(MyReadFile, tile);
            to_split = StringSplit(tile);
            NUM_TILES = std::stof(to_split[1]);
            for (int i=0; i<NUM_TILES; i++)
            {
                getline(MyReadFile, tile_coords);
                tile_loc.push_back(tile_coords);
            }

            getline(MyReadFile, grid);
            to_split = StringSplit(grid);
            GRID_SIZE[0] = std::stof(to_split[1]);
            GRID_SIZE[1] = std::stof(to_split[2]);
            for (int i=0; i<GRID_SIZE[1]; i++)
            {
                getline(MyReadFile, grid_coords);
                grid_full.push_back(grid_coords);
            }

            // COLLISION TILE PARSING
            getline(MyReadFile, collision_tiles_str);
            to_split = StringSplit(collision_tiles_str);
            for(int i = 1; i < to_split.size(); i++)
            {
                COLLISION_TILES_INDICES.push_back(std::stof(to_split[i]));
            }

            getline(MyReadFile, player_values);
            to_split = StringSplit(player_values);

            player1 = new Player({std::stof(to_split[0]), std::stof(to_split[1])}, std::stof(to_split[2]), std::stof(to_split[3]), std::stof(to_split[4]), std::stof(to_split[5]), std::stof(to_split[6]), &entities);
            
            std::cout << "THIS ONE" << std::endl;
            player1 -> AssignTexture("textures/PlayerSheet.png");
            std::cout << "WORKS" << std::endl;
            player1 -> animations = AnimationLoader("animations/player_animations.txt");
            entities.push_back(player1);

            getline(MyReadFile, enemy_type_string);
            to_split = StringSplit(enemy_type_string);
            enemy_type = std::stoi(to_split[1]);

            getline(MyReadFile, enemy_values);
            to_split = StringSplit(enemy_values);

            if(enemy_type == 0)
            {
                Malenia* enemy = new Malenia(to_split[0], {std::stof(to_split[1]), std::stof(to_split[2])}, {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), std::stof(to_split[7]), std::stof(to_split[8]), std::stof(to_split[9]), std::stof(to_split[10]), &entities);

                getline(MyReadFile, enemy_texture);
                to_split = StringSplit(enemy_texture);
                enemy -> AssignTexture("textures/" + to_split[1]);
                std::cout << "WORKS" << std::endl;
                enemy -> animations = AnimationLoader("animations/malenia_animations.txt");

                entities.push_back(enemy);
            }
            else if (enemy_type == 1)
            {
                Amogus* enemy = new Amogus(to_split[0], {std::stof(to_split[1]), std::stof(to_split[2])}, {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), std::stof(to_split[7]), std::stof(to_split[8]), std::stof(to_split[9]), std::stof(to_split[10]), &entities);
                
                getline(MyReadFile, enemy_texture);
                to_split = StringSplit(enemy_texture);
                enemy -> AssignTexture("textures/" + to_split[1]);
                std::cout << "WORKS" << std::endl;
                enemy -> animations = AnimationLoader("animations/amogus_animations.txt");

                entities.push_back(enemy);
            }
            else if (enemy_type == 2)
            {
                Mahoraga* enemy = new Mahoraga(to_split[0], {std::stof(to_split[1]), std::stof(to_split[2])}, {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), std::stof(to_split[7]), std::stof(to_split[8]), std::stof(to_split[9]), std::stof(to_split[10]), &entities);
                
                getline(MyReadFile, enemy_texture);
                to_split = StringSplit(enemy_texture);
                enemy -> AssignTexture("textures/" + to_split[1]);
                std::cout << "WORKS" << std::endl;
                enemy -> animations = AnimationLoader("animations/mahoraga_animations.txt");

                entities.push_back(enemy);
            }
            else
            {
                Enemy* enemy = new Enemy(to_split[0], {std::stof(to_split[1]), std::stof(to_split[2])}, {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), std::stof(to_split[7]), std::stof(to_split[8]), std::stof(to_split[9]), std::stof(to_split[10]), &entities);
                
                getline(MyReadFile, enemy_texture);
                to_split = StringSplit(enemy_texture);
                enemy -> AssignTexture("textures/" + to_split[1]);
                std::cout << "WORKS" << std::endl;
                
                entities.push_back(enemy);
            }

            // for(int i = 0; i < enemy_count; i++)
            // {
            //     getline(MyReadFile, enemy_values);
            //     to_split = StringSplit(enemy_values);
            //     Amogus* enemy = new Amogus(to_split[0], {std::stof(to_split[1]), std::stof(to_split[2])}, {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), std::stof(to_split[7]), std::stof(to_split[8]), std::stof(to_split[9]), std::stof(to_split[10]), &entities);
            //     entities.push_back(enemy);
            //     // enemy_list.push_back(enemy);
            // }

            std::cout << "PARSING ENDDDDD==============" << std::endl;
            
        MyReadFile.close();

        for (int i=0; i<tile_loc.size(); i++)
        {
            std::vector<float> tile_xy;

            to_split = StringSplit(tile_loc[i]);
            for (int j=0; j<to_split.size(); j++)
            {
                tile_xy.push_back(std::stof(to_split[j]));
            }
            TILES.push_back(tile_xy);
        }

        for (int i=0; i<GRID_SIZE[1]; i++)
        {
            std::vector<float> grid_row;

            to_split = StringSplit(grid_full[i]);
            for (int j=0; j<GRID_SIZE[0]; j++)
            {
                grid_row.push_back(std::stof(to_split[j]));
            }
            GRID.push_back(grid_row);
        }
    }

    void LevelLoader(int level = 0)
    {
        std::string filename = "levels/" + LEVEL_LIST[level];
        std::cout << "GOING TO PARSE THIS FILE!" << std::endl;
        ParseFile(filename);
    }

    void SaveLoader(std::string savefile)
    {
        entities.clear();
        std::string filename = savefile;
        std::string entity_count_str, entity_values;
        std::vector<std::string> to_split;
        int entity_count = 0;

        std::fstream MyReadFile(filename);
            getline(MyReadFile, entity_count_str);
            entity_count = std::stoi(entity_count_str);

            for(int i=0; i<entity_count; i++)
            {
                getline(MyReadFile, entity_values);
                to_split = StringSplit(entity_values);
                
                if(to_split[0] == "Player")
                {
                    player1 = new Player({std::stof(to_split[1]), std::stof(to_split[2])}, std::stof(to_split[3]), std::stof(to_split[4]), std::stof(to_split[5]), std::stof(to_split[6]), std::stof(to_split[7]), &entities);
                    entities.push_back(player1);
                }
                if(to_split[0] == "Enemy")
                {
                    Enemy* enemy = new Enemy(to_split[1], {std::stof(to_split[2]), std::stof(to_split[3])}, {std::stof(to_split[4]), std::stof(to_split[5])}, std::stof(to_split[6]), std::stof(to_split[7]), std::stof(to_split[8]), std::stof(to_split[9]), std::stof(to_split[10]), std::stof(to_split[11]), &entities);
                    entities.push_back(enemy);
                }
                if(to_split[0] == "Item")
                {
                    if(to_split[1] == "HP")
                    {
                        HP_Item* item1 = new HP_Item(to_split[2], {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), &entities);
                        entities.push_back(item1);
                    }
                    if(to_split[1] == "ATK")
                    {
                        ATK_Item* item2 = new ATK_Item(to_split[2], {std::stof(to_split[3]), std::stof(to_split[4])}, std::stof(to_split[5]), std::stof(to_split[6]), &entities);
                        entities.push_back(item2);
                    }
                }
            }        
        MyReadFile.close();
    }

    Texture2D ResourceLoader()
    {
        std::string filename = "textures/" + IMG_NAME;
        std::cout << "WORKS" << std::endl;
        Texture2D tilesheet = LoadTexture(filename.c_str());
        
        return tilesheet;
    }

    // FILE MODIFIERS
    void SaveProgress(int n_entity)
    {
        std::cout << "CLOSE" << std::endl;
        std::ofstream outfile("/saves/save.txt");
        std::string entity_data = std::to_string(n_entity) + "\n";

        for(int i=0; i<entities.size(); i++)
        {
            if(entities[i]->entity_type == "Player")
            {
                entity_data += entities[i]->entity_type + " "
                            + std::to_string(player1->position.x) + " "
                            + std::to_string(player1->position.y) + " "
                            + std::to_string(player1->radius) + " "
                            + std::to_string(player1->speed) + " "
                            + std::to_string(player1->hp) + " "
                            + std::to_string(player1->basic_dmg) + " "
                            + std::to_string(player1->atk_rad) + "\n";
            }
            if(entities[i]->entity_type == "Enemy")
            {
                // entity_data += entities[i]->entity_type + " "
                //             + enemy_list[i-1]->name + " "
                //             + std::to_string(enemy_list[i-1]->position.x) + " "
                //             + std::to_string(enemy_list[i-1]->position.y) + " "
                //             + std::to_string(enemy_list[i-1]->size.x) + " "
                //             + std::to_string(enemy_list[i-1]->size.y) + " "
                //             + std::to_string(enemy_list[i-1]->speed) + " "
                //             + std::to_string(enemy_list[i-1]->hp) + " "
                //             + std::to_string(enemy_list[i-1]->basic_dmg) + " "
                //             + std::to_string(enemy_list[i-1]->detection_rad) + " "
                //             + std::to_string(enemy_list[i-1]->aggro_rad) + " "
                //             + std::to_string(enemy_list[i-1]->attack_rad) + "\n";
            }
            if(entities[i]->entity_type == "Item")
            {
                std::string name = (entities[i]->basic_dmg == 10.0f) ? "HP" : "ATK";
                
                entity_data += entities[i]->entity_type + " "
                            + name + " " 
                            + std::to_string(entities[i]->position.x) + " "
                            + std::to_string(entities[i]->position.y) + " "
                            + std::to_string(entities[i]->size.y) + " "
                            + std::to_string(entities[i]->hp) + " "
                            + std::to_string(entities[i]->basic_dmg) + "\n";
            }
        }

        outfile << entity_data;
        outfile.close();
    }
};

#endif