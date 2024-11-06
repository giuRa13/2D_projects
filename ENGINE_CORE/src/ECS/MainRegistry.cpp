#include "Core/ECS/MainRegistry.hpp"
#include "Core/Resources/AssetManager.hpp"
#include <Logger/Logger.hpp>
#include <Sounds/MusicPlayer/MusicPlayer.hpp>
#include <Sounds/SoundPlayer/SoundFxPlayer.hpp>


namespace ENGINE_CORE::ECS
{
    MainRegistry& MainRegistry::GetInstance()
    {
        static MainRegistry instance{};
        return instance;
    }

    void MainRegistry::Initialize()
    {
        m_pMainRegistry = std::make_unique<Registry>();
        ENGINE_ASSERT( m_pMainRegistry && "Failed to initialize main registry." );

        auto pAssetManager = std::make_shared<ENGINE_RESOURCES::AssetManager>();
        m_pMainRegistry->AddToContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>( std::move( pAssetManager ) );

        auto pMusicPlayer = std::make_shared<ENGINE_SOUNDS::MusicPlayer>();
        m_pMainRegistry->AddToContext<std::shared_ptr<ENGINE_SOUNDS::MusicPlayer>>( std::move( pMusicPlayer ) );

        auto pSoundPlayer = std::make_shared<ENGINE_SOUNDS::SoundFxPlayer>();
        m_pMainRegistry->AddToContext<std::shared_ptr<ENGINE_SOUNDS::SoundFxPlayer>>( std::move( pSoundPlayer ) );

        m_bInitialized = true;
    }

    ENGINE_RESOURCES::AssetManager& MainRegistry::GetAssetManager()
    {
        ENGINE_ASSERT( m_bInitialized && "Main Registry must be initialized before use." );
        return *m_pMainRegistry->GetContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>();
    }

    ENGINE_SOUNDS::MusicPlayer& MainRegistry::GetMusicPlayer()
    {
        ENGINE_ASSERT( m_bInitialized && "Main Registry must be initialized before use." );
        return *m_pMainRegistry->GetContext<std::shared_ptr<ENGINE_SOUNDS::MusicPlayer>>();
    }

    ENGINE_SOUNDS::SoundFxPlayer& MainRegistry::GetSoundPlayer()
    {
        ENGINE_ASSERT( m_bInitialized && "Main Registry must be initialized before use." );
        return *m_pMainRegistry->GetContext<std::shared_ptr<ENGINE_SOUNDS::SoundFxPlayer>>();
    }

}