//
// Created by kate on 6/12/23.
//

#ifndef KATE_ENGINE_ORTHOGRAPHIC_CAMERA_CONTROLLER_HH
#define KATE_ENGINE_ORTHOGRAPHIC_CAMERA_CONTROLLER_HH

#include <memory>

// Force radians always
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <Tools/Common.hh>
#include <Core/Events/Event.hh>

#include "Core/Events/AppEvents.hh"
#include <Core/Events/MouseEvents.hh>
#include <Renderer/Camera/OrthographicCamera.hh>

namespace kaTe {
    /**
     * This class controls an orthographic camera. The camera can be an entity independent to
     * it or be part of it as an aggregate relationship, we specify so upon construction
     * */
    class OrthographicCameraController {
    public:
        /**
         * We can optionally pass this controller a camera if we want it to handle a specific target
         * */
        explicit OrthographicCameraController(double aspectRatio, bool enableRotation = false, std::shared_ptr<OrthographicCamera> target = nullptr);
        OrthographicCameraController(UInt32_T width, UInt32_T height, bool enableRotation = false, std::shared_ptr<OrthographicCamera> target = nullptr);

        auto OnUpdate() -> void;
        /**
         * When we want this class to handle a specific type of event
         * */
        auto OnEvent(Event& event) -> void;

        auto SetProjection(double left, double right, double bottom, double top) -> void;

        /**
         * This function tells whether this OrthographicCameraController controls and external camera or not
         * meaning the camera is an independent entity to this OrthographicCameraController
         * */
        auto HasCamera() -> bool { return m_TargetCamera != nullptr; }

        auto GetCamera() const -> const std::shared_ptr<OrthographicCamera>&;
        auto GetCamera() -> std::shared_ptr<OrthographicCamera>&;

    private:
        auto OnMouseScrolledEvent(MouseScrollEvent& event) -> bool;
        auto OnWindowResized(WindowResizedEvent& event) -> bool;
    private:
        OrthographicCameraController(const OrthographicCameraController&) = delete;
        auto operator=(const OrthographicCameraController&) = delete;

        OrthographicCameraController(OrthographicCameraController&&) = delete;
        auto operator=(OrthographicCameraController&&) = delete;
    private:
        static constexpr double s_MaxZoom{ 0.25 };

        double m_AspectRatio{};
        double m_Zoom{ 1.0 };
        double m_FieldOfViewSensitivity{ 0.08 };
        bool m_EnableRotation{};

        glm::vec3 m_TargetCameraPosition{};
        double m_TargetCameraMovementSpeed{ 3.0 };

        // In radians
        double m_TargetCameraRotation{};
        // Represents the rotation in degrees so radians/time_unit (time_unit could be seconds,
        // milliseconds, etc.). The default units are seconds
        double m_TargetCameraRotationSpeed{ 90.0 };

        std::shared_ptr<OrthographicCamera> m_TargetCamera{};
    };
}


#endif//KATE_ENGINE_ORTHOGRAPHIC_CAMERA_CONTROLLER_HH
