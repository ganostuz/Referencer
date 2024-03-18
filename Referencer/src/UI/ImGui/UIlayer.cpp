#include "rfpch.h"
#include "UIlayer.h"
#include <imgui.h>
#include "events\KeyEvent.h"
#include "events\ApplicationEvents.h"
#include "events\MouseEvent.h"
#include "KeyCodes.h"
#include "Utilities.h"
#include "thirdParty\imgui_notify.h"

#include "imgui_internal.h"
#include "GLFW\glfw3.h"
#include "backends\imgui_impl_glfw.h"
#include "fonts\Roboto.h"

#define IMGUI_COLOR_FROM_GLMVEC4(color) ImVec4((color).x, (color).y, (color).z, (color).w)
#define ImVec4ToGlmVec4(color) glm::vec4((color).x, (color).y, (color).z, (color).w)

//#include "ImGui\imgui_impl_glfw.h"

namespace Referencer {

    UIlayer::UIlayer()
        : Layer("ImGui layer"), m_showMenu(true), m_zoom(1.0f), m_offsetX(0), m_offsetY(0), m_instantOffsetX(0), m_instantOffsetY(0), m_instantZoom(1.0f), m_viewportIndex(1), m_wasCopyed(false), m_renameBuffer(""), m_urlBuffer(""), m_outputPathBuffer(""), m_renameViewportPointer(nullptr), m_showSettings(false), m_showInternetDialog(false), m_save(false)
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
        
        io.IniFilename = NULL;
        io.LogFilename = NULL;

