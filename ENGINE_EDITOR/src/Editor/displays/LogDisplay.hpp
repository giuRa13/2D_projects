#pragma once

#include "IDisplay.hpp"
#include <imgui.h>


namespace ENGINE_EDITOR
{

    class LogDisplay : public IDisplay
    {

    private:
        ImGuiTextBuffer m_TextBuffer;
        ImVector<int> m_TextOffsets;
        bool m_bAutoScroll;

        void GetLogs();


    public:
        LogDisplay();
        ~LogDisplay() = default;

        void Clear();
        virtual void Draw() override;
    
    };

}