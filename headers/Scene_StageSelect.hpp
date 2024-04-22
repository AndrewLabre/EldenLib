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

    bool GetAudio()
    {
        return setAudio4;
    }

    static StageSelect* GetInstance()
    {
        static StageSelect instance;
        return &instance;
    }

    void Begin() override
    {
        //InitAudioDevice();
        SetTargetFPS(60);
        
        animFrames4 = 0;
        img4 = LoadImageAnim("textures/lebomboclat_gif.gif", &animFrames4);
        bg4 = LoadTextureFromImage(img4);
        nextFrameDataOffset4 = 0;
        currentAnimFrame4 = 0;
        frameDelay4 = 8;
        frameCounter4 = 0;

        setAudio4 = Settings::GetInstance()->GetVol();

        bgMusic4 = LoadMusicStream("sounds/le_bomboclat.mp3");
        SetMusicVolume(bgMusic4, 1.0f);
        if (setAudio4) {
            SetMusicVolume(bgMusic4, 1.0f);
        }
        else
        {
            SetMusicVolume(bgMusic4, 0.0f);
        }
        bgMusic4.looping = true;
            
        ui_library.root_container.bounds = { 10.0f, 10.0f, 1080.0f, 620.0f };

        std::unique_ptr<Button> Stage1 = std::make_unique<Button>();
        Stage1->text = "MALENIA, BLADE OF MIQUELLA";
        Stage1->width = 756.0f;
        Stage1->bounds = { (GetScreenWidth()/2.0f)-378.0f, (GetScreenHeight()/2.0f), 756.0f, 50.0f };
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
        Stage2->width = 756.0f;
        Stage2->bounds = { (GetScreenWidth()/2.0f)-378.0f, (GetScreenHeight()/2.0f) + 70.0f, 756.0f, 50.0f };
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
        Stage3->width = 756.0f;
        Stage3->bounds = { (GetScreenWidth()/2.0f)-378.0f, (GetScreenHeight()/2.0f) + 140.0f, 756.0f, 50.0f };
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

        Title.text = "STAGE SELECT";
        Title.color = GRAY;
        float x = ((GetScreenWidth()-MeasureText("STAGE SELECT", 64.0f))/2.0f)-2.0f;
        float y = ((GetScreenHeight()/4.0f)-100.0f)+2.0f;
        Title.bounds = { x, y, 380.0f, 50.0f };
        ui_library.root_container.AddChild(&Title);

        Title2.text = "STAGE SELECT";
        Title2.color = BLACK;
        x = ((GetScreenWidth()-MeasureText("STAGE SELECT", 64.0f))/2.0f)+2.0f;
        y = ((GetScreenHeight()/4.0f)-100.0f)-2.0f;
        Title2.bounds = {x, y, 100.0f, 50.0f };
        ui_library.root_container.AddChild(&Title2);
    }

    void End() override
    {
        UnloadMusicStream(bgMusic4);
        //CloseAudioDevice();
        UnloadTexture(bg4);
        UnloadImage(img4);
        //ResourceManager::GetInstance()->UnloadAllTextures();
    }

    void Update() override
    {
        UpdateMusicStream(bgMusic4);

        frameCounter4++;
        if (frameCounter4 >= frameDelay4)
        {
            currentAnimFrame4++;
            if (currentAnimFrame4 >= animFrames4) currentAnimFrame4 = 0;
            nextFrameDataOffset4 = img4.width*img4.height*4*currentAnimFrame4;
            UpdateTexture(bg4, ((unsigned char *)img4.data) + nextFrameDataOffset4);
            frameCounter4 = 0;
        }

        ui_library.Update();
    }

    void Draw() override
    {
        PlayMusicStream(bgMusic4);
        if (CheckCollisionPointRec(GetMousePosition(), Title.bounds) && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        {
            DrawTexture(bg4, GetScreenWidth()/2.0f - bg4.width/2.0f, GetScreenHeight()/2.0f - bg4.height/2.0f, WHITE);
        }
        ui_library.Draw();
    }

private:
    UILibrary ui_library;
    Image img4;
    Texture2D bg4;
    int animFrames4;
    unsigned int nextFrameDataOffset4;
    int currentAnimFrame4;
    int frameDelay4;
    int frameCounter4;

    Music bgMusic4;

    Label Title;
    Label Title2;

    bool state;
    bool setAudio4;

    StageSelect()
    {
        state = true;
    }
};

#endif