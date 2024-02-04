#include "rfpch.h"
#include "Viewport2D.h"
#include "Viewport.h"
#include <imgui.h>
#include "glad\glad.h"
#include <GLFW\glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Referencer {

	const float ZOOM_SENSITIVITY = 2.0f;

	void Viewport2D::handleInput()
	{
		setSelected(true);
			

		ImGuiIO io = ImGui::GetIO();
		if (io.MouseWheel != 0.0f)
		{
			m_width = m_width * (1 + io.MouseWheel / ZOOM_SENSITIVITY);
			m_height = m_height * (1 + io.MouseWheel / ZOOM_SENSITIVITY);
			ImGui::SetWindowSize(ImVec2(m_width, m_height));

		}
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
			setRunning(false);
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
			setOpened(false);

		if (ImGui::IsKeyDown(ImGuiKey_MouseRight) && ImGui::IsWindowHovered())
			ImGui::OpenPopup("my_select");

		//ImGui::PopStyleVar();
		if (ImGui::BeginPopup("my_select"))
		{
			ImGui::MenuItem("Opened", "Del", &isRunning());
			ImGui::MenuItem("Visible", "H", &isOpened());
			if (ImGui::MenuItem("BG color")) {
				ImGui::OpenPopup("ColorPopup");
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("ColorPopup")) 
		{
			static ImVec4 bgColor = ImVec4(0.3f, 0.5f, 0.7f, 1.0f); // Initial background color temp

			// Color picker widget
			ImGui::ColorPicker4("Background Color", (float*)&bgColor, ImGuiColorEditFlags_Float);

			// Apply button
			if (ImGui::Button("Apply")) {
				ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = bgColor; // Change window background color
				ImGui::CloseCurrentPopup(); // Close the pop-up after applying
			}

			ImGui::EndPopup();
		}

		//handle scroll & mouse
	}
	void Viewport2D::pan(float x, float y)
	{
		ImGui::SetNextWindowPos(ImVec2{ m_posX + x, m_posY + y });
	}
	void Viewport2D::zoom(float zoom)
	{

	}
	// constructor takes name bool isOpen and sourcem image path
	Viewport2D::Viewport2D(std::string name, bool isOpen, std::string source, int posX, int posY)
		:Viewport(name, isOpen), m_texture(0), m_posX(posX), m_posY(posY), m_imageSource(source), m_firstTimeRender(true)
	{
		// TODO: popripade mozes nechat nacitany obrazok do pamate
		
		int tempWidth, tempHeight;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* image_data = stbi_load(source.c_str(), &tempWidth, &tempHeight, NULL, 4); // ak vrate nullptr zrus vytvorenie okna to iste 3D
		m_width = tempWidth;
		m_height = tempHeight;
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
	Viewport2D::Viewport2D(const Viewport2D& other)
		:Viewport(other), m_texture(0), m_posX(other.m_posX+10), m_posY(other.m_posY + 10), m_imageSource(other.m_imageSource), m_firstTimeRender(other.m_firstTimeRender)
	{
		int tempWidth, tempHeight;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* image_data = stbi_load(m_imageSource.c_str(), &tempWidth, &tempHeight, NULL, 4); // ak vrate nullptr zrus vytvorenie okna to iste 3D
		m_width = tempWidth;
		m_height = tempHeight;
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
	}
	// function called every frame
	void Viewport2D::onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(.0f, .0f));

		// toto asi netreba
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

		if (offsetX != 0 || offsetY != 0)
		{
			pan(offsetX, offsetY);
		}
		if (m_firstTimeRender)
			ImGui::SetNextWindowSize({ m_width/2, m_height/2 }, ImGuiCond_FirstUseEver);

		ImGui::Begin(getFullName().c_str(), &isRunning(), windowFlags);

		
		//idk maybe overhead

		m_width = ImGui::GetWindowSize().x;
		m_height = ImGui::GetWindowSize().y;

		m_posX = ImGui::GetWindowPos().x;
		m_posY = ImGui::GetWindowPos().y;

		if (ImGui::IsWindowHovered())
			handleInput();
		else
			setSelected(false);
		// will go to render
		

		ImGui::Image((void*)(intptr_t)m_texture, ImVec2(m_width, m_height));
		ImGui::End();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		m_firstTimeRender = false;
	}
	//event handler
	void Viewport2D::onEvent(Event& e)
	{
	}

	int Viewport2D::getType()
	{
		return 0;
	}

	Viewport* Viewport2D::clone()
	{
		return new Viewport2D(*this);
	}

}