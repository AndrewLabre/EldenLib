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
    bool exit_scene = false;

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
        // InitAudioDevice();
        SetTargetFPS(60);

        animFrames = 0;
        img = LoadImageAnim("textures/ledarksun_gif.gif", &animFrames);
        bg = LoadTextureFromImage(img);
        nextFrameDataOffset = 0;
        currentAnimFrame = 0;
        frameDelay = 8;
        frameCounter = 0;

        animFrames2 = 0;
        img2 = LoadImageAnim("textures/lesun_gif.gif", &animFrames2);
        bg2 = LoadTextureFromImage(img2);
        nextFrameDataOffset2 = 0;
        currentAnimFrame2 = 0;
        frameDelay2 = 8;
        frameCounter2 = 0;

        setAudio = Settings::GetInstance()->GetVol();
        std::cout<< "VVVVVVVVVVVVVVVVVVV: " << setAudio<<std::endl;

        bgMusic = LoadMusicStream("sounds/le_darksun.mp3");
        SetMusicVolume(bgMusic, 2.0f);
        if (setAudio) {
            SetMusicVolume(bgMusic, 2.0f);
        }
        else
        {
            SetMusicVolume(bgMusic, 0.0f);
        }
        bgMusic.looping = true;

        setAudio2 = Settings::GetInstance()->GetVol();
        std::cout<< "VVVVVVVVVVVVVVVVVVV: " << setAudio2<<std::endl;

        bgMusic2 = LoadMusicStream("sounds/le_sun.mp3");
        SetMusicVolume(bgMusic2, 1.0f);
        if (setAudio2) {
            SetMusicVolume(bgMusic2, 1.0f);
        }
        else
        {
            SetMusicVolume(bgMusic2, 0.0f);
        }
        bgMusic2.looping = true;
            
        ui_library.root_container.bounds = { 10.0f, 10.0f, 1080.0f, 620.0f };

        std::unique_ptr<Button> Start = std::make_unique<Button>();
        Start->text = "START";
        Start->width = 100.0f;
        Start->bounds = { (GetScreenWidth()/2.0f)-50.0f, (GetScreenHeight()/2.0f)+64.0f, 100.0f, 50.0f };
        Start->customClickHandler = [this]()
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(2);
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
                GetSceneManager()->SwitchScene(1);
            }
        };
        ui_library.root_container.AddChild(Settings.release());

        std::unique_ptr<Button> Exit = std::make_unique<Button>();
        Exit->text = "EXIT";
        Exit->width = 100.0f;
        Exit->bounds = { GetScreenWidth()/2.0f-50.0f, (GetScreenHeight()/2.0f+GetScreenHeight()/8.0f)+139.0f, 100.0f, 50.0f };
        Exit->customClickHandler = [this]()
        {
            exit_scene = true;
        };
        ui_library.root_container.AddChild(Exit.release());

        Title.text = "ELDEN LIB";
        Title.color = GRAY;
        float x = ((GetScreenWidth()-MeasureText("ELDEN LIB", 64.0f))/2.0f)-2.0f;
        float y = ((GetScreenHeight()/4.0f)-100.0f)+2.0f;
        Title.bounds = { x, y, 500.0f, 50.0f };
        ui_library.root_container.AddChild(&Title);

        Title2.text = "ELDEN LIB";
        Title2.color = BLACK;
        x = ((GetScreenWidth()-MeasureText("ELDEN LIB", 64.0f))/2.0f)+2.0f;
        y = ((GetScreenHeight()/4.0f)-100.0f)-2.0f;
        Title2.bounds = {x, y, 100.0f, 50.0f };
        ui_library.root_container.AddChild(&Title2);
    }

    void End() override
    {
        UnloadMusicStream(bgMusic);
        UnloadMusicStream(bgMusic2);
        // CloseAudioDevice();
        UnloadTexture(bg);
        UnloadTexture(bg2);
        UnloadImage(img);
        UnloadImage(img2);
        // CloseWindow();
    }

    void Update() override
    {
        UpdateMusicStream(bgMusic);
        UpdateMusicStream(bgMusic2);

        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            currentAnimFrame++;
            if (currentAnimFrame >= animFrames) currentAnimFrame = 0;
            nextFrameDataOffset = img.width*img.height*4*currentAnimFrame;
            UpdateTexture(bg, ((unsigned char *)img.data) + nextFrameDataOffset);
            frameCounter = 0;
        }

        frameCounter2++;
        if (frameCounter2 >= frameDelay2)
        {
            currentAnimFrame2++;
            if (currentAnimFrame2 >= animFrames2) currentAnimFrame2 = 0;
            nextFrameDataOffset2 = img2.width*img2.height*4*currentAnimFrame2;
            UpdateTexture(bg2, ((unsigned char *)img2.data) + nextFrameDataOffset2);
            frameCounter2 = 0;
        }

        ui_library.Update();
    }

    void Draw() override
    {
        if (CheckCollisionPointRec(GetMousePosition(), Title.bounds) && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        {
            if (IsMusicStreamPlaying(bgMusic2)) StopMusicStream(bgMusic2);
            DrawTexture(bg, GetScreenWidth()/2.0f - bg.width/2.0f, GetScreenHeight()/2.0f - bg.height/2.0f, WHITE);
            PlayMusicStream(bgMusic);
        }
        else
        {
            if (IsMusicStreamPlaying(bgMusic)) StopMusicStream(bgMusic);
            DrawTexture(bg2, GetScreenWidth()/2.0f - bg.width/2.0f, GetScreenHeight()/2.0f - bg.height/2.0f, WHITE);
            PlayMusicStream(bgMusic2);
        }
        ui_library.Draw();
    }

private:
    UILibrary ui_library;
    Image img, img2;
    Texture2D bg, bg2;
    int animFrames, animFrames2;
    unsigned int nextFrameDataOffset, nextFrameDataOffset2;
    int currentAnimFrame, currentAnimFrame2;
    int frameDelay, frameDelay2;
    int frameCounter, frameCounter2;

    Music bgMusic, bgMusic2;

    Label Title;
    Label Title2;

    bool setAudio, setAudio2;

    TitleScene() {}
};

#endif