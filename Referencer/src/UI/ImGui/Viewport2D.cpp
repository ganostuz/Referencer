#include "rfpch.h"
#include "Viewport2D.h"
#include "Viewport.h"
#include <imgui.h>
#include "glad\glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Referencer {

	Viewport2D::Viewport2D(std::string name, bool isOpen, std::string source)
		:Viewport(name, isOpen), texture(0), ratio(0.f)
	{
		// TODO: will go to render
		unsigned char* image_data = stbi_load(source.c_str(), &width, &height, NULL, 4);

		// Create a OpenGL texture identifier
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		if (image_data)
		{
			stbi_image_free(image_data);
		}
		// above to render
	}

	void Viewport2D::onUpdate()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(.0f, .0f));
		
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;
		ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);

		ImGui::Begin(getName().c_str(), &isRunning(), windowFlags);
		if (ImGui::IsWindowFocused())
		{
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
				setRunning(false);
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
				setOpened(false);
		}
		// will go to render
		width = ImGui::GetWindowSize().x;
		height = ImGui::GetWindowSize().y;
		ImGui::Image((void*)(intptr_t)texture, ImVec2(width, height));
		ImGui::End();

		//ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	void Viewport2D::onEvent(Event& e)
	{
	}

}