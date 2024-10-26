#include "Sounds/SoundPlayer/SoundFxPlayer.hpp"
#include "Sounds/Essentials/SoundFX.hpp"
#include <Logger/Logger.hpp>


namespace ENGINE_SOUNDS
{

    void SoundFxPlayer::Play(SoundFX& soundFx)
    {
        Play(soundFx, 0, -1);
    }


    void SoundFxPlayer::Play(SoundFX& soundFx, int loops, int channel)
    {
        if(!soundFx.GetSoundFxPtr())
        {
            ENGINE_ERROR("Failed to play SoundFX[{}] on Channel[{}] -- SoundFX* is null ", soundFx.GetName(), channel);
            return;
        }

        if(Mix_PlayChannel(channel, soundFx.GetSoundFxPtr(), loops) == -1)
        {
            std::string error{Mix_GetError()};
            ENGINE_ERROR("Failed to play SoundFX[{}] on Channel[{}] -- ERROR: {} ", soundFx.GetName(), channel, error);
        }
    }


    void SoundFxPlayer::SetVolume(int channel, int volume)
    {
        if(volume < 0 || volume > 100)
        {
            ENGINE_ERROR("Failed to set volume. Must be betweeb 0 - 100 -- Input [{}]", volume);
            return;
        }

        int volume_changed = static_cast<int>((volume / 100.f) * 128);
        Mix_Volume(channel, volume_changed);
    }


    void SoundFxPlayer::Stop(int channel)
    {
        if(Mix_HaltChannel(channel) == -1)
        {
            std::string error{Mix_GetError()};
            ENGINE_ERROR("Failed to halt SoundFX for Channel[{}]", channel == -1 ? "all channels" : std::to_string(channel));
        }
    }

    bool SoundFxPlayer::IsPlaying(int channel)
    {
        return Mix_Playing(channel);
    }

}