#include <Sounds/MusicPlayer/MusicPlayer.hpp>
#include <Sounds/Essentials/Music.hpp>
#include <Logger/Logger.hpp>


namespace ENGINE_SOUNDS
{

    MusicPlayer::MusicPlayer()
    {
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) // 2 for stereo(2 channels)
        {
            std::string error{Mix_GetError()};
            ENGINE_ERROR("Unable to open SDL_Music_MIXER - {}", error);
            return;
        }
        ENGINE_LOG("CHANNELS ALLOCATED [{}]", Mix_AllocateChannels(16));
    }
    
    MusicPlayer::~MusicPlayer()
    {
        Mix_HaltMusic();
        Mix_CloseAudio();
        Mix_Quit();
        ENGINE_LOG("Music Player Closed");
    }


    void MusicPlayer::Play(Music& music, int loops)
    {
        if ( !music.GetMusicPtr() )
        {
            ENGINE_ERROR( "Failed to play music [{}] - Mix Music was Null", music.GetName() );
            return;
        }

        if ( Mix_PlayMusic( music.GetMusicPtr(), loops ) != 0 )
        {
            std::string error{ Mix_GetError() };
            ENGINE_ERROR( "Failed to play music [{}] Mix_Error - {}", music.GetName(), error );
        }
    }


    void MusicPlayer::Pause()
    {
        Mix_PausedMusic();
    }

    void MusicPlayer::Resume()
    {
        Mix_ResumeMusic();
    }

    void MusicPlayer::Stop()
    {
        Mix_HaltMusic();
    }

    void MusicPlayer::SetVolume(int volume)
    {
        if(volume < 0 || volume > 100)
        {
            ENGINE_ERROR("Failed to set volume. Must be betweeb 0 - 100 -- Input [{}]", volume);
            return;
        }

        int volume_changed = static_cast<int>((volume / 100.f) * 128);
        Mix_VolumeMusic(volume_changed);
    }

    bool MusicPlayer::IsPlaying()
    {
        return Mix_PlayingMusic();
        // 1 if Music playing
        // 0 if its not
    }

}