#pragma once

#include "Batcher.hpp"
#include "Rendering/Essentials/BatchTypes.hpp"
#include "Rendering/Essentials/Primitives.hpp"


namespace ENGINE_RENDERING
{

    class LineBatchRenderer : public Batcher<LineBatch, LineGlyph>
    {

    private:
        virtual void GenerateBatches() override;
        void Initialize();


    public:
        LineBatchRenderer();
        virtual ~LineBatchRenderer() = default;

        virtual void End() override;
        virtual void Render() override;

        void AddLine(const Line& line);
    };

}