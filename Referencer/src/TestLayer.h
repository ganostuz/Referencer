#pragma once

#include "rfpch.h"
#include "Layer.h"
#include "events\Event.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer/Shader.h>
//#include <Renderer/Camera.h>
#include <Renderer/Model.h>
#include "events\KeyEvent.h"
#include "events\MouseEvent.h"
#include "events\Event.h"
#include "stb_image.h"
//#include "Renderer\arcball_camera.h"
#include "Renderer\MyCamera.h"

namespace Referencer {
	glm::vec2 transform_mouse(glm::vec2 in)
	{
		return glm::vec2(in.x * 2.f / 800.0f - 1.f, 1.f - 2.f * in.y / 600.0f);
	}

	class TestLayer : public Layer
	{

	private:
		Model ourModel;
		//Camera ourCamera;
		Camera ourCamera;
		Shader ourShader;
		bool firstMouse = true;
		bool captureMouse = false, captureWheelClick = false;
		float lastX, lastY;

		bool handleKeyboard(KeyEvent& e)
		{
			//float deltaTime = 0.01f;
			//if (e.getKeyCode() == GLFW_KEY_W)
			//	ourCamera.ProcessKeyboard(FORWARD, deltaTime);
			//if (e.getKeyCode() == GLFW_KEY_S)
			//	ourCamera.ProcessKeyboard(BACKWARD, deltaTime);
			//if (e.getKeyCode() == GLFW_KEY_A)
			//	ourCamera.ProcessKeyboard(LEFT, deltaTime);
			//if (e.getKeyCode() == GLFW_KEY_D)
			//	ourCamera.ProcessKeyboard(RIGHT, deltaTime);
		//mozme dat ze sme to handlovali...
			return true;
		}
		bool handleScroll(MouseScrolledEvent& e)
		{
			//ourCamera.ProcessMouseScroll(e.getOffsetY() * 0.1);
			ourCamera.Zoom(e.getOffsetY());

			//mozme dat ze sme to handlovali...
			return true;
		}
		bool handleMouse(MouseMovedEvent& e)
		{
			if (captureMouse || captureWheelClick) 
			{
				if (!firstMouse)
				{
					if (captureMouse)
						ourCamera.Rotate(glm::vec2(lastX, lastY), glm::vec2(e.getX(), e.getY()));
					if (captureWheelClick)
						//ourCamera.pan(transform_mouse(glm::vec2(lastX, lastY)) - transform_mouse(glm::vec2(e.getX(), e.getY())));
					lastX = e.getX();
					lastY = e.getY();
				}
				lastX = e.getX();
				lastY = e.getY();
				firstMouse = false;
				

				return true;
			}
			return false;
			
		}
		bool handleMouseButtonPressed(MouseButtonPressed& e)
		{
			if (e.getMouseButton() == GLFW_MOUSE_BUTTON_1)
				captureMouse = true; 
			else if (e.getMouseButton() == GLFW_MOUSE_BUTTON_3)
				captureWheelClick = true;
			return false;
		}
		bool handleMouseButtonReleased(MouseButtonReleased& e)
		{
			if (e.getMouseButton() == GLFW_MOUSE_BUTTON_1)
			{
				firstMouse = true;
				captureMouse = false;
			}
			else if (e.getMouseButton() == GLFW_MOUSE_BUTTON_3)
			{
				captureWheelClick = false;
				firstMouse = true;
			}
			return false;
		}
	public:
		TestLayer(const std::string& name = "Layer")
			: Layer(name), ourShader("resources/modelShader.vertex", "resources/modelShader.fragment"), ourCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f, 1.f, 0.f)), ourModel("D:/dev/Referencer/Referencer/resources/objects/backpack/backpack.obj"){}
		~TestLayer(){}

		void onAttach()
		{
			
			const unsigned int SCR_WIDTH = 600;
			const unsigned int SCR_HEIGHT = 600;
			glEnable(GL_DEPTH_TEST);
			float lastX = SCR_WIDTH / 2.0f;
			float lastY = SCR_HEIGHT / 2.0f;
			bool firstMouse = true;
		}
		void onDetach() {}
		void onUpdate() 
		{
			const unsigned int SCR_WIDTH = 800;
			const unsigned int SCR_HEIGHT = 600;

			// don't forget to enable shader before setting uniforms
			ourShader.bind();

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(30.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = ourCamera.GetViewMatrix();
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);

			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);
		}
		void onEvent(Event& event) 
		{
			float deltaTime = 0.001f;
			EventDispatcher dispatcher(event);
			dispatcher.dispatch<KeyPressedEvent>(std::bind(&TestLayer::handleKeyboard, this, std::placeholders::_1));
			dispatcher.dispatch<MouseScrolledEvent>(std::bind(&TestLayer::handleScroll, this, std::placeholders::_1));
			dispatcher.dispatch<MouseMovedEvent>(std::bind(&TestLayer::handleMouse, this, std::placeholders::_1));
			dispatcher.dispatch<MouseButtonPressed>(std::bind(&TestLayer::handleMouseButtonPressed, this, std::placeholders::_1));
			dispatcher.dispatch<MouseButtonReleased>(std::bind(&TestLayer::handleMouseButtonReleased, this, std::placeholders::_1));
		
		}
	};
}

