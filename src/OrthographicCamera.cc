//
// Created by kate on 6/7/23.
//

#include <glm/gtc/matrix_transform.hpp>
#include <Renderer/OrthographicCamera.hh>

namespace kaTe {

    OrthographicCamera::OrthographicCamera(double left, double right, double bottom, double top)
        :   m_Projection{ glm::ortho(left, right, bottom, top, s_DefaultNearPlane, s_DefaultFarPlane) }, m_ViewMatrix{ 1.0f }
    {
        m_ProjectionAndView = m_Projection * m_ViewMatrix;
    }

    OrthographicCamera::OrthographicCamera(double left, double right, double bottom, double top, double zNear, double zFar)
        :   m_Projection{ glm::ortho(left, right, bottom, top, zNear, zFar) }, m_ViewMatrix{ 1.0f }
    {
        m_ProjectionAndView = m_Projection * m_ViewMatrix;
    }

    auto OrthographicCamera::recomputeViewMatrix() -> void {
        constexpr glm::mat4 identityMatrix(1.0f);
        constexpr glm::vec3 zAxis{ 0.0f, 0.0f, 1.0f };

        glm::mat4 transform{ glm::translate(identityMatrix, m_Position) *
                                glm::rotate(identityMatrix, static_cast<float>(glm::radians(m_Rotation)), zAxis) };

        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionAndView = m_Projection * m_ViewMatrix;
    }
}