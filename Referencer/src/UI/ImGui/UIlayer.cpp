#include "rfpch.h"
#include "UIlayer.h"
#include <imgui.h>
#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"
#include "KeyCodes.h"

#include "GLFW\glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32 
#include "GLFW\glfw3native.h"



//#include "ImGui\imgui_impl_glfw.h"

namespace Referencer {

    UIlayer::UIlayer()
        : Layer("ImGui layer"), m_showMenu(true), m_zoom(1.0f), m_offsetX(0), m_offsetY(0), m_instantOffsetX(0), m_instantOffsetY(0), m_instantZoom(1.0f)
    {
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
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
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
        
        // callbacks

        

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
        // first checks if deleted update();
        // than renders compoments render();
        // in which helper funcition renderViewports, renderMenu, renderViewportStack, etc
        Begin(); // begins new frame
        if (ImGui::IsMouseDragging(0) && ImGui::IsKeyDown(ImGuiKey_LeftShift))
        {
            ImVec2 delta = ImGui::GetMouseDragDelta(0);

            m_instantOffsetX = delta.x - m_offsetX;
            m_instantOffsetY = delta.y - m_offsetY;

            m_offsetX = delta.x;
            m_offsetY = delta.y;

        }
        if (ImGui::IsMouseReleased(0))
        {
            m_offsetX = 0.0f;
            m_offsetY = 0.0f;

            m_instantOffsetX = 0.0f;
            m_instantOffsetY = 0.0f;
        }

        ImGuiIO io = ImGui::GetIO();

        if (io.MouseWheel != 0.0f) 
        {
            m_instantZoom = io.MouseWheel;
            m_zoom += io.MouseWheel;
            std::cout << m_zoom << std::endl;
        }
        else
        {
            m_instantZoom = 0.0f;
        }
        /*
            halfX = SCREEN_WIDTH / 2
            halfY = SCREEN_HEIGHT / 2
                deltaX = xPos - halfX;
            deltaY = yPos - halfY;
            x = halfX + deltaX * zoom + xOff;
            y = halfY + deltaY * zoom + yOff;
        */

        // pass into it correct/instant offset
        UpdateViewports(); // deletes viewports with delete flag + updates them based on pan, zoom

        Render(); // renders components
        
        End(); // renders imgui
    }

    void UIlayer::onEvent(Event& e)
    {
        std::cout << this->getName() << e << std::endl;
        ImGuiIO& io = ImGui::GetIO();
        
        e.setHandled((e.isInCategory(EventCategoryMouse) && io.WantCaptureMouse) ||
            (e.isInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard));
    }

    // main private funcs
    
    // begins new frame
    void UIlayer::Begin()
    {
        static bool idk = true;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::LoadIniSettingsFromMemory(); // from your own file
        ImGui::ShowDemoWindow(&idk);
       
    }

    // renders imgui
    void UIlayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::getApplication(); // not needed anymore... maybe
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
    // renders components
    void UIlayer::Render()
    {
        ImGui::Begin("layers");
        
        RenderMainMenu();

        RenderMenu();

        RenderLayerManager();

        RenderViewports();

        ImGui::End();
    }

    // deletes viewports with delete flag + render 2d,3d viewports that are not hidden
    void UIlayer::UpdateViewports()
    {
        for (auto it = m_viewports.begin(); it != m_viewports.end(); it++)
        {
            if (!(*it)->isRunning())
            {
                delete* it; // to do implement another data structure
                m_viewports.erase(it);
                //m_selected.erase(i); // todo seleced remove handling
                // if viewport is selected erase
                break;
            }
            else if ((*it)->getIsOpened())
                (*it)->onUpdate(m_instantOffsetX, m_instantOffsetY, m_instantZoom, m_zoom); // tu treba passnut offsety a zoom
            /*
            halfX = SCREEN_WIDTH / 2
            halfY = SCREEN_HEIGHT / 2
                deltaX = xPos - halfX;
            deltaY = yPos - halfY;
            x = halfX + deltaX * zoom + xOff;
            y = halfY + deltaY * zoom + yOff;
            */
        }
    }

