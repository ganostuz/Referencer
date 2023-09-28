#include "rfpch.h"
#include "UIlayer.h"
#include <vector>
#include <imgui.h>
#include <imgui_internal.h>
#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"
#include "KeyCodes.h"


//#include "ImGui\imgui_impl_glfw.h"

namespace Referencer {

    UIlayer::UIlayer()
        : Layer("IMgui layer")
    {
        //m_viewports.push_back(new Viewport2D("dlhe meno", true));
        //m_selected.push_back(false);
    }

    UIlayer::~UIlayer()
    {
        onDetach();
    }

    void UIlayer::onAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;


        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowRounding = 5.0f;
        style.ChildRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

        //colors
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        Application& app = Application::getApplication();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

        // Setup Platform/Renderer bindings
            // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

    }

    void UIlayer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
    void UIlayer::onUpdate()
    {
        Begin();
        for (auto it = m_viewports.begin(); it != m_viewports.end(); it++)
        {
            if (!(*it)->isRunning())
            {
                delete* it; // to do implement another data structure
                m_viewports.erase(it);
                //m_selected.erase(i); // todo seleced remove handling
                break;
            }
            else if((*it)->getIsOpened())
                (*it)->onUpdate();
        }


           ImGui::Begin("layers");
           if (ImGui::Button("+ 2d viewport")) {
               m_viewports.push_back(new Viewport2D(std::to_string(glfwGetTime()), true));
               m_selected.push_back(false);
           }
           ImGui::SameLine(ImGui::GetWindowWidth() - 100);
           if (ImGui::Button("+ 3d viewport")) {
               m_viewports.push_back(new Viewport3D(std::to_string(glfwGetTime()), true));
               m_selected.push_back(false);
           }

           for (int i = 0; i < m_viewports.size(); i++)
           {
               ImGui::Checkbox(m_viewports[i]->getName().c_str(), &m_viewports[i]->getIsOpened());
               std::cout << m_viewports[i]->getName().c_str()<< m_viewports[i]->getIsOpened() << std::endl;
           }

           static bool selection[5] = { false, false, false, false, false };
           for (int n = 0; n < 5; n++)
           {
               char buf[32];
               sprintf(buf, "Object %d", n);
               if (ImGui::Selectable(buf, selection[n]))
               {
                   if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
                       memset(selection, 0, sizeof(selection));
                   selection[n] ^= 1;
               }
               if(!ImGui::IsWindowFocused())
                   memset(selection, 0, sizeof(selection));
           }

           ImGui::End();

        End();
    }

    void UIlayer::onEvent(Event& e)
    {
        std::cout << this->getName() << e << std::endl;
        ImGuiIO& io = ImGui::GetIO();
        e.setHandled((e.isInCategory(EventCategoryMouse) && io.WantCaptureMouse) ||
            (e.isInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard));
    }

    void UIlayer::Begin()
    {
        static bool idk = true;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(&idk);
       
    }

    void UIlayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::getApplication();
        io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}
