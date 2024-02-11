#include "rfpch.h"
#include "Viewport2D.h"
#include "Viewport.h"
#include <imgui.h>
#include "glad\glad.h"
#include <GLFW\glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Referencer {

	const float ZOOM_SENSITIVITY = 10.0f;

	void Viewport2D::handleInput()
	{
		setSelected(true);


		ImGuiIO io = ImGui::GetIO();
		if (io.MouseWheel != 0.0f)
		{
			m_zoom = (io.MouseWheel * ZOOM_SENSITIVITY);
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
	Viewport2D::Viewport2D(std::string name, bool isOpen, std::string source, int posX, int posY, std::shared_ptr<ImageData> image)
		:Viewport(name, isOpen), m_texture(0), m_posX(posX), m_posY(posY), m_imageSource(source), m_zoom(0.0f), m_image(image)
	{
		// TODO: popripade mozes nechat nacitany obrazok do pamate
		if (m_image == nullptr)
		{
			m_image = std::make_shared<ImageData>();
			stbi_set_flip_vertically_on_load(false);
			m_image->pixels = stbi_load(source.c_str(), &m_image->width, &m_image->height, NULL, 4); // ak vrate nullptr zrus vytvorenie okna to iste 3D
		}
		m_width = m_image->width;
		m_height = m_image->height;
		// Create a OpenGL texture identifier
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*m_image).width, (*m_image).height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*m_image).pixels);
		// above to render
	}
	Viewport2D::Viewport2D(const Viewport2D& other)
		: Viewport(other), m_texture(0), m_posX(other.m_posX + 10), m_posY(other.m_posY + 10), m_imageSource(other.m_imageSource), m_width(other.m_width), m_height(other.m_height), m_zoom(other.m_zoom)
	{
		// Create a new ImageData object with a copy of the pixel data
		m_image = std::make_shared<ImageData>(*other.m_image);
		// Initialize OpenGL texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image->width, m_image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image->pixels);
	}
	Viewport2D::~Viewport2D()
	{
		glDeleteTextures(1, &m_texture);
	}
	// function called every frame
	void Viewport2D::onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		

		// toto asi netreba
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

		if (offsetX != 0 || offsetY != 0)
		{
			pan(offsetX, offsetY);
		}
		ImGui::SetNextWindowSize({ m_width / 2, m_height / 2 }, ImGuiCond_FirstUseEver);

		if (m_zoom != 0.0f)
		{
			// prv resiznut podla minuleho framu nasledne prijat dalsi input
			ImGui::SetNextWindowPos({ m_posX - m_zoom / 2, m_posY - m_zoom / 2 });
			ImGui::SetNextWindowSize({ m_width + m_zoom, m_height + m_zoom });

			m_zoom = 0.0f;
		}

		

		if (ImGui::Begin(("###" + std::to_string(getID().get())).c_str(), &isRunning(), windowFlags))
		{
			m_width = ImGui::GetWindowSize().x;
			m_height = ImGui::GetWindowSize().y;

			m_posX = ImGui::GetWindowPos().x;
			m_posY = ImGui::GetWindowPos().y;

			if (ImGui::IsWindowHovered())
				handleInput();
			else
				setSelected(false);
			// will go to render

			ImGuiIO io = ImGui::GetIO();
			if (ImGui::IsKeyDown(ImGuiKey_MouseRight) && ImGui::IsWindowHovered())
				ImGui::OpenPopup("my_select_popup");

			if (ImGui::BeginPopup("my_select_popup"))
			{
				ImGui::MenuItem("Opened", "Del", &isRunning());
				ImGui::MenuItem("Visible", "H", &isOpened());
				ImGui::EndPopup();
			}

			ImGui::Image((void*)(intptr_t)m_texture, ImVec2(m_width, m_height));
		}

		ImGui::End();

		ImGui::PopStyleVar(2);

		

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
	void Viewport2D::SerializeImage(std::ostream& os) const {
		// Write the width and height

		os.write(reinterpret_cast<char*>(&this->m_image->width), sizeof(this->m_image->width));
		os.write(reinterpret_cast<char*>(&this->m_image->height), sizeof(this->m_image->height));

		// Write the pixel data // unsigned?
		os.write(reinterpret_cast<char*>(this->m_image->pixels), this->m_image->width * this->m_image->height * 4); // Assuming  4 bytes per pixel (RGBA)
	}

}