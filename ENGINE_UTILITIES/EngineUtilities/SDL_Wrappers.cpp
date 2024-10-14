#include "SDL_Wrappers.hpp"
#include <iostream>


void ENGINE_UTIL::SDL_Destroyer::operator()(SDL_Window* window) const
{
    SDL_DestroyWindow(window);
    std::cout << "Destroyed SDL Window\n";
}

void ENGINE_UTIL::SDL_Destroyer::operator()(SDL_GameController* controller) const
{
}

void ENGINE_UTIL::SDL_Destroyer::operator()(SDL_Cursor* cursor) const
{
}

Controller make_shared_controller(SDL_GameController* controller)
{
    return Controller();
}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{
    return Cursor();
}
