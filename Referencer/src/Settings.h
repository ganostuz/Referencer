#pragma once
#include "rfpch.h"
#include "glm\glm.hpp"
#include "toml.hpp"

namespace Referencer
{
	struct SettingsHolder
	{
		// main window
		//glm::vec2 initialWindowPos;
		//glm::vec2 initialWindowSize;
		// char* / std::string windowName;

		//ImGui colors
        glm::vec4 ImGuiCol_WindowBg;

        glm::vec4 ImGuiCol_Header;
        glm::vec4 ImGuiCol_HeaderHovered;
        glm::vec4 ImGuiCol_HeaderActive;

		glm::vec4 ImGuiCol_Button;
        glm::vec4 ImGuiCol_ButtonHovered;
        glm::vec4 ImGuiCol_ButtonActive;

        glm::vec4 ImGuiCol_FrameBg;
        glm::vec4 ImGuiCol_FrameBgHovered;
        glm::vec4 ImGuiCol_FrameBgActive;

        glm::vec4 ImGuiCol_TitleBg;
        glm::vec4 ImGuiCol_TitleBgActive;
        glm::vec4 ImGuiCol_TitleBgCollapsed;

	};
	class Settings
	{
	private:
		SettingsHolder m_Settings;
		std::string m_settingsPath;

		template<typename T, size_t N>
		void assignFromTomlArray(const toml::array& arr, glm::vec<N, T>& value);

		void initDefaultSettings();
		toml::value tomlize(const SettingsHolder& settings);
	public:
		Settings(std::string settingsPath = "");
		void saveSettings(std::string settingsPath = "");// asi dame ako null
		SettingsHolder& getSettings();
	};
}
