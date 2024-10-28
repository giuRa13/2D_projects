#pragma once 

#include <glad/glad.h>
#include <memory>
#include <vector>
#include "Rendering/Essentials/Primitives.hpp"
#include "Rendering/Core/BatchRenderer.hpp"
#include "Rendering/Core/LineBatchRenderer.hpp"
#include "Rendering/Core/RectBatchRenderer.hpp"
#include "Rendering/Core/CircleBatchRenderer.hpp"
#include "Rendering/Core/TextBatchRenderer.hpp"
#include "Rendering/Essentials/Shader.hpp"
#include "Rendering/Core/Camera2D.hpp"



namespace ENGINE_RENDERING
{

    class Renderer
    {

    public:

        enum class GLCapability : GLuint
        {
            BLEND = GL_BLEND,
            DEPHT_TEST = GL_DEPTH_TEST,
            LINE_SMOOTH = GL_LINE_SMOOTH,
        };

        enum class BlendingFactors : GLuint
        {
            ONE = GL_ONE,
            SRC_ALPHA = GL_SRC_ALPHA,
            ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        };


    private:
        std::vector<Line> m_Lines;
        std::vector<Rect> m_Rects;
        std::vector<Circle> m_Circles;
        std::vector<Text> m_Text;

        std::unique_ptr<LineBatchRenderer> m_pLineBatch;
        std::unique_ptr<RectBatchRenderer> m_pRectBatch;
        std::unique_ptr<CircleBatchRenderer> m_pCircleBatch;
        std::unique_ptr<SpriteBatchRenderer> m_pSpriteBatch;
        std::unique_ptr<TextBatchRenderer> m_pTextBatch;


    public:
        Renderer();
        ~Renderer() = default;

        // OpenGL wrappers
        void SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
        void ClearBuffers(bool color = true, bool depth = true, bool stencil = false);

        void SetBlendCapability(BlendingFactors sFactor, BlendingFactors dFactor);
        void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);

        void SetCapability(GLCapability capability, bool enable);
        bool IsCapabilityEnabled(GLCapability capability) const;

        void SetLineWidth(GLfloat lineWidth);

        // Draw calls
        void DrawLine(const Line& line);
        void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, float lineWidth = 1.f);

        void DrawRect(const Rect& rect);
        void DrawRect(const glm::vec2& position, float width, float height, const Color& color);
        void DrawFilledRect(const Rect& rect);

        void DrawCircle(const Circle& circle);
        void DrawCircle(const glm::vec2& position, float radius, const Color& color, float thickness = 1.f);

        void DrawText2D(const Text& text);

        void DrawLines(class Shader& shader, class Camera2D& camera);
        void DrawFilledRects(class Shader& shader, class Camera2D& camera);
        void DrawCircles(class Shader& shader, class Camera2D& camera);
        void DrawAllText(class Shader& shader, class Camera2D& camera);

        void ClearPrimitives();

    };

}