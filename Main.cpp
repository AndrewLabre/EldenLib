/******************************************************************************
 * Homework 4 - Level Design
 * Antoporda, Banal, Labre 2024
 *
 * 
 * W, A, S, D for player movement
 * Left click for attack, Right click for block
 * Space to Dodge
 * 
 * Press ESC to save before exit
 * V to load savefile
 * 
 *
 * Outputs level specified in the levels/ directory
 * Tilesheet in textures/
 * 
 *****************************************************************************/

// g++ Main.cpp -o EldenLib -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm

#include <raylib.h>

#include "headers/Scene_Manager.hpp"
#include "headers/Scene_Title.hpp"
#include "headers/Scene_Settings.hpp"
#include "headers/Scene_Game.hpp"
#include "headers/Scene_Game1.hpp"
#include "headers/Scene_Game2.hpp"
#include "headers/Scene_StageSelect.hpp"


// GLOBALS
const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const int FPS(60);
const std::string WINDOW_NAME("Elden Lib");

int main() {
    // SET WINDOW
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME.c_str());
    SetTargetFPS(FPS);
    
    SceneManager scene_manager;

    TitleScene* title_scene = TitleScene::GetInstance();
    title_scene->SetSceneManager(&scene_manager);

    Settings* settings = Settings::GetInstance();
    settings->SetSceneManager(&scene_manager);

    GameScene game_scene;
    game_scene.SetSceneManager(&scene_manager);

    GameScene1 game_scene1;
    game_scene1.SetSceneManager(&scene_manager);

    GameScene2 game_scene2;
    game_scene2.SetSceneManager(&scene_manager);

    StageSelect* stage_select = StageSelect::GetInstance();
    stage_select->SetSceneManager(&scene_manager);

    scene_manager.RegisterScene(title_scene, 0);
    scene_manager.RegisterScene(settings, 1);
    scene_manager.RegisterScene(stage_select, 2);
    scene_manager.RegisterScene(&game_scene, 3);
    scene_manager.RegisterScene(&game_scene1, 4);
    scene_manager.RegisterScene(&game_scene2, 5);

    scene_manager.SwitchScene(0);

    while(!WindowShouldClose())
    {
        Scene* active_scene = scene_manager.GetActiveScene();

        BeginDrawing();
            ClearBackground(WHITE);

            if (active_scene != nullptr)
            {
                active_scene->Update();
                active_scene->Draw();
                if (active_scene == title_scene)
                {
                    if(title_scene->exit_scene)
                    {
                        break;
                    }
                }
            }
        EndDrawing();
    }

    // // SAVE
    // if((IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) && game_state == 0)
    // {
    //     SaveProgress(entities.size());
    // }

    // CLEANING
    Scene* active_scene = scene_manager.GetActiveScene();
    if (active_scene != nullptr)
    {
        active_scene->End();
    }

    scene_manager.UnregisterScene(0);
    scene_manager.UnregisterScene(1);
    scene_manager.UnregisterScene(2);
    scene_manager.UnregisterScene(3);
    scene_manager.UnregisterScene(4);
    scene_manager.UnregisterScene(5);

    ResourceManager::GetInstance()->UnloadAllTextures();

    CloseWindow();
    return 0;
}