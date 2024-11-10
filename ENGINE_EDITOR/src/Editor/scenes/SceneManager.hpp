#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

#define SCENE_MANAGER() ENGINE_EDITOR::SceneManager::GetInstance()


namespace ENGINE_EDITOR
{
    class SceneObject;

    class SceneManager
    {

    private:
        SceneManager() = default;
        ~SceneManager() = default;
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;

        std::map<std::string, std::shared_ptr<ENGINE_EDITOR::SceneObject>> m_mapScenes;
        std::string m_sCurrentScene{""};


    public:
        static SceneManager& GetInstance();

        bool AddScene(const std::string& sSceneName);
        std::shared_ptr<ENGINE_EDITOR::SceneObject> GetScene(const std::string& sSceneName);
        std::shared_ptr<ENGINE_EDITOR::SceneObject> GetCurrentScene();
        std::vector<std::string> GetSceneNames() const;

        inline void SetCurrentScene(const std::string& sSceneName) { m_sCurrentScene = sSceneName; }
        inline const std::string& GetCurrentSceneName() const { return m_sCurrentScene; }

    };

}