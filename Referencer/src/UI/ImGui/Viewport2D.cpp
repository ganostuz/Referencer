#include "rfpch.h"
#include "Viewport2D.h"
#include "Viewport.h"
#include <imgui.h>

namespace Referencer {

	Viewport2D::Viewport2D(std::string name, bool isOpen)
		:Viewport(name, isOpen)
	{
	}

	void Viewport2D::onUpdate()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		ImGui::SetNextWindowSize(ImVec2(200, 200),ImGuiCond_FirstUseEver);

		ImGui::Begin(getName().c_str(), &isRunning(), windowFlags);
		if (ImGui::IsWindowFocused())
		{

			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				setRunning(false);
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
				setOpened(false);
		}
		ImGui::End();

		ImGui::PopStyleVar();
	}

	void Viewport2D::onEvent(Event& e)
	{
	}

}