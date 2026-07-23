#pragma once
#include "Layer.h"
#include "events/Event.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Application.h"
#include "Viewport.h"
#include "Viewport2D.h"
#include "Viewport3D.h"



namespace Referencer {

	class UIlayer : public Layer
	{
	private:
		std::vector<Viewport*> m_viewports;
		float m_zoom, m_instantZoom;
		int m_offsetX, m_offsetY, m_instantOffsetX, m_instantOffsetY, m_viewportIndex;
		bool m_showMenu, m_wasCopyed, m_showSettings, m_showInternetDialog, m_save;

		std::string m_renameBuffer, m_urlBuffer, m_outputPathBuffer;
		Viewport* m_renameViewportPointer;

	public:
		UIlayer();
		~UIlayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& e);
		void serialize(toml::value& config);

	private:

		void Begin();
		void End();
		void Render();
		void UpdateViewports();

		void LoadSettingsToImGui(const SettingsHolder& settings);
		void SaveImGuiToSettings(SettingsHolder& settings);

		void RenderMainMenu();
		void RenderMenu();
		void RenderLayerManager();
		void RenderViewports();
		void ShowGlobalSettingsEditor();
		void RenderInternetDialog();
		void RenderNotifications();

		bool handleDrops(DragAndDropEvent& e);
		static int TextEditCallback(ImGuiInputTextCallbackData* data);

		

	};
}