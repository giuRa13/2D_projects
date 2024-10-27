#pragma once
#include "Batcher.hpp"
#include "Rendering/Essentials/BatchTypes.hpp"

namespace ENGINE_RENDERING
{

    class CircleBatchRenderer : public Batcher<RectBatch, CircleGlyph>
    {

    private:
        virtual void GenerateBatches() override;
        void Initialize();


    public:
        CircleBatchRenderer();
        virtual ~CircleBatchRenderer() = default;

        virtual void End() override;
        virtual void Render() override;
        
        void AddCircle( const glm::vec4& destRect, const Color& color, float thickness,
            glm::mat4 model = glm::mat4{ 1.0f } );
        
        void AddCircle( const struct Circle& circle );
    };
} 