#include <Core/Scripting/SoundBindings.hpp>
#include <Core/ECS/Registry.hpp>
#include <Core/Resources/AssetManager.hpp>
#include <Sounds/MusicPlayer/MusicPlayer.hpp>
#include <Sounds/SoundPlayer/SoundFxPlayer.hpp>
#include <Logger/Logger.hpp>

using namespace ENGINE_SOUNDS;
using namespace ENGINE_RESOURCES;


void ENGINE_CORE::Scripting::SoundBinder::CreateSoundBind(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
{
    auto& musicPlayer = registry.GetContext<std::shared_ptr<MusicPlayer>>();
    if(!musicPlayer)
    {
        ENGINE_ERROR("Failed to Bind the Music Player to Lua -- not in the Registry");
        return;
    }

    auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();
    if(!assetManager)
    {
        ENGINE_ERROR("Failed to Bind the Music Player to Lua -- Asset Manager not exists in the Registry");
        return;
    }

    lua.new_usertype<MusicPlayer>(
        "Music",
        sol::no_constructor,
        "play", sol::overload(
            [&] (const std::string& musicName, int loops){
                auto music = assetManager->GetMusic(musicName);
                if(!music)
                {
                    ENGINE_ERROR("Failet to get Music [{}] froom the asset Manager", musicName);
                    return;
                }
                musicPlayer->Play(*music, loops);
            },
            [&](const std::string& musicName) {
                auto music = assetManager->GetMusic(musicName);
                if(!music)
                {
                    ENGINE_ERROR("Failet to get Music [{}] froom the asset Manager", musicName);
                    return;
                }
                musicPlayer->Play(*music, -1);
            }
        ),
        "stop", [&](){
            musicPlayer->Stop();
        },
        "pause", [&](){
            musicPlayer->Pause();
        },
        "resume", [&](){
            musicPlayer->Resume();
        },
        "set_volume", [&](int volume){
            musicPlayer->SetVolume(volume);
        },
        "is_playing", [&](){
            return musicPlayer->IsPlaying();
        }
    );

    auto& soundFxPlayer = registry.GetContext<std::shared_ptr<SoundFxPlayer>>();
    if(!soundFxPlayer)
    {
        ENGINE_ERROR("Failed to Bind the Sound Player to Lua -- not in the Registry");
        return;
    }

    lua.new_usertype<SoundFxPlayer>(
        "Sound",
        sol::no_constructor,
        "play", sol::overload(
            [&] (const std::string& soundName)
            {
                auto pSoundFx = assetManager->GetSoundFx(soundName);
                if(!pSoundFx)
                {
                    ENGINE_ERROR("Failed to get [{}] from the assetManager", soundName);
                    return;
                }

                soundFxPlayer->Play(*pSoundFx);
            },
            [&] (const std::string& soundName, int loops, int channel)
            {
                auto pSoundFx = assetManager->GetSoundFx(soundName);
                if(!pSoundFx)
                {
                    ENGINE_ERROR("Failed to get [{}] from the assetManager", soundName);
                    return;
                }

                soundFxPlayer->Play(*pSoundFx, loops, channel);
            }
        ),
        "start",
        [&](int channel){
            soundFxPlayer->Stop(channel);
        },
        "set_volume",
        [&](int channel, int volume){
            soundFxPlayer->SetVolume(channel, volume);
        },
        "is_playing",
        [&](int channel) { return soundFxPlayer->IsPlaying(channel); }
    );
}