//
// Created by kate on 6/7/23.
//

#ifndef KATE_ENGINE_ORTHOGRAPHIC_CAMERA_HH
#define KATE_ENGINE_ORTHOGRAPHIC_CAMERA_HH

#include "Platform/Window/Window.hh"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace kaTe {
    class OrthographicCamera {
    public:
        OrthographicCamera(double left, double right, double bottom, double top);
        OrthographicCamera(double left, double right, double bottom, double top, double zNear, double zFar);

        auto getFieldOfView() const -> double { return m_FieldOfView; }
        auto getAspectRatio() const -> double { return m_AspectRatio; }
        auto getRotation() const -> double { return m_Rotation; }
        auto getPosition() const -> const glm::vec3& { return m_Position; }
        auto getProjection() const -> const glm::mat4& { return m_Projection; }
        auto getView() const -> const glm::mat4& { return m_ViewMatrix; }
        auto getProjectionView() const -> const glm::mat4& { return m_ProjectionAndView; }

        auto setFieldOfView(double fov) -> void{ m_FieldOfView = fov; recomputeViewMatrix(); }
        auto setAspectRatio(double ar) -> void{ m_AspectRatio = ar; recomputeViewMatrix(); }
        // Takes rotations in degrees
        auto setRotation(double rotation) -> void { m_Rotation = rotation; recomputeViewMatrix(); }
        auto setPosition(const glm::vec3& pos) -> void{ m_Position = pos; recomputeViewMatrix(); }
        auto setPosition(double x, double y) -> void{ m_Position = { x, y, 0.0 }; recomputeViewMatrix(); }
        auto setProjection(glm::mat4 proj) -> void{ m_Projection = proj; recomputeViewMatrix(); }
        auto setProjection(double left, double right, double bottom, double top) -> void;
        auto setView(glm::mat4 view) -> void{ m_ViewMatrix = view; recomputeViewMatrix(); }
        auto updateProjection(const Window &window) -> void;
    private:
        auto recomputeViewMatrix() -> void;
    private:
        static constexpr double s_DefaultNearPlane{ -1.0 };
        static constexpr double s_DefaultFarPlane{ 1.0 };

        glm::mat4 m_ViewMatrix{};
        glm::mat4 m_Projection{};
        glm::mat4 m_ProjectionMatrix{};
        glm::vec3 m_Position{};

        // To avoid recomputing (projection * view) if not necessary
        glm::mat4 m_ProjectionAndView{};

        double m_FieldOfView{};
        double m_AspectRatio{};

        // Rotation in degrees
        double m_Rotation{};
    };
}


#endif//KATE_ENGINE_ORTHOGRAPHIC_CAMERA_HH
