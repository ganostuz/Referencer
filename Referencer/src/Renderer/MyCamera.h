#pragma once
#include "glm\glm.hpp"
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

namespace Referencer {

    class Camera
    {
    public:
        Camera() = default;

        Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector)
            : m_eye(std::move(eye))
            , m_lookAt(std::move(lookat))
            , m_upVector(std::move(upVector))
        {
            UpdateViewMatrix();
        }

        glm::mat4x4 GetViewMatrix() const { return m_viewMatrix; }
        glm::vec3 GetEye() const { return m_eye; }
        glm::vec3 GetUpVector() const { return m_upVector; }
        glm::vec3 GetLookAt() const { return m_lookAt; }

        // Camera forward is -z
        glm::vec3 GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
        glm::vec3 GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }

        void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
        {
            m_eye = std::move(eye);
            m_lookAt = std::move(lookat);
            m_upVector = std::move(up);
            UpdateViewMatrix();
        }

        void UpdateViewMatrix()
        {
            // Generate view matrix using the eye, lookAt and up vector
            glm::vec3 viewDirection = glm::normalize(m_lookAt - m_eye);
            m_viewMatrix = glm::lookAt(m_eye + viewDirection * m_zoom, m_lookAt, m_upVector);
        }
        void Rotate(glm::vec2 prevPos, glm::vec2 currPos, glm::vec2 screenSize)
        {
            float xAngle = (prevPos.x - currPos.x) * (2 * glm::pi<float>() / screenSize.x);
            float yAngle = (prevPos.y - currPos.y) * (2* glm::pi<float>() / screenSize.y);

            float cosAngle = glm::dot(GetViewDir(), m_upVector);
            if (cosAngle * glm::sign(yAngle) > 0.99f)
                yAngle = 0;

            glm::mat4x4 rotationMatrixX(1.0f);
            rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
            m_eye = glm::vec3(rotationMatrixX * glm::vec4(m_eye - m_lookAt, 1.0f)) + m_lookAt;


            // step 3: Rotate the camera around the pivot point on the second axis.
            glm::mat4x4 rotationMatrixY(1.0f);
            rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());

            glm::vec3 finalPosition = glm::vec3(rotationMatrixY * glm::vec4(m_eye - m_lookAt, 1.0f)) + m_lookAt;

            // Update the camera view (we keep the same lookat and the same up vector)
            SetCameraView(finalPosition, GetLookAt(), m_upVector);
        }
        void Rotate(glm::vec2 posDelta,glm::vec2 screenSize)
        {
            float xAngle = posDelta.x * (2 * glm::pi<float>() / screenSize.x);
            float yAngle = posDelta.y * (2 * glm::pi<float>() / screenSize.y);

            float cosAngle = glm::dot(GetViewDir(), m_upVector);
            if (cosAngle * glm::sign(yAngle) > 0.99f)
                yAngle = 0;

            glm::mat4x4 rotationMatrixX(1.0f);
            rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
            m_eye = glm::vec3(rotationMatrixX * glm::vec4(m_eye - m_lookAt, 1.0f)) + m_lookAt;


            // step 3: Rotate the camera around the pivot point on the second axis.
            glm::mat4x4 rotationMatrixY(1.0f);
            rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());

            glm::vec3 finalPosition = glm::vec3(rotationMatrixY * glm::vec4(m_eye - m_lookAt, 1.0f)) + m_lookAt;

            // Update the camera view (we keep the same lookat and the same up vector)
            SetCameraView(finalPosition, GetLookAt(), m_upVector);
        }
        void Zoom(const float zoomDelta)
        {
            m_zoom += (float)zoomDelta; //sensitivity

            printf("%f\n", m_zoom);


            SetCameraView(m_eye, GetLookAt(), m_upVector);
        }
        void reset() 
        { 
            m_zoom = 0.0f; 
            SetCameraView({ 0.0f, 0.0f, 3.0f }, m_lookAt, { 0.0f, 1.0f, 0.0f });
        }
    private:
        glm::mat4x4 m_viewMatrix;
        glm::vec3 m_eye; // Camera position in 3D
        glm::vec3 m_lookAt; // Point that the camera is looking at
        glm::vec3 m_upVector; // Orientation of the camera
        float m_zoom = 1.0f;
    };

}
