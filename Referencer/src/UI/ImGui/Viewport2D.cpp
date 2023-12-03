#include "rfpch.h"
#include "Viewport2D.h"
#include "Viewport.h"
#include <imgui.h>
#include "glad\glad.h"
#include <GLFW\glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Referencer {

	const float ZOOM_SENSITIVITY = 5.0f;

	void Viewport2D::handleInput()
	{
		//handle keyboard
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
			setRunning(false);
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
			setOpened(false);

		//handle scroll & mouse
	}
	void Viewport2D::pan(float x, float y)
	{
		ImGui::SetNextWindowPos(ImVec2{ m_posX + x, m_posY + y });
	}
	void Viewport2D::zoom(float zoom)
	{
		float cursorX = ImGui::GetMousePos().x;
		float cursorY = ImGui::GetMousePos().y;

		float deltaX = m_posX - cursorX;
		float deltaY = m_posY - cursorY;

		// there is a mistake viac sa to presuva k miske nez von
		m_posX = m_posX + deltaX / zoom * (ZOOM_SENSITIVITY * 0.05f);
		m_posY = m_posY + deltaY / zoom * (ZOOM_SENSITIVITY * 0.05f);

		ImGui::SetNextWindowPos(ImVec2{ m_posX, m_posY });

		ImGui::SetNextWindowSize(ImVec2{ 
			m_width + (ZOOM_SENSITIVITY * zoom ), 
			m_height + (ZOOM_SENSITIVITY * zoom ) 
			});
		std::cout << "size: " << m_width + (ZOOM_SENSITIVITY * zoom) << "  " << m_height + (ZOOM_SENSITIVITY * zoom) << std::endl;
		std::cout << "pos: " << m_posX << "  " << m_posY << std::endl;
	}
	// constructor takes name bool isOpen and sourcem image path
	Viewport2D::Viewport2D(std::string name, bool isOpen, std::string source, int posX, int posY)
		:Viewport(name, isOpen), m_texture(0), m_ratio(0.f), m_posX(posX), m_posY(posY)
	{
		// TODO: will go to render
		int tempWidth, tempHeight;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* image_data = stbi_load(source.c_str(), &tempWidth, &tempHeight, NULL, 4);
		m_ratio = tempWidth / tempHeight;

		// Create a OpenGL texture identifier
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempWidth, tempHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		if (image_data)
		{
			stbi_image_free(image_data);
		}
		// above to render
	}
	// function called every frame
	void Viewport2D::onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(.0f, .0f));

		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

		if (offsetX != 0 || offsetY != 0)
		{
			pan(offsetX, offsetY);
		}
		else if (instantZoom != 0.0f)
		{
			zoom(instantZoom);
		}
			

		ImGui::Begin(getName().c_str(), &isRunning(), windowFlags);
		
		//idk maybe overhead

		m_width = ImGui::GetWindowSize().x;
		m_height = ImGui::GetWindowSize().y;
		m_ratio = m_width / m_height;

		m_posX = ImGui::GetWindowPos().x;
		m_posY = ImGui::GetWindowPos().y;

		if (ImGui::IsWindowFocused())
		{
			handleInput();
		}
		// will go to render
		

		ImGui::Image((void*)(intptr_t)m_texture, ImVec2(m_width, m_height));
		ImGui::End();

		//ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
	//event handler
	void Viewport2D::onEvent(Event& e)
	{
	}

}