        Application& app = Application::getApplication();
        SettingsHolder& settings = app.getSettings();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());


        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        LoadSettingsToImGui(settings);

        // Setup Platform/Renderer bindings
        // Setup Platform/Renderer backends
        //glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
        io.Fonts->AddFontFromMemoryCompressedTTF(&Roboto_compressed_data, Roboto_compressed_size, 16.0f);
        ImGui::MergeIconsWithLatestFont(16.f, false);
        //io.Fonts->AddFontFromFileTTF("D:/dev/Referencer/Referencer/resources/fonts/Roboto-Medium.ttf", 15.0f);
        //ImFont* font1 = io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
    }

    void UIlayer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UIlayer::onUpdate()
    {
        
        Begin(); // begins new frame
         
        // vsetko pojde do handle_input ctrl n o shift s
        if (ImGui::IsKeyPressed(ImGuiKey_N))
        {
            m_viewports.clear();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_O))
        {
            std::string path = Utilities::loadFileDialog("*.png\0*.png\0*.jpg\0*.jpg\0");
            if (path != "")
            {
                std::string name = "viewport(2D)_";
                m_viewports.push_back(new Viewport2D(name + std::to_string(m_viewportIndex), true, path));
                m_viewportIndex++;
                /*try
                {
                    std::filesystem::create_symlink(path, "D:\\dev\\" + m_viewports[m_viewportIndex - 1]->getID().get());

                }
                catch (const std::exception& e)
                {
                    std::string idk = e.what();
                }
                */
                
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_S))
        {
            std::string path = Utilities::saveFileDialog("Referencer file (*.ref2d)\0.ref2d\0");
            if (path != "")
            {
                std::ofstream file(path + ".ref2d", std::ios::binary);
                for (int i = 0; i < m_viewports.size(); ++i)
                {
                    Viewport2D* viewport2DPtr = dynamic_cast<Viewport2D*>(m_viewports[i]);
                    if (viewport2DPtr != nullptr) {
                        // Successfully cast to Viewport2D, handle accordingly
                        viewport2DPtr->SerializeImage(file);
                        continue;
                    }


                }
                ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "File saved successfully!" });
            }

        }

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
            ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Viewport copyed successfully!" });
        }




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

    void UIlayer::serialize(toml::value& config)
    {
        for (int i = 0; i < m_viewports.size(); ++i)
        {
            //m_viewports[i]->serialize(config);
        }

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
        //ImGui::ShowDemoWindow();

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

        RenderMenu();

        ShowGlobalSettingsEditor(); // rename to render

        RenderInternetDialog();

        RenderLayerManager();

        RenderViewports();

        //render notification
        RenderNotifications();

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

    void UIlayer::LoadSettingsToImGui(const SettingsHolder& settings)
    {

        // Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();

        // toto popjde do global configu
        style.WindowRounding = 5.0f;
        style.ChildRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_WindowBg);

        // Headers
        colors[ImGuiCol_Header] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_Header);
        colors[ImGuiCol_HeaderHovered] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_HeaderHovered);
        colors[ImGuiCol_HeaderActive] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_HeaderActive);

        // Buttons
        colors[ImGuiCol_Button] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_Button);
        colors[ImGuiCol_ButtonHovered] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_ButtonHovered);
        colors[ImGuiCol_ButtonActive] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_ButtonActive);

        // Frame BG
        colors[ImGuiCol_FrameBg] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_FrameBg);
        colors[ImGuiCol_FrameBgHovered] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_FrameBgHovered);
        colors[ImGuiCol_FrameBgActive] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_FrameBgActive);

        // Tabs
        //colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        //colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        //colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        //colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        //colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_TitleBg);
        colors[ImGuiCol_TitleBgActive] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_TitleBgActive);
        colors[ImGuiCol_TitleBgCollapsed] = IMGUI_COLOR_FROM_GLMVEC4(settings.ImGuiCol_TitleBgCollapsed);
    }

    void UIlayer::SaveImGuiToSettings(SettingsHolder& settings)
    {
        const ImGuiStyle& style = ImGui::GetStyle();

        settings.ImGuiCol_WindowBg = ImVec4ToGlmVec4(style.Colors[ImGuiCol_WindowBg]);
        settings.ImGuiCol_Header = ImVec4ToGlmVec4(style.Colors[ImGuiCol_Header]);
        settings.ImGuiCol_HeaderHovered = ImVec4ToGlmVec4(style.Colors[ImGuiCol_HeaderHovered]);
        settings.ImGuiCol_HeaderActive = ImVec4ToGlmVec4(style.Colors[ImGuiCol_HeaderActive]);
        settings.ImGuiCol_Button = ImVec4ToGlmVec4(style.Colors[ImGuiCol_Button]);
        settings.ImGuiCol_ButtonHovered = ImVec4ToGlmVec4(style.Colors[ImGuiCol_ButtonHovered]);
        settings.ImGuiCol_ButtonActive = ImVec4ToGlmVec4(style.Colors[ImGuiCol_ButtonActive]);
        settings.ImGuiCol_FrameBg = ImVec4ToGlmVec4(style.Colors[ImGuiCol_FrameBg]);
        settings.ImGuiCol_FrameBgHovered = ImVec4ToGlmVec4(style.Colors[ImGuiCol_FrameBgHovered]);
        settings.ImGuiCol_FrameBgActive = ImVec4ToGlmVec4(style.Colors[ImGuiCol_FrameBgActive]);
        settings.ImGuiCol_TitleBg = ImVec4ToGlmVec4(style.Colors[ImGuiCol_TitleBg]);
        settings.ImGuiCol_TitleBgActive = ImVec4ToGlmVec4(style.Colors[ImGuiCol_TitleBgActive]);
        settings.ImGuiCol_TitleBgCollapsed = ImVec4ToGlmVec4(style.Colors[ImGuiCol_TitleBgCollapsed]);
    }

    void UIlayer::RenderMainMenu()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

        if (m_showMenu && ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "N"))
                {
                    m_viewports.clear();
                    ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Viewports erased." });
                }
                

                if (ImGui::MenuItem("Open", "O"))
                {
                    std::string path = Utilities::loadFileDialog("*.png\0*.png\0*.jpg\0*.jpg\0");
                    if (path != "")
                    {
                        std::string name = "viewport(2D)_";
                        m_viewports.push_back(new Viewport2D(name + std::to_string(m_viewportIndex), true, path));
                        m_viewportIndex++;
                        /*
                        try
                        {
                        std::filesystem::create_symlink(path, "D:\\dev\\" + m_viewports[m_viewportIndex-1]->getID().get());

                        }
                        catch (const std::exception& e)
                        {
                            std::string idk = e.what();
                        }*/
                        
                        
                        
                        //m_selected.push_back(false);
                    }
                    ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "file opened successfully!" });
                }
                if (ImGui::MenuItem("Save as", "S"))
                {
                   
                    std::string path = Utilities::saveFileDialog("Referencer file (*.ref2d)\0.ref2d\0");
                    if (path != "")
                    {
                        std::ofstream file(path + ".ref2d", std::ios::binary);
                        for (int i = 0; i < m_viewports.size(); ++i)
                        {
                            Viewport2D* viewport2DPtr = dynamic_cast<Viewport2D*>(m_viewports[i]);
                            if (viewport2DPtr != nullptr) {
                                viewport2DPtr->SerializeImage(file);
                                continue;
                            }
                            // handle saving 3D
                        }
                        ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "File Saved Sucessfully!" });
                    }
                }
                ImGui::MenuItem("Show settings", NULL, &m_showSettings);
                ImGui::MenuItem("Import from internet", NULL, &m_showInternetDialog);

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Shortcuts"))
            {
                ImGui::SeparatorText("window shortcuts");
                ImGui::MenuItem("Delete", "DEL", false, false);
                ImGui::MenuItem("Hide", "H", false, false);
                ImGui::MenuItem("Duplicate", "SHIFT + D", false, false);
                ImGui::SeparatorText("manager shortcuts");
                ImGui::MenuItem("New scene", "N", false, false);
                ImGui::MenuItem("Open", "O", false, false);
                ImGui::MenuItem("Save as", "S", false, false);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::PopStyleVar();
    }

    void UIlayer::RenderMenu()
    {
        //ImGui::ShowDemoWindow((bool *)true);
    }

    void UIlayer::RenderLayerManager()
    {

        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        //napad co keby som dal main window za jeden z viewportov a ked pohnes tak callback a pohnes glfw oknom aby fungoval drag&drop
        ImGuiWindowClass window_class1;
        window_class1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplit;
        ImGui::SetNextWindowClass(&window_class1);
        ImGui::Begin("Layer manager", (bool*)0, flags);

        RenderMainMenu();
        ImGui::Text("Layer names");
        ImGui::SameLine(ImGui::GetWindowWidth() - 70.0f - ImGui::GetStyle().FramePadding.x * 2.0f);
        ImGui::Text("Visibility");
        ImGui::Separator();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { style.FramePadding.x, 4 });

        
        if (ImGui::BeginPopup("Rename")) {
            if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                ImGui::SetKeyboardFocusHere(0);
                ImGui::InputText("New name", m_renameBuffer.data(), m_renameBuffer.capacity() + 1, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AutoSelectAll, TextEditCallback, &m_renameBuffer);


            if ((ImGui::Button("OK") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) && m_renameBuffer!="") {
                m_renameViewportPointer->getName() = m_renameBuffer;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        for (int i = 0; i < m_viewports.size(); i++)
        {
            //ImGui::MenuItem(m_viewports[i]->getName().c_str(), NULL, &m_viewports[i]->isSelected());
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5));
            ImGui::Selectable((m_viewports[i]->getName() + "###" + m_viewports[i]->getFullName()).c_str(), &m_viewports[i]->isSelected(), ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, 23.0f));
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                m_renameViewportPointer = m_viewports[i];
                ImGui::OpenPopup("Rename");
            }
            ImGui::PopStyleVar();

            ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f - ImGui::GetStyle().FramePadding.x * 2.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

            ImGui::Checkbox(("###" + std::to_string(m_viewports[i]->getID().get())).c_str(), &m_viewports[i]->isOpened());
            ImGui::PopStyleVar();

        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();


        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();

    }

    void UIlayer::RenderViewports()
    {

        for (auto it = m_viewports.begin(); it != m_viewports.end(); it++)
        {
            if ((*it)->isOpened())
                (*it)->onUpdate(m_instantOffsetX, m_instantOffsetY, 1.0f, 1.0f); // tu treba passnut offsety a zoom
        }

    }
    void UIlayer::ShowGlobalSettingsEditor() {
        if (!m_showSettings)
            return;
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(400, 300));
        ImGui::Begin("Global Settings Editor", &m_showSettings, ImGuiWindowFlags_NoSavedSettings);

        // Create a local reference to ImGui's style
        ImGuiStyle& style = ImGui::GetStyle();

        // ImGui colors
        ImGui::Text("Colors");
        ImGui::ColorEdit4("Window Bg", (float*)&style.Colors[ImGuiCol_WindowBg], ImGuiColorEditFlags_Float);
        ImGui::Separator();
        ImGui::ColorEdit4("Header", (float*)&style.Colors[ImGuiCol_Header], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Header Hovered", (float*)&style.Colors[ImGuiCol_HeaderHovered], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Header Active", (float*)&style.Colors[ImGuiCol_HeaderActive], ImGuiColorEditFlags_Float);
        ImGui::Separator();
        ImGui::ColorEdit4("Button", (float*)&style.Colors[ImGuiCol_Button], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Button Hovered", (float*)&style.Colors[ImGuiCol_ButtonHovered], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Button Active", (float*)&style.Colors[ImGuiCol_ButtonActive], ImGuiColorEditFlags_Float);
        ImGui::Separator();
        ImGui::ColorEdit4("Frame Bg", (float*)&style.Colors[ImGuiCol_FrameBg], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Frame Bg Hovered", (float*)&style.Colors[ImGuiCol_FrameBgHovered], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Frame Bg Active", (float*)&style.Colors[ImGuiCol_FrameBgActive], 
            ImGuiColorEditFlags_Float);
        ImGui::Separator();
        ImGui::ColorEdit4("Title Bg", (float*)&style.Colors[ImGuiCol_TitleBg], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Title Bg Active", (float*)&style.Colors[ImGuiCol_TitleBgActive], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Title Bg Collapsed", (float*)&style.Colors[ImGuiCol_TitleBgCollapsed], ImGuiColorEditFlags_Float);

        if (ImGui::Button("Save"))
        {
            SaveImGuiToSettings(Application::getApplication().getSettings());
        }
        ImGui::SameLine(50.0f);
        if (ImGui::Button("Load saved"))
        {
            LoadSettingsToImGui(Application::getApplication().getSettings());
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
    void UIlayer::RenderInternetDialog()
    {
        if (!m_showInternetDialog)
            return;
        
        ImGui::Begin("Internet importer", &m_showInternetDialog, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

        const char* formats[] = { "obj", "stl", "png", "jpg", "jpeg"};
        static const char* current_item = formats[0]; // to class

        ImGui::InputText("Import url", m_urlBuffer.data(), m_urlBuffer.capacity() + 1, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AutoSelectAll, TextEditCallback, &m_urlBuffer);
        ImGui::Checkbox("import & save", &m_save);
        ImGui::BeginDisabled(!m_save);
        ImGui::InputText("Output path", m_outputPathBuffer.data(), m_outputPathBuffer.capacity() + 1, ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AutoSelectAll, TextEditCallback, &m_outputPathBuffer);
        ImGui::SameLine();
        if (ImGui::ArrowButton("##", ImGuiDir_Right))
            m_outputPathBuffer = Utilities::openFolderDialog().c_str();
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted("search in file explorer.");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        ImGui::EndDisabled();
        if (ImGui::BeginCombo("select format", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(formats); n++)
            {
                bool is_selected = (current_item == formats[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(formats[n], is_selected))
                    current_item = formats[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }
        if (ImGui::Button("Download"))
        {
            std::string path(m_outputPathBuffer + "\\23453453." + current_item);
            if (Utilities::downloadFile(m_urlBuffer, path))
                ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Download successfull!" });
            else
                ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Error while downloading!" });

            std::string name = "viewport(2D) ";
            m_viewports.push_back(new Viewport2D(name + std::to_string(m_viewportIndex), true, path));

            ++m_viewportIndex;

            if (!m_save)
            {
                remove((m_outputPathBuffer + "\\23453453." + current_item).c_str());
            }
        }
        ImGui::End();
    }
    void UIlayer::RenderNotifications()
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
        ImGui::RenderNotifications(); 
        ImGui::PopStyleColor(1);
    }
    bool UIlayer::handleDrops(DragAndDropEvent& e)
    {
        // funguje ale treba pridat error handling + podporu pre serializing
        std::string endings2d[] = { "png", "webp", "jpeg", "jpg", "bmp", "psd"};
        std::string endings3d[] = { "amf", "3ds", "ac", "ase", "assbin", "b3d", "bvh", "collada", "dxf", "csm",
                                    "hmp", "irrmesh", "iqm", "irr", "lwo", "lws", "m3d", "md2", "md3", "md5",
                                    "mdc", "mdl", "nff", "ndo", "off", "obj", "ogre", "opengex", "ply", "ms3d",
                                    "dob", "ifc", "xgl", "fbx", "q3d", "q3bsp", "raw", "sib", "smd",
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

            if (ending == "ref2d")
            {
                std::ifstream file(path, std::ios::binary);
                if (!file) { // ma to problem s utf 8
                    std::cerr << "Error opening file: " << path << std::endl;
                    ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Error while loading files!" });
                    return false;
                }

                int width, height;

                while (true) {
                    // Read the width and height
                    if (!file.read(reinterpret_cast<char*>(&width), sizeof(width))) break;
                    if (!file.read(reinterpret_cast<char*>(&height), sizeof(height))) break;

                    // Allocate memory for the image data
                    unsigned char* pixels = new unsigned char[width * height * 4]; // Assuming  4 bytes per pixel (RGBA)

                    // Read the pixel data
                    if (!file.read(reinterpret_cast<char*>(pixels), width * height * 4)) {
                        delete[] pixels; // Free the allocated memory if reading fails
                        break;
                    }

                    // Create a new ImageData instance
                    std::shared_ptr<ImageData> imgData = std::make_shared<ImageData>(width, height, pixels);

                    // Create a new Viewport2D instance
                    std::string name = "viewport(2D) ";
                    m_viewports.push_back(new Viewport2D(name + std::to_string(m_viewportIndex), true, path,10,10,imgData));

                    ++m_viewportIndex;
                }
            }
            
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
            // hanle better
            ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Files loaded successfully!" });

        }
        return true;
    }
    int UIlayer::TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            // Resize string callback
            std::string* str = (std::string*)data->UserData;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        return 0;
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
