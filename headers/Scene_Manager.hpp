#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "UI.hpp"


class SceneManager;

class Scene {
public:
    virtual ~Scene() {};
    virtual void Begin() = 0;
    virtual void End() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    void SetSceneManager(SceneManager* scene_manager)
    {
        this->scene_manager = scene_manager;
    }

    SceneManager* GetSceneManager()
    {
        return scene_manager;
    }

private:
    SceneManager* scene_manager;
};


class SceneManager {
public:
    virtual ~SceneManager() {};

    void RegisterScene(Scene* scene, int scene_id)
    {
        scenes[scene_id] = scene;
    }

    void UnregisterScene(int scene_id)
    {
        scenes.erase(scene_id);
    }

    void SwitchScene(int scene_id)
    {
        if (scenes.find(scene_id) == scenes.end())
        {
            std::cout << "Scene ID not found" << std::endl;
            return;
        }

        if (active_scene != nullptr)
        {
            active_scene->End();
            active_scene = nullptr;
        }

        std::cout << "Moved to Scene " << scene_id << std::endl;
        active_scene = scenes[scene_id];
        active_scene->Begin();
    }

    Scene* GetActiveScene()
    {
        return active_scene;
    }

private:
    std::unordered_map<int, Scene*> scenes;
    Scene* active_scene = nullptr;
};


class ResourceManager {
public:
    virtual ~ResourceManager() {};
    
    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;

    static ResourceManager* GetInstance()
    {
        static ResourceManager insance;
        return &insance;
    }

    Texture GetTexture(const std::string& path)
    {
        std::filesystem::path dirpath = "../textures/";
        std::string texpath = dirpath.string() + path;

        if (textures.find(texpath) == textures.end())
        {
            std::cout << "Loaded " << texpath << " from Disk" << std::endl;
            textures[texpath] = LoadTexture(texpath.c_str());
        }
        else
        {
            std::cout << "Resource Already Loaded" << std::endl;
        }

        return textures[texpath];
    }

    void UnloadAllTextures()
    {
        for (auto it : textures)
        {
            UnloadTexture(it.second);
        }

        textures.clear();
    }

private:
    std::unordered_map<std::string, Texture> textures;
    ResourceManager() {}
};

#endif