    // seperation of in class funcs
    // makes native file dialog for saving file
    std::string UIlayer::saveFileDialog(const char* filter)
    {
        OPENFILENAMEA ofn;
        char szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::getApplication().getWindow().getNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }

    //makes native file dialog for loading file
    std::string UIlayer::loadFileDialog(const char* filter)
    {
        OPENFILENAMEA ofn;
        char szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::getApplication().getWindow().getNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }

    void UIlayer::RenderMainMenu()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

        if (m_showMenu && ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "CTRL + N"))
                {
                    // Handle menu item click
                    // pop up save current layout?
                    // destroy this instance of UIlayer and viewports
                    // new UIlayer instance
                }
                if (ImGui::MenuItem("Save", "CTRL + S"))
                {
                    // take path from memory and save here
                }
                if (ImGui::MenuItem("Open", "CTRL + O"))
                {
                    std::string path = loadFileDialog("Referencer file (*.ref)\0*.ref\0*.png\0*.png\0*.jpg\0*.jpg\0");
                    if (path != "")
                    {
                        m_viewports.push_back(new Viewport2D(std::to_string(glfwGetTime()), true, path));
                        //m_selected.push_back(false);
                    }
                }
                if (ImGui::MenuItem("Save as", "CTRL + SHIFT + S"))
                {
                    std::string path = saveFileDialog("Referencer file (*.ref)\0.ref\0");
                    if (path != "")
                    {
                        // save file
                    }
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y")) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::EndMainMenuBar();
        }

        ImGui::PopStyleVar();
    }

    void UIlayer::RenderMenu()
    {
        ImGuiIO io = ImGui::GetIO();
        if (ImGui::IsKeyDown(ImGuiKey_MouseRight))
            ImGui::OpenPopup("my_select_popup");
        if (ImGui::BeginPopup("my_select_popup"))
        {
            for (int i = 0; i < 5; i++)
            {
                if (ImGui::Selectable("menu selectable"))
                {
                }
            }
            ImGui::EndPopup();
        }
    }

    void UIlayer::RenderLayerManager()
    {
        if (ImGui::Button("+ 2d viewport")) {
            m_viewports.push_back(new Viewport2D(std::to_string(glfwGetTime()), true, "")); // source must not be empty or black screen
            //m_selected.push_back(false); // selected tiez pojde to viewports
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        if (ImGui::Button("+ 3d viewport")) {
            m_viewports.push_back(new Viewport3D(std::to_string(glfwGetTime()), true));
            //m_selected.push_back(false);
        }

        
    }

    void UIlayer::RenderViewports()
    {
        for (int i = 0; i < m_viewports.size(); i++)
        {
            ImGui::MenuItem(m_viewports[i]->getName().c_str(), NULL, &m_viewports[i]->getIsOpened());
        }

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
        ImGui::Begin("Test");
        if (ImGui::BeginTable("table", 1, ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable))
        {
            for (int i = 0; i < m_viewports.size(); i++)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 10, 10 });// okliestenie checkbox
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, style.CellPadding.y * 2)); // Fix
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 15, style.CellPadding.y }); // vyska riadku
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 12.5, 0 });

                ImGui::TableNextRow(0, .0f); // min row height
                ImGui::TableSetColumnIndex(0);
                ImGui::SetNextItemAllowOverlap();
                ImGui::MenuItem(m_viewports[i]->getName().c_str(), NULL, &m_viewports[i]->getIsOpened());

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2, style.ItemSpacing.y)); // Fix

                ImGui::SameLine(ImGui::GetWindowWidth() - 20.0f - ImGui::GetStyle().FramePadding.x * 2.0f);
                ImGui::Checkbox(("##" + m_viewports[i]->getName()).c_str(), &m_viewports[i]->getIsOpened());
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
            }
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();

        ImGui::End();
        


    }

}
