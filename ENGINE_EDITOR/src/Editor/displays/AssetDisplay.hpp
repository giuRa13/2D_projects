#pragma once

#include "IDisplay.hpp"
#include <string>

namespace ENGINE_UTIL
{
    enum class AssetType;
}

namespace ENGINE_EDITOR
{

    class AssetDisplay : public IDisplay
    {

    private:
        const std::vector<std::string> m_SelectableTypes{ "TEXTURES", "FONTS", "MUSIC", "SOUNDFX", "SCENES" };

        bool m_bIsHovered, m_bAssetTypeChanged, m_bRename;
        bool m_bWindowSelected, m_bWindowHovered;
        std::string m_sSelectedAssetName, m_sSelectedType, m_sDragSource, m_sRenameBuf;
        ENGINE_UTIL::AssetType m_eSelectedType;
        float m_AssetSize;
        int m_SelectedID;

        void SetAssetType();
        void DrawSelectedAssets();
        
        unsigned int GetTextureID(const std::string& sAssetName);
        bool DoRenameAsset(const std::string& sOldName, const std::string& sNewName);
        void CheckRename(const std::string& sCheckName);
        void OpenAssetContext(const std::string& sAssetName);


    public:
        AssetDisplay();
        ~AssetDisplay() = default;

        virtual void Draw() override;
        virtual void Update() override;


    };

}