#include "SceneManager.hpp"
#include "SceneObject.hpp"
#include <EngineUtils/EngineUtilities.hpp>
#include <Logger/Logger.hpp>


namespace ENGINE_EDITOR
{

    SceneManager& SceneManager::GetInstance()
    {
        static SceneManager instance{};
        return instance;
    }


    bool SceneManager::AddScene(const std::string& sSceneName)
    {
        if(m_mapScenes.contains(sSceneName))
        {
            ENGINE_ERROR("Failed to add new Scene Object - [{}] already exists", sSceneName);
            return false;
        }

        m_mapScenes.emplace(sSceneName, std::move(std::make_shared<ENGINE_EDITOR::SceneObject>(sSceneName)));

        auto [itr, bSuccess] = m_mapScenes.emplace( sSceneName, std::move( std::make_shared<ENGINE_EDITOR::SceneObject>( sSceneName ) ) );
	    return bSuccess;
    }
    

    std::shared_ptr<ENGINE_EDITOR::SceneObject> SceneManager::GetScene(const std::string& sSceneName)
    {
        auto sceneItr = m_mapScenes.find(sSceneName);
        if(sceneItr == m_mapScenes.end())
        {
            ENGINE_ERROR("Failed to get the Scene Object - [{}] already exists", sSceneName);
            return nullptr;
        }
        return sceneItr->second;
    }

    std::shared_ptr<ENGINE_EDITOR::SceneObject> SceneManager::GetCurrentScene()
    {
        if(m_sCurrentScene.empty())
            return nullptr;

        auto sceneItr = m_mapScenes.find(m_sCurrentScene);
        if(sceneItr == m_mapScenes.end())
        {
            ENGINE_ERROR("Failed to get the Scene Object - [{}] already exists", m_sCurrentScene);
            return nullptr;
        }
        return sceneItr->second;
    }

    std::vector<std::string> SceneManager::GetSceneNames() const
    {
        return ENGINE_UTIL::GetKeys( m_mapScenes );
    }

}