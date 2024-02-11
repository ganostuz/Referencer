#include "rfpch.h"
#include "Settings.h"
#include "toml.hpp"

namespace Referencer {

	template<typename T, size_t N>
	void Settings::assignFromTomlArray(const toml::array& arr, glm::vec<N, T>& value) {
		const size_t size = std::min(arr.size(), static_cast<std::size_t>(N));
		for (size_t i = 0; i < size; ++i) {
			value[i] = toml::get<T>(arr[i]);
		}
	}
	toml::value Settings::tomlize(const SettingsHolder& settings) {
		return toml::value{ {"global_config", toml::table{
			{"ImGuiCol_WindowBg", toml::array{settings.ImGuiCol_WindowBg.r, settings.ImGuiCol_WindowBg.g, settings.ImGuiCol_WindowBg.b, settings.ImGuiCol_WindowBg.a}},
			{"ImGuiCol_Header", toml::array{settings.ImGuiCol_Header.r, settings.ImGuiCol_Header.g, settings.ImGuiCol_Header.b, settings.ImGuiCol_Header.a}},
			{"ImGuiCol_HeaderHovered", toml::array{settings.ImGuiCol_HeaderHovered.r, settings.ImGuiCol_HeaderHovered.g, settings.ImGuiCol_HeaderHovered.b, settings.ImGuiCol_HeaderHovered.a}},
			{"ImGuiCol_HeaderActive", toml::array{settings.ImGuiCol_HeaderActive.r, settings.ImGuiCol_HeaderActive.g, settings.ImGuiCol_HeaderActive.b, settings.ImGuiCol_HeaderActive.a}},
			{"ImGuiCol_Button", toml::array{settings.ImGuiCol_Button.r, settings.ImGuiCol_Button.g, settings.ImGuiCol_Button.b, settings.ImGuiCol_ButtonHovered.a}},
			{"ImGuiCol_ButtonHovered", toml::array{settings.ImGuiCol_ButtonHovered.r, settings.ImGuiCol_ButtonHovered.g, settings.ImGuiCol_ButtonHovered.b, settings.ImGuiCol_ButtonHovered.a}},
			{"ImGuiCol_ButtonActive", toml::array{settings.ImGuiCol_ButtonActive.r, settings.ImGuiCol_ButtonActive.g, settings.ImGuiCol_ButtonActive.b, settings.ImGuiCol_ButtonActive.a}},
			{"ImGuiCol_FrameBg", toml::array{settings.ImGuiCol_FrameBg.r, settings.ImGuiCol_FrameBg.g, settings.ImGuiCol_FrameBg.b, settings.ImGuiCol_FrameBg.a}},
			{"ImGuiCol_FrameBgHovered", toml::array{settings.ImGuiCol_FrameBgHovered.r, settings.ImGuiCol_FrameBgHovered.g, settings.ImGuiCol_FrameBgHovered.b, settings.ImGuiCol_FrameBgHovered.a}},
			{"ImGuiCol_FrameBgActive", toml::array{settings.ImGuiCol_FrameBgActive.r, settings.ImGuiCol_FrameBgActive.g, settings.ImGuiCol_FrameBgActive.b, settings.ImGuiCol_FrameBgActive.a}},
			{"ImGuiCol_TitleBg", toml::array{settings.ImGuiCol_TitleBg.r, settings.ImGuiCol_TitleBg.g, settings.ImGuiCol_TitleBg.b, settings.ImGuiCol_TitleBg.a}},
			{"ImGuiCol_TitleBgActive", toml::array{settings.ImGuiCol_TitleBgActive.r, settings.ImGuiCol_TitleBgActive.g, settings.ImGuiCol_TitleBgActive.b, settings.ImGuiCol_TitleBgActive.a}},
			{"ImGuiCol_TitleBgCollapsed", toml::array{settings.ImGuiCol_TitleBgCollapsed.r, settings.ImGuiCol_TitleBgCollapsed.g, settings.ImGuiCol_TitleBgCollapsed.b, settings.ImGuiCol_TitleBgCollapsed.a}},
		}} 
		};
	}
	void Settings::initDefaultSettings()
	{
		m_Settings.ImGuiCol_WindowBg = { 0.1f, 0.105f, 0.11f, 1.0f };

		m_Settings.ImGuiCol_Button = { 0.2f, 0.205f, 0.21f, 1.0f };
		m_Settings.ImGuiCol_ButtonActive = { 0.15f, 0.1505f, 0.151f, 1.0f };
		m_Settings.ImGuiCol_ButtonHovered = { 0.3f, 0.305f, 0.31f, 1.0f };
		  
		m_Settings.ImGuiCol_FrameBg = { 0.2f, 0.205f, 0.21f, 1.0f };
		m_Settings.ImGuiCol_FrameBgActive = { 0.15f, 0.1505f, 0.151f, 1.0f };
		m_Settings.ImGuiCol_FrameBgHovered = { 0.3f, 0.305f, 0.31f, 1.0f };
		  
		m_Settings.ImGuiCol_Header = { 0.2f, 0.205f, 0.21f, 1.0f };
		m_Settings.ImGuiCol_HeaderActive = { 0.15f, 0.1505f, 0.151f, 1.0f };
		m_Settings.ImGuiCol_HeaderHovered = { 0.3f, 0.305f, 0.31f, 1.0f };
		  
		m_Settings.ImGuiCol_TitleBg = { 0.15f, 0.1505f, 0.151f, 1.0f };
		m_Settings.ImGuiCol_TitleBgActive = { 0.15f, 0.1505f, 0.151f, 1.0f };
		m_Settings.ImGuiCol_TitleBgCollapsed = { 0.15f, 0.1505f, 0.151f, 1.0f };
	}
	Settings::Settings(std::string settingsPath)
		:m_settingsPath(settingsPath)
	{
		if (m_settingsPath == "")
		{
			initDefaultSettings();
		}
		else
		{
			//error handling, app is throwing exceptions when type or count is wrong
			const auto data = toml::parse(m_settingsPath);
			const auto& config = toml::find(data, "global_config");
			// mozes skusit aj cez find_or
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_WindowBg")), m_Settings.ImGuiCol_WindowBg);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_Header")), m_Settings.ImGuiCol_Header);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_HeaderHovered")), m_Settings.ImGuiCol_HeaderHovered);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_HeaderActive")), m_Settings.ImGuiCol_HeaderActive);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_Button")), m_Settings.ImGuiCol_Button);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_ButtonHovered")), m_Settings.ImGuiCol_ButtonHovered);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_ButtonActive")), m_Settings.ImGuiCol_ButtonActive);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_FrameBg")), m_Settings.ImGuiCol_FrameBg);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_FrameBgHovered")), m_Settings.ImGuiCol_FrameBgHovered);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_FrameBgActive")), m_Settings.ImGuiCol_FrameBgActive);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_TitleBg")), m_Settings.ImGuiCol_TitleBg);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_TitleBgActive")), m_Settings.ImGuiCol_TitleBgActive);
			assignFromTomlArray(toml::get<toml::array>(config.at("ImGuiCol_TitleBgCollapsed")), m_Settings.ImGuiCol_TitleBgCollapsed);
			//std::string title = toml::find<std::string>(data, "saveDir");
		}
			
	}

	void Settings::saveSettings(std::string settingsPath)
	{
		if (settingsPath == "")
		{
			if (m_settingsPath == "")
			{
				// assert no valid path use local
				m_settingsPath = "D:\\dev\\temp";
			}
			settingsPath = m_settingsPath;
		}

		std::ofstream file(settingsPath);
		if (file.is_open()) {
			file << std::setw(80) << tomlize(m_Settings);
			file.close();
			//ok
		}
		else {
			// assert
		}

	}

	SettingsHolder& Settings::getSettings()
	{
		return m_Settings;
	}

}