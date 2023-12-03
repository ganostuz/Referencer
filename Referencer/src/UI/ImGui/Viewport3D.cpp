#include "rfpch.h"

#include "Viewport3D.h"
#include "Viewport.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Referencer {

	Viewport3D::Viewport3D(std::string name, bool isOpen)
		:Viewport(name, isOpen), m_firstMouse(true), m_captureMouse(false),m_width(400), m_height(400), m_shader("resources/modelShader.vertex", "resources/modelShader.fragment"), m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)), m_model("D:/dev/Referencer/Referencer/resources/objects/backpack/backpack.obj")
	{
		genBuffers();
	}

	void Viewport3D::handleInput()
	{

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
			setRunning(false);
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
			setOpened(false);

		// mouse positions & click
		if (ImGui::IsMouseDown(2))
		{
			ImVec2 currPos = ImGui::GetMousePos();

			if (!m_firstMouse)
			{
				m_camera.Rotate(glm::vec2(lastX, lastY), glm::vec2(currPos.x, currPos.y), glm::vec2(m_width, m_height));
			}
			lastX = currPos.x;
			lastY = currPos.y;
			m_firstMouse = false;
		}
		else
		{
			m_firstMouse = true;
		}

		// wheel scroll
		ImGuiIO& io = ImGui::GetIO();

		// Check if the mouse wheel was scrolled
		if (io.MouseWheel != 0.0f)
		{
			// Call your Zoom function with the mouse wheel delta
			m_camera.Zoom(io.MouseWheel);
		}



	}

	void Viewport3D::genBuffers()
	{
		glGenFramebuffers(1, &m_fbo);
		glGenTextures(1, &m_texture);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);  // width and height
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "FRAMEBUFFER NOT DONE" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Viewport3D::deleteBuffers()
	{
		glDeleteTextures(1, &m_texture);
		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteFramebuffers(1, &m_fbo);
	}

	void Viewport3D::renderModel()
	{

		glViewport(0, 0, m_width, m_height);
		m_shader.bind();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(30.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		m_shader.setMat4("projection", projection);
		m_shader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		m_shader.setMat4("model", model);
		m_model.Draw(m_shader);

		m_shader.unBind();
	}

	void Viewport3D::onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom)
	{
		// fuj ale funguje
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderModel();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(.0f, .0f));
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);

		ImGui::Begin((getName() + " 3D").c_str(), &isRunning(), windowFlags);

		if (ImGui::IsWindowFocused())
		{
			handleInput();
		}
		if (m_width != ImGui::GetWindowSize().x || m_height != ImGui::GetWindowSize().y)
		{
			m_width = ImGui::GetWindowSize().x;
			m_height = ImGui::GetWindowSize().y;

			// resize
			//deleteBuffers();
			//genBuffers();
		}

		ImGui::Image((void*)(intptr_t)m_texture, ImVec2{ (float)m_width, (float)m_height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	void Viewport3D::onEvent(Event& e)
	{
	}

	Viewport3D::~Viewport3D()
	{
		deleteBuffers();
	}

}