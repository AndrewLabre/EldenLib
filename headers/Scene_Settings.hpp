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
        //InitAudioDevice();
        SetTargetFPS(60);

        animFrames3 = 0;
        img3 = LoadImageAnim("textures/lemao_gif.gif", &animFrames3);
        bg3 = LoadTextureFromImage(img3);
        nextFrameDataOffset3 = 0;
        currentAnimFrame3 = 0;
        frameDelay3 = 8;
        frameCounter3 = 0;

        bgMusic3 = LoadMusicStream("sounds/le_mao.mp3");
        SetMusicVolume(bgMusic3, 1.0f);
        bgMusic3.looping = true;
            
        ui_library.root_container.bounds = { 10.0f, 10.0f, 1080.0f, 620.0f };

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

        Title.text = "SETTINGS";
        Title.color = GRAY;
        float x = ((GetScreenWidth()-MeasureText("SETTINGS", 64.0f))/2.0f)-2.0f;
        float y = ((GetScreenHeight()/4.0f)-100.0f)+2.0f;
        Title.bounds = { x, y, 380.0f, 50.0f };
        ui_library.root_container.AddChild(&Title);

        Title2.text = "SETTINGS";
        Title2.color = BLACK;
        x = ((GetScreenWidth()-MeasureText("SETTINGS", 64.0f))/2.0f)+2.0f;
        y = ((GetScreenHeight()/4.0f)-100.0f)-2.0f;
        Title2.bounds = {x, y, 100.0f, 50.0f };
        ui_library.root_container.AddChild(&Title2);
    }

    void End() override
    {
        UnloadMusicStream(bgMusic3);
        //CloseAudioDevice();
        UnloadTexture(bg3);
        UnloadImage(img3);
        //ResourceManager::GetInstance()->UnloadAllTextures();
    }

    void Update() override
    {
        if (state)
        {
            SetMusicVolume(bgMusic3, 1.0f);
        }
        else
        {
            SetMusicVolume(bgMusic3, 0.0f);
        }
        UpdateMusicStream(bgMusic3);

        frameCounter3++;
        if (frameCounter3 >= frameDelay3)
        {
            currentAnimFrame3++;
            if (currentAnimFrame3 >= animFrames3) currentAnimFrame3 = 0;
            nextFrameDataOffset3 = img3.width*img3.height*4*currentAnimFrame3;
            UpdateTexture(bg3, ((unsigned char *)img3.data) + nextFrameDataOffset3);
            frameCounter3 = 0;
        }

        ui_library.Update();
    }

    void Draw() override
    {
        PlayMusicStream(bgMusic3);
        if (CheckCollisionPointRec(GetMousePosition(), {1270.0f, 710.0f, 10.0f, 10.0f}) && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        {
            DrawTexture(bg3, GetScreenWidth()/2.0f - bg3.width/2.0f, GetScreenHeight()/2.0f - bg3.height/2.0f, WHITE);
        }
        ui_library.Draw();
    }

private:
    UILibrary ui_library;
    Image img3;
    Texture2D bg3;
    int animFrames3;
    unsigned int nextFrameDataOffset3;
    int currentAnimFrame3;
    int frameDelay3;
    int frameCounter3;

    Music bgMusic3;

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