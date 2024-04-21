#ifndef SCENE_SETTINGS
#define SCENE_SETTINGS

#include <raylib.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "Scene_Manager.hpp"


class Settings : public Scene {
public:
    static Settings* GetInstance()
    {
        static Settings instance;
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

        std::cout<< "VVVVVVVVVVVVVVVVVVV: " << state<<std::endl;

            //state = true;

        bg = ResourceManager::GetInstance()->GetTexture("omni2.png");

        bgMusic = LoadMusicStream("../sounds/hotmilk.mp3");
        SetMusicVolume(bgMusic, 0.25f);
        bgMusic.looping = true;
        PlayMusicStream(bgMusic);
            
        ui_library.root_container.bounds = { 10.0f, 10.0f, 600.0f, 500.0f };

        SetAudio.text = "Audio ON";
        SetAudio.alttext = "Audio OFF";
        SetAudio.color = BLACK;
        SetAudio.bounds = { (GetScreenWidth()/2.0f)-25.0f, GetScreenHeight()/2.0f, 50.0f, 50.0f };
        SetAudio.customClickHandler = [this]()
        {
            if (state)
            {
                state = false;
            }
            else
            {
                state = true;
            }
            std::cout << SetAudio.Name() << std::endl;
        };
        ui_library.root_container.AddChild(&SetAudio);

        std::unique_ptr<Button> Back = std::make_unique<Button>();
        Back->text = "BACK";
        Back->width = 100.0f;
        Back->bounds = { (GetScreenWidth()/2.0f)-50, (GetScreenHeight()/2.0f+GetScreenHeight()/8.0f)+64.0f, 100.0f, 50.0f };
        Back->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(0);
            }
        };
        ui_library.root_container.AddChild(Back.release());

        // Title = std::make_unique<Label>();
        Title.text = "SETTINGS";
        Title.color = GRAY;
        float x = ((GetScreenWidth()-MeasureText("SETTINGS", 64.0f))/2.0f)-2.0f;
        float y = ((GetScreenHeight()/4.0f)-100.0f)+2.0f;
        Title.bounds = { x, y, 380.0f, 50.0f };
        ui_library.root_container.AddChild(&Title);

        // Title2 = std::make_unique<Label>();
        Title2.text = "SETTINGS";
        Title2.color = BLACK;
        x = ((GetScreenWidth()-MeasureText("SETTINGS", 64.0f))/2.0f)+2.0f;
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
        if (CheckCollisionPointRec(GetMousePosition(), Title.bounds) && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        {
            DrawTexturePro(bg, {0.0f,0.0f,676.0f,463.0f}, {0.0f,0.0f,800.0f,600.0f}, {0.0f, 0.0f}, 0.0f, WHITE);
        }
        ui_library.Draw();
    }

private:
    UILibrary ui_library;
    Texture bg;

    Music bgMusic;

    Label Title;
    Label Title2;

    CheckBox SetAudio;
    bool state;

    Settings()
    {
        state = true;
    }
};

#endif