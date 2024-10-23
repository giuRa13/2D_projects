#include "Windowing/Inputs/Button.hpp"


void ENGINE_WINDOWING::Inputs::Button::Update(bool bPressed)
{
    bJustPressed = !bIsPressed && bPressed;
    bJustReleased = bIsPressed && !bPressed;
    bIsPressed = bPressed;
} 


void ENGINE_WINDOWING::Inputs::Button::Reset()
{
    bJustPressed = false;
    bJustReleased = false;
} 

