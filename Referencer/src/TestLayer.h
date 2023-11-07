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
#include <Renderer/Camera.h>
#include <Renderer/Model.h>
#include "events\KeyEvent.h"
#include "events\Event.h"

namespace Referencer {
	class TestLayer : public Layer
	{

	private:
		Model ourModel;
		Camera ourCamera;
		Shader ourShader;

		bool handleInput(KeyEvent& e)
		{
			float deltaTime = 0.01f;
			if (e.getKeyCode() == GLFW_KEY_W)
				ourCamera.ProcessKeyboard(FORWARD, deltaTime);
			if (e.getKeyCode() == GLFW_KEY_S)
				ourCamera.ProcessKeyboard(BACKWARD, deltaTime);
			if (e.getKeyCode() == GLFW_KEY_A)
				ourCamera.ProcessKeyboard(LEFT, deltaTime);
			if (e.getKeyCode() == GLFW_KEY_D)
				ourCamera.ProcessKeyboard(RIGHT, deltaTime);
		//mozme dat ze sme to handlovali...
			return true;
		}
	public:
		TestLayer(const std::string& name = "Layer")
			: Layer(name), ourShader("modelLoadingShader.vertex", "modelLoadingShader.fragment"), ourCamera(glm::vec3(0.0f, 0.0f, 3.0f)), ourModel("D:/dev/Referencer/Referencer/resources/objects/backpack/backpack.obj") {}
		~TestLayer(){}

		void onAttach()
		{
			const unsigned int SCR_WIDTH = 800;
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

			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't forget to enable shader before setting uniforms
			ourShader.bind();

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(ourCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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
			float deltaTime = 0.02f;
			EventDispatcher dispatcher(event);
			dispatcher.dispatch<KeyTypedEvent>(std::bind(&TestLayer::handleInput, this, std::placeholders::_1));
		
		}
	};
}

