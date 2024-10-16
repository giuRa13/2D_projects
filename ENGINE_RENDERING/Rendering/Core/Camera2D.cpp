#include "Camera2D.hpp"


namespace ENGINE_RENDERING
{

	Camera2D::Camera2D() : Camera2D(640, 480)
	{ }


	Camera2D::Camera2D(int width, int height)
        : m_Width{ width }, m_Height{ height }, m_Scale{ 1.0f },
        m_Position{ glm::vec2{0} }, m_CameraMatrix{ 1.0f }, m_OrthoProjection{ 1.0f }, m_bNeedsUpdate{ true }
	{
        // Origin Top Left                                  // Origin Bottom Left
        m_OrthoProjection = glm::ortho(
            0.0f,                             // Left       
            static_cast<float>(m_Width),      // Right      
            static_cast<float>(m_Height),     // Bottom         0.0f,
            0.0f,                             // Top            static_cast<float>(m_Height), 
            0.0f,                            // near       
            1.0f                              // far       
        );

        Update();
	}


    void Camera2D::Update()
    {
        if (!m_bNeedsUpdate)
            return;

        glm::vec3 translate{ -m_Position.x, -m_Position.y, 0.0f };
        m_CameraMatrix = glm::translate(m_OrthoProjection, translate);

        glm::vec3 scale{ m_Scale, m_Scale, 0.0f };
        m_CameraMatrix *= glm::scale(glm::mat4(1.0f), scale);

        m_bNeedsUpdate = false;
    }

}