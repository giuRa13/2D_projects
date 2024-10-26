#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>


namespace ENGINE_UTIL
{

    struct SDL_Destroyer
    {
		void operator()(SDL_Window* window) const;
		void operator()(SDL_GameController* controller) const;
    void operator()(Mix_Chunk* chunk) const;
    void operator()(Mix_Music* music) const;
		void operator()(SDL_Cursor* cursor) const;
    };


}

//typedef std::shared_ptr<SDL_GameController> Controller;
//static Controller make_shared_controller(SDL_GameController* controller);

typedef std::shared_ptr<SDL_Cursor> Cursor;
static Cursor make_shared_cursor(SDL_Cursor* cursor);

typedef std::unique_ptr<SDL_Window, ENGINE_UTIL::SDL_Destroyer> WindowPtr;

//typedef std::unique_ptr<Mix_Chunk, ENGINE_UTIL::SDL_Destroyer> SoundFxPtr;
typedef std::unique_ptr<Mix_Music, ENGINE_UTIL::SDL_Destroyer> MusicPtr;