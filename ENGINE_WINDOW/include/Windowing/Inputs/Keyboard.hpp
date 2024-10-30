#pragma once 

#include "Windowing/Inputs/Keys.hpp"
#include "Windowing/Inputs/Button.hpp"
#include <map>


namespace ENGINE_WINDOWING::Inputs
{
    class Keyboard
    {

    private:
    std::map<int, Button> m_mapButtons;


    public:
        Keyboard();
        ~Keyboard() = default;

        void Update(); //(Reset)
        void OnKeyPressed(int key);
        void OnKeyReleased(int key);

        const bool IsKeyPressed(int key) const;
        const bool IsKeyJustPressed(int key) const;
        const bool IsKeyJustReleased(int key) const;

        const std::map<int, Button>& GetButtonMap() const { return m_mapButtons; }

    };

}