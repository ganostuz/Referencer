#pragma once
#define CURL_STATICLIB
#define GLFW_EXPOSE_NATIVE_WIN32 

#include <string>
#include "Debug.h"
#include "Application.h"

#include <Windows.h>
#include <ShObjIdl.h>
#include <atlbase.h>

#include <GLFW/glfw3.h>
#include "GLFW\glfw3native.h"
#include "curl\curl.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Referencer {

	class Utilities
	{
	public:
        
        static inline void inspect(const int width,
            const int height,
            const unsigned char* const bits,
            ImVec2 mouseUVCoord,
            ImVec2 displayedTextureSize)
        {
            ImGui::BeginTooltip();
            ImGui::BeginGroup();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // settings
            const float zoomRectangleWidth = 160.f;
            const int zoomSize = 4;

            // bitmap zoom
            ImGui::InvisibleButton("AnotherInvisibleMan", ImVec2(zoomRectangleWidth, zoomRectangleWidth));
            const ImRect pickRc(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
            draw_list->AddRectFilled(pickRc.Min, pickRc.Max, 0xFF000000);
            const float quadWidth = zoomRectangleWidth / float(zoomSize * 2 + 1);
            const ImVec2 quadSize(quadWidth, quadWidth);
            const int basex = ImClamp(int(mouseUVCoord.x * width), zoomSize, width - zoomSize);
            const int basey = ImClamp(int(mouseUVCoord.y * height), zoomSize, height - zoomSize);
            for (int y = -zoomSize; y <= zoomSize; y++)
            {
                for (int x = -zoomSize; x <= zoomSize; x++)
                {
                    uint32_t texel = ((uint32_t*)bits)[(basey - (y*-1)) * width + x + basex];
                    ImVec2 pos = pickRc.Min + ImVec2(float(x + zoomSize), float(y + zoomSize)) * quadSize;
                    draw_list->AddRectFilled(pos, pos + quadSize, texel);

                    //draw_list->AddRectFilled(pos, pos + quadSize, texel);
                }
            }
            ImGui::SameLine();

            // center quad
            const ImVec2 pos = pickRc.Min + ImVec2(float(zoomSize), float(zoomSize)) * quadSize;
            draw_list->AddRect(pos, pos + quadSize, 0xFF0000FF, 0.f, 15, 2.f);

            // normal direction
            ImGui::InvisibleButton("AndOneMore", ImVec2(zoomRectangleWidth, zoomRectangleWidth));
            ImRect normRc(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
            for (int y = -zoomSize; y <= zoomSize; y++)
            {
                for (int x = -zoomSize; x <= zoomSize; x++)
                {
                    uint32_t texel = ((uint32_t*)bits)[(basey - (y * -1)) * width + x + basex];
                    const ImVec2 posQuad = normRc.Min + ImVec2(float(x + zoomSize), float(y + zoomSize)) * quadSize;
                    //draw_list->AddRectFilled(pos, pos + quadSize, texel);
                    const float nx = float(texel & 0xFF) / 128.f - 1.f;
                    const float ny = float((texel & 0xFF00) >> 8) / 128.f - 1.f;
                    const ImRect rc(posQuad, posQuad + quadSize);
                    draw_list->AddCircle(rc.GetCenter(), rc.GetWidth() / 2.f, 0x20AAAAAA, 24, 1.f);
                    draw_list->AddCircle(rc.GetCenter(), rc.GetWidth() / 4.f, 0x20AAAAAA, 24, 1.f);
                    draw_list->AddLine(rc.GetCenter(), rc.GetCenter() + ImVec2(nx, ny) * rc.GetWidth() / 2.f, 0xFF0000FF, 2.f);
                }
            }



            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            uint32_t texel = ((uint32_t*)bits)[(basey) * width + basex];
            //uint32_t texel = ((uint32_t*)bits)[(basey - zoomSize * 2 - 1) * width + basex];
            ImVec4 color = ImColor(texel);
            ImVec4 colHSV;
            ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z, colHSV.x, colHSV.y, colHSV.z);
            ImGui::Text("U %1.3f V %1.3f", mouseUVCoord.x, mouseUVCoord.y);
            ImGui::Text("Coord %d %d", int(mouseUVCoord.x * width), int(mouseUVCoord.y * height));
            ImGui::Separator();
            ImGui::Text("R 0x%02x  G 0x%02x  B 0x%02x", int(color.x * 255.f), int(color.y * 255.f), int(color.z * 255.f));
            ImGui::Text("R %1.3f G %1.3f B %1.3f", color.x, color.y, color.z);
            ImGui::Separator();
            ImGui::Text(
                "H 0x%02x  S 0x%02x  V 0x%02x", int(colHSV.x * 255.f), int(colHSV.y * 255.f), int(colHSV.z * 255.f));
            ImGui::Text("H %1.3f S %1.3f V %1.3f", colHSV.x, colHSV.y, colHSV.z);
            ImGui::Separator();
            ImGui::Text("Alpha 0x%02x", int(color.w * 255.f));
            ImGui::Text("Alpha %1.3f", color.w);
            ImGui::Separator();
            ImGui::Text("Size %d, %d", int(displayedTextureSize.x), int(displayedTextureSize.y));
            ImGui::EndGroup();
            //histogram(width, height, bits);
            ImGui::EndTooltip();
        }
        // open native dialog window for saving content
        static std::string saveFileDialog(const char* filter)
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
        // open native dialog window for loading content
        static std::string loadFileDialog(const char* filter)
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
        // open native dialog window for choosing directory
        static std::string openFolderDialog()
        {
            std::wstring selectedFolderPath;
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                CComPtr<IFileDialog> pfd;
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
                if (SUCCEEDED(hr))
                {
                    // Set the options to select folders
                    DWORD dwOptions;
                    hr = pfd->GetOptions(&dwOptions);
                    if (SUCCEEDED(hr))
                    {
                        hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
                        if (SUCCEEDED(hr))
                        {
                            // Show the dialog
                            hr = pfd->Show(NULL);
                            if (SUCCEEDED(hr))
                            {
                                CComPtr<IShellItem> psi;
                                hr = pfd->GetResult(&psi);
                                if (SUCCEEDED(hr))
                                {
                                    PWSTR pszPath = NULL;
                                    hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Use the selected folder path
                                        selectedFolderPath = pszPath;
                                        CoTaskMemFree(pszPath);
                                    }
                                }
                            }
                        }
                    }
                }
                CoUninitialize();
            }
            return std::string(selectedFolderPath.begin(), selectedFolderPath.end());
        }
		
        static bool downloadFile(const std::string& url, const std::string& outputPath) {
            CURL* curl = curl_easy_init();
            if (!curl) {
                std::cerr << "Failed to initialize libcurl." << std::endl;
                return false;
            }

            FILE* fp = fopen(outputPath.c_str(), "wb");
            if (!fp) {
                std::cerr << "Failed to open file for writing." << std::endl;
                curl_easy_cleanup(curl);
                return false;
            }

            // Define the lambda function for the write callback
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
                fclose(fp);
                curl_easy_cleanup(curl);
                return false;
            }

            fclose(fp);
            curl_easy_cleanup(curl);
            return true;
        }
    private:
        static size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
            size_t written = fwrite(ptr, size, nmemb, stream);
            return written;
        }
        
	};
}