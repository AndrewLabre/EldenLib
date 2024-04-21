#ifndef SCENE_STAGESELECT
#define SCENE_STAGEsELECT

#include <raylib.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "Scene_Manager.hpp"


class StageSelect : public Scene {
public:
    int level;

    static StageSelect* GetInstance()
    {
        static StageSelect instance;
        return &instance;
    }

    bool GetVol()
    {
        return state;
    }

    void Begin() override
    {
        //InitWindow(800, 600, "Encore!2");
        InitAudioDevice();
        SetTargetFPS(60);

            //state = true;

        bgMusic = LoadMusicStream("sounds/hotmilk.mp3");
        SetMusicVolume(bgMusic, 0.25f);
        bgMusic.looping = true;
        PlayMusicStream(bgMusic);
            
        ui_library.root_container.bounds = { 10.0f, 10.0f, 600.0f, 500.0f };

        std::unique_ptr<Button> Stage1 = std::make_unique<Button>();
        Stage1->text = "MALENIA, BLADE OF MIQUELLA";
        Stage1->width = 750.0f;
        Stage1->bounds = { (GetScreenWidth()/2.0f)-375.0f, (GetScreenHeight()/2.0f), 750.0f, 50.0f };
        Stage1->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(3);
            }
        };
        ui_library.root_container.AddChild(Stage1.release());

        std::unique_ptr<Button> Stage2 = std::make_unique<Button>();
        Stage2->text = "AMOGUS, GYAT OF THE OHIO RIZZLER";
        Stage2->width = 750.0f;
        Stage2->bounds = { (GetScreenWidth()/2.0f)-375.0f, (GetScreenHeight()/2.0f) + 70.0f, 750.0f, 50.0f };
        Stage2->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(4);
            }
        };
        ui_library.root_container.AddChild(Stage2.release());

        std::unique_ptr<Button> Stage3 = std::make_unique<Button>();
        Stage3->text = "MAHORAGA, EIGHT-HANDLED SWORD DIVERGENT SILA DIVINE GENERAL";
        Stage3->width = 750.0f;
        Stage3->bounds = { (GetScreenWidth()/2.0f)-375.0f, (GetScreenHeight()/2.0f) + 140.0f, 750.0f, 50.0f };
        Stage3->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(5);
            }
        };
        ui_library.root_container.AddChild(Stage3.release());

        std::unique_ptr<Button> Back = std::make_unique<Button>();
        Back->text = "BACK";
        Back->width = 100.0f;
        Back->bounds = { (GetScreenWidth()/2.0f)-50, (GetScreenHeight()/2.0f) + 210.0f, 100.0f, 50.0f };
        Back->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(0);
            }
        };
        ui_library.root_container.AddChild(Back.release());

        // Title = std::make_unique<Label>();
        Title.text = "STAGE SELECT";
        Title.color = GRAY;
        float x = ((GetScreenWidth()-MeasureText("STAGE SELECT", 64.0f))/2.0f)-2.0f;
        float y = ((GetScreenHeight()/4.0f)-100.0f)+2.0f;
        Title.bounds = { x, y, 380.0f, 50.0f };
        ui_library.root_container.AddChild(&Title);

        // Title2 = std::make_unique<Label>();
        Title2.text = "STAGE SELECT";
        Title2.color = BLACK;
        x = ((GetScreenWidth()-MeasureText("STAGE SELECT", 64.0f))/2.0f)+2.0f;
        y = ((GetScreenHeight()/4.0f)-100.0f)-2.0f;
        Title2.bounds = {x, y, 100.0f, 50.0f };
        ui_library.root_container.AddChild(&Title2);
    }

    void End() override
    {
            // (IsMusicStreamPlaying(bgMusic)) StopMusicStream(bgMusic);
            // delete[] &ui_library;
            // Title.reset();
            // Title2.reset();
        UnloadMusicStream(bgMusic);
        CloseAudioDevice();
        ResourceManager::GetInstance()->UnloadAllTextures();
    }

    void Update() override
    {
        if (state)
        {
            SetMusicVolume(bgMusic, 0.25f);
        }
        else
        {
            SetMusicVolume(bgMusic, 0.0f);
        }
        UpdateMusicStream(bgMusic);
        ui_library.Update();
    }

    void Draw() override
    {
        ui_library.Draw();
    }

private:
    UILibrary ui_library;

    Music bgMusic;

    Label Title;
    Label Title2;

    bool state;

    StageSelect()
    {
        state = true;
    }
};

#endif