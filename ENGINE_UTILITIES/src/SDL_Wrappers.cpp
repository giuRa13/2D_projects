#include <EngineUtils/SDL_Wrappers.hpp>
#include <iostream>
#include <Logger/Logger.hpp>


void ENGINE_UTIL::SDL_Destroyer::operator()( SDL_Window* window ) const
{
	SDL_DestroyWindow( window );
	//std::cout <<"Destoyed SDL Window\n";
	ENGINE_LOG("Destoyed SDL Window");
}

/*void ENGINE_UTIL::SDL_Destroyer::operator()( SDL_GameController* controller ) const
{

}
void ENGINE_UTIL::SDL_Destroyer::operator()( SDL_Cursor* cursor ) const
{

}*/

void ENGINE_UTIL::SDL_Destroyer::operator()(Mix_Chunk* chunk) const
{
	Mix_FreeChunk(chunk);
	std::cout <<"Freed SDL Mix_Chunk\n";
}
    
void ENGINE_UTIL::SDL_Destroyer::operator()(Mix_Music* music) const
{
	Mix_FreeMusic(music);
	std::cout <<"Freed SDL Mix_Music\n";
}

/*Controller make_shared_controller(SDL_GameController* controller)
{
    return Controller();
}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{
	
}*/