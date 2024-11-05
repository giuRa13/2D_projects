#pragma once

#include "Core/States/State.hpp"
#include <vector>


namespace ENGINE_CORE
{

    class StateStack
    {

    private:
        std::vector<State> m_States{};
        std::unique_ptr<State> m_pStateHolder{nullptr};


    public:
        StateStack() = default;
        ~StateStack() = default;

        void Push(State& state);
        void Pop();
        void ChangeState(State& state);

        void Update(const float dt);
        void Render();

        State& Top();

        static void CreateLuaStateStackBinds(sol::state& lua);

    };

}