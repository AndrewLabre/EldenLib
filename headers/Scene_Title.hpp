#ifndef SCENE_TITLE
#define SCENE_TITLE

#include <raylib.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "Scene_Manager.hpp"
#include "Scene_Settings.hpp"


class TitleScene : public Scene {
public:
    static TitleScene* GetInstance()
    {
        static TitleScene instance;
        return &instance;
    }

    bool GetAudio()
    {
        return setAudio;
    }

    void Begin() override
    {
        InitAudioDevice();
        SetTargetFPS(60);

        animFrames = 0;
        img = LoadImageAnim("whistle.gif", &animFrames);
        bg = LoadTextureFromImage(img);
        nextFrameDataOffset = 0;
        currentAnimFrame = 0;
        frameDelay = 8;
        frameCounter = 0;

        setAudio = Settings::GetInstance()->GetVol();
        std::cout<< "VVVVVVVVVVVVVVVVVVV: " << setAudio<<std::endl;

        bgMusic = LoadMusicStream("../sounds/whistle.mp3");
        SetMusicVolume(bgMusic, 0.5f);
        if (setAudio) {
            SetMusicVolume(bgMusic, 0.5f);
        }
        else
        {
            SetMusicVolume(bgMusic, 0.0f);
        }
        bgMusic.looping = true;
            
        ui_library.root_container.bounds = { 10.0f, 10.0f, 600.0f, 500.0f };

        std::unique_ptr<Button> Start = std::make_unique<Button>();
        Start->text = "START";
        Start->width = 100.0f;
        Start->bounds = { (GetScreenWidth()/2.0f)-50.0f, (GetScreenHeight()/2.0f)+64.0f, 100.0f, 50.0f };
        Start->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(1);
            }
        };
        ui_library.root_container.AddChild(Start.release());

        std::unique_ptr<Button> Settings = std::make_unique<Button>();
        Settings->text = "SETTINGS";
        Settings->width = 120.0f;
        Settings->bounds = { GetScreenWidth()/2.0f-60.0f, (GetScreenHeight()/2.0f+GetScreenHeight()/8.0f)+64.0f, 120.0f, 50.0f };
        Settings->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(3);
            }
        };
        ui_library.root_container.AddChild(Settings.release());

        std::unique_ptr<Button> Exit = std::make_unique<Button>();
        Exit->text = "EXIT";
        Exit->width = 100.0f;
        Exit->bounds = { GetScreenWidth()/2.0f-50.0f, (GetScreenHeight()/2.0f+GetScreenHeight()/8.0f)+139.0f, 100.0f, 50.0f };
        Exit->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(2);
            }
        };
        ui_library.root_container.AddChild(Exit.release());

            // Title = std::make_unique<Label>();
        Title.text = "ENCORE II: BOD";
        Title.color = GRAY;
        float x = ((GetScreenWidth()-MeasureText("ENCORE II: BOD", 64.0f))/2.0f)-2.0f;
        float y = ((GetScreenHeight()/4.0f)-100.0f)+2.0f;
        Title.bounds = { x, y, 500.0f, 50.0f };
        ui_library.root_container.AddChild(&Title);

            // Title2 = std::make_unique<Label>();
        Title2.text = "ENCORE II: BOD";
        Title2.color = BLACK;
        x = ((GetScreenWidth()-MeasureText("ENCORE II: BOD", 64.0f))/2.0f)+2.0f;
        y = ((GetScreenHeight()/4.0f)-100.0f)-2.0f;
        Title2.bounds = {x, y, 100.0f, 50.0f };
        ui_library.root_container.AddChild(&Title2);
    }

    void End() override
    {
            // if (IsMusicStreamPlaying(bgMusic)) StopMusicStream(bgMusic);
            // delete[] &ui_library;
            // Title.reset();
            // Title2.reset();
        UnloadMusicStream(bgMusic);
        CloseAudioDevice();
        UnloadTexture(bg);
        UnloadImage(img);
    }

    void Update() override
    {
        UpdateMusicStream(bgMusic);
        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            currentAnimFrame++;
            if (currentAnimFrame >= animFrames) currentAnimFrame = 0;
            nextFrameDataOffset = img.width*img.height*4*currentAnimFrame;
            UpdateTexture(bg, ((unsigned char *)img.data) + nextFrameDataOffset);
            frameCounter = 0;
        }
        ui_library.Update();
    }

    void Draw() override
    {
        if (CheckCollisionPointRec(GetMousePosition(), Title.bounds) && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        {
            DrawTexture(bg, GetScreenWidth()/2 - bg.width/2, 140, WHITE);
            PlayMusicStream(bgMusic);
        }
        else
        {
            if (IsMusicStreamPlaying(bgMusic)) StopMusicStream(bgMusic);
        }
            // DrawTexture(bg, GetScreenWidth()/2 - bg.width/2, 140, WHITE);
            // PlayMusicStream(bgMusic);
        ui_library.Draw();
    }

private:
    UILibrary ui_library;
    Image img;
    Texture2D bg;
    int animFrames;
    unsigned int nextFrameDataOffset;
    int currentAnimFrame;
    int frameDelay;
    int frameCounter;

    Music bgMusic;

    Label Title;
    Label Title2;

    bool setAudio;

    TitleScene() {}
};

#endif