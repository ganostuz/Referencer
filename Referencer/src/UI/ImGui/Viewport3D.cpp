#include "rfpch.h"
#include "Viewport3D.h"
#include "Viewport.h"
#include <imgui.h>

namespace Referencer {

	Viewport3D::Viewport3D(std::string name, bool isOpen)
		:Viewport(name, isOpen)
	{
	}

	void Viewport3D::onUpdate()
	{
		// fuj ale funguje
		bool wasPreviousWindowFocused = false;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		if (isSelected())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f); // Set the border width to 2 pixels
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.33f, 0.94f, 0.85f, 1.0f)); // Set the border color to red
		}
		ImGui::Begin((getName() + " 3D").c_str(), &isRunning(), windowFlags);

		if (wasPreviousWindowFocused = ImGui::IsWindowFocused())
		{
			
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				setRunning(false);
			if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
				setOpened(false);
		}

		ImGui::End();
		if (isSelected())
		{
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		if (wasPreviousWindowFocused)
		{
			setSelected(true);
		}
		else
		{
			setSelected(false);
		}
	}


	void Viewport3D::onEvent(Event& e)
	{
	}

}