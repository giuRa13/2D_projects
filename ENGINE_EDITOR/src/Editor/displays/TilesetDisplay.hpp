#pragma once

#include "IDisplay.hpp"
#include <string>



namespace ENGINE_EDITOR
{

    class TilesetDisplay : public IDisplay
    {

    private:
        std::string m_sTileset{""};
        int m_Selected{-1};


    public:
        TilesetDisplay() = default;
        ~TilesetDisplay() = default;

        virtual void Draw() override;

        inline void SetTileset(const std::string& sTileset) { m_sTileset = sTileset; }
        inline const std::string& GetTilesetName() const { return m_sTileset; }

    };

}