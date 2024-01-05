#include "rfpch.h"
#include "UIlayer.h"
#include <imgui.h>
#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"
#include "KeyCodes.h"

#include "imgui_internal.h"
#include "GLFW\glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32 
#include "GLFW\glfw3native.h"
#include "backends\imgui_impl_glfw.h"



//#include "ImGui\imgui_impl_glfw.h"

namespace Referencer {

    UIlayer::UIlayer()
        : Layer("ImGui layer"), m_showMenu(true), m_zoom(1.0f), m_offsetX(0), m_offsetY(0), m_instantOffsetX(0), m_instantOffsetY(0), m_instantZoom(1.0f), m_viewportIndex(1), m_wasCopyed(false), m_renameBuffer(""), m_renameViewportPointer(nullptr)
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
        //glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
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

        if(ImGui::IsKeyReleased(ImGuiKey_D))
            m_wasCopyed = false;
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

        if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyDown(ImGuiKey_D) && !m_wasCopyed)
        {
            std::vector<Viewport*> duplicatedViewports;

            for (int i = 0; i < m_viewports.size(); ++i)
            {
                if (m_viewports[i]->isSelected())
                    duplicatedViewports.push_back(m_viewports[i]->clone());
            }
            m_viewports.insert(m_viewports.end(), duplicatedViewports.begin(), duplicatedViewports.end());
            m_wasCopyed = true;
        }
        // ak ctrl+c prejdes vsetky viewporty a zapises ich id potom ctrl+v porovnanvas s existujucimi ak su init a push
        // alternativa ctrl+c vytvoris ich a pridas do stacku copied ak ctrl+v pushe cely stack 
        // handlenes to s imgui copy&paste ImGui::SetClipboardText(); // get
        // calc. stred objektu
        // sprav KSI & pozri ako presne sa vytvara okno ked imgui viewport je enabled




        // pass into it correct/instant offset
        UpdateViewports(); // deletes viewports with delete flag + updates them based on pan, zoom

        Render(); // renders components

        End(); // renders imgui
    }

    void UIlayer::onEvent(Event& e)
    {
        std::cout << this->getName() << e << std::endl;
        ImGuiIO& io = ImGui::GetIO();

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<DragAndDropEvent>(std::bind(&UIlayer::handleDrops, this, std::placeholders::_1));

        e.setHandled((e.isInCategory(EventCategoryMouse) && io.WantCaptureMouse) ||
            (e.isInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard));// tu bude asii problem pokial drag and drop prejde bez toho aby bol oznaceny ako handled
    }

    // main private funcs
    // begins new frame
    void UIlayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        //glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::LoadIniSettingsFromMemory(); // from your own file

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

        RenderMainMenu();

        RenderMenu();

        RenderLayerManager();

        RenderViewports();

    }

    // deletes viewports with delete flag + render 2d,3d viewports that are not hidden
    void UIlayer::UpdateViewports()
    {
        m_viewports.erase(std::remove_if(m_viewports.begin(), m_viewports.end(),
            [](Referencer::Viewport* viewportPtr) {
                return viewportPtr != nullptr && !viewportPtr->isRunning();
            }), m_viewports.end());
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
                ImGui::VSliderFloat("idk", ImVec2(200, 200), &m_zoom, 0.0f, 10.0f);
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
                    std::string path = loadFileDialog("Referencer file (*.ref)\0*.ref\0*.png\0*.png\0*.jpg\0*.jpg\0*.obj\0*.obj\0");
                    if (path != "")
                    {
                        std::string name = "viewport(2D)_";
                        m_viewports.push_back(new Viewport2D(name + std::to_string(m_viewportIndex), true, path));
                        m_viewportIndex++;
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
                if (ImGui::MenuItem("Delete", "DEL")) {}
                if (ImGui::MenuItem("Hide", "H")) {}
                if (ImGui::MenuItem("Duplicate", "CTRL+D")) {}
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

    }

    void UIlayer::RenderLayerManager()
    {

        ImGuiStyle& style = ImGui::GetStyle();

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        //napad co keby som dal main window za jeden z viewportov a ked pohnes tak callback a pohnes glfw oknom aby fungoval drag&drop
        ImGui::Begin("Layer manager", (bool*)0, flags);
        ImGui::Text("Layer names");
        ImGui::SameLine(ImGui::GetWindowWidth() - 70.0f - ImGui::GetStyle().FramePadding.x * 2.0f);
        ImGui::Text("Visibility");
        ImGui::Separator();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { style.FramePadding.x, 4 });

        if (ImGui::BeginPopup("Rename")) {
            ImGui::InputText("New Name", m_renameBuffer, IM_ARRAYSIZE(m_renameBuffer));

            if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
                m_renameViewportPointer->getName() = m_renameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        for (int i = 0; i < m_viewports.size(); i++)
        {
            //ImGui::MenuItem(m_viewports[i]->getName().c_str(), NULL, &m_viewports[i]->isSelected());
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5));
            ImGui::Selectable(m_viewports[i]->getName().c_str(), &m_viewports[i]->isSelected(), ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, 23.0f));
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                m_renameViewportPointer = m_viewports[i];
                ImGui::OpenPopup("Rename");
                // problem meni sa meno a zaroven aj id takze si mysli ze nove okno eg. odznova
            }
            ImGui::PopStyleVar();

            ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f - ImGui::GetStyle().FramePadding.x * 2.0f);
            //ImGui::PushStyleVar(ImGuiStyleVar_Rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::Checkbox(("##" + std::to_string(m_viewports[i]->getID().get())).c_str(), &m_viewports[i]->isOpened());
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();

        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();


        ImGui::End();

    }

    void UIlayer::RenderViewports()
    {
        for (auto it = m_viewports.begin(); it != m_viewports.end(); it++)
        {
            if ((*it)->isOpened())
                (*it)->onUpdate(m_instantOffsetX, m_instantOffsetY, 1.0f, 1.0f); // tu treba passnut offsety a zoom

        }

    }
    bool UIlayer::handleDrops(DragAndDropEvent& e)
    {
        // funguje ale treba pridat error handling + podporu pre serializing
        std::string endings2d[] = { "png", "webp", "jpeg", "jpg", "bmp" };
        std::string endings3d[] = { "amf", "3ds", "ac", "ase", "assbin", "b3d", "bvh", "collada", "dxf", "csm",
                                    "hmp", "irrmesh", "iqm", "irr", "lwo", "lws", "m3d", "md2", "md3", "md5",
                                    "mdc", "mdl", "nff", "ndo", "off", "obj", "ogre", "opengex", "ply", "ms3d",
                                    "dob", "blend", "ifc", "xgl", "fbx", "q3d", "q3bsp", "raw", "sib", "smd",
                                    "stl", "terragen", "3d", "x", "x3d", "gltf", "3mf", "mmd"
        };

        const char** paths = e.getPaths();

        for (int i = 0; i < e.getCount(); i++)
        {
            std::string path = paths[i];
            int index = path.find_last_of('.');
            std::string ending = path.substr(index + 1);

            std::transform(ending.begin(), ending.end(), ending.begin(),
                [](unsigned char ch) { return std::tolower(static_cast<char>(std::tolower(static_cast<unsigned char>(ch)))); });

            for (std::string iter : endings2d)
            {
                if (iter == ending)
                {
                    std::string name = "viewport(2D) ";
                    m_viewports.push_back(new Viewport2D(name + std::to_string(m_viewportIndex), true, path));
                    m_viewportIndex++;
                    break;
                }
            }

            for (std::string iter : endings3d)
            {
                if (iter == ending)
                {
                    std::string name = "viewport(3D) ";
                    m_viewports.push_back(new Viewport3D(name + std::to_string(m_viewportIndex), true, path));
                    m_viewportIndex++;
                    break;
                }
            }

        }
        return true;
    }
    /*
    void UIlayer::RenderViewports()
    {
        for (int i = 0; i < m_viewports.size(); i++)
        {
            ImGui::MenuItem(m_viewports[i]->getName().c_str(), NULL, &m_viewports[i]->isOpened());
        }

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
        ImGui::Begin("Test");

        if (ImGui::BeginTable("table", 1, ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable))
        {
            for (int i = 0; i < m_viewports.size(); i++)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, style.CellPadding.y * 2)); // Fix
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 15, style.CellPadding.y }); // vyska riadku
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 12.5, 0 });

                ImGui::TableNextRow(0, .0f); // min row height
                ImGui::TableSetColumnIndex(0);
                //ImGui::SetNextItemAllowOverlap();
                //ImGui::MenuItem(m_viewports[i]->getName().c_str(), NULL, &m_viewports[i]->isSelected());
                ImGui::Selectable(m_viewports[i]->getName().c_str(), &m_viewports[i]->isSelected(), ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap);
                if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
                    ImGui::OpenPopup("MyPopup");
                }

                // Create the popup
                if (ImGui::BeginPopupModal("MyPopup")) {
                    ImGui::Text("You did it!");
                    static char buffer[256];
                    ImGui::InputText("New Name", buffer, sizeof(buffer));
                    if (ImGui::Button("OK")) {

                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2, style.ItemSpacing.y)); // Fix

                ImGui::SameLine(ImGui::GetWindowWidth() - 20.0f - ImGui::GetStyle().FramePadding.x * 2.0f);
                ImGui::Checkbox(("##" + std::to_string(m_viewports[i]->getID().get())).c_str(), &m_viewports[i]->isOpened());
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
    */

}
