#include "rfpch.h"

#include "Viewport3D.h"
#include "Viewport.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include "thirdParty\imgui_notify.h"


namespace Referencer {

	Viewport3D::Viewport3D(std::string name, bool isOpen, std::string path)
		:Viewport(name, isOpen), m_firstMouse(true), m_width(400), m_modelSource(path), m_height(400), m_shader("D:/dev/Referencer/Referencer/resources/modelShader.vertex", "D:/dev/Referencer/Referencer/resources/modelShader.fragment", true), m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)), m_model(path), m_first_time(true), m_scale(1.0f), m_lightColor(1.0f, 1.0f, 1.0f), m_lightStrength(1), m_lightPos(2.0f,2.0f,2.0f), m_objectColor(0.5f, 0.5f, 0.5f), m_translate(0.0f, 0.0f, 0.0f), m_interpolation(0.0f), m_showSettings(false), m_showShaderEditor(false)
	{
		// pridaj set shaders odstran zavislost od dependencies
		m_fragment = m_shader.getFragmentShader();
		m_vertex = m_shader.getVertexShader();
		genBuffers();
		m_hasTexture = m_model.textures_loaded.size() > 0;
		m_center = m_model.GetCenter();
	}
	Viewport3D::Viewport3D(const Viewport3D& other)
		: Viewport(other), m_firstMouse(true), m_width(other.m_width), m_modelSource(other.m_modelSource), m_height(other.m_height), m_shader(), m_camera(other.m_camera), m_model(other.m_modelSource), m_first_time(other.m_first_time), m_scale(other.m_scale), m_lightStrength(other.m_lightStrength), m_interpolation(other.m_interpolation), m_showSettings(false), m_showShaderEditor(false)
	{
		m_shader.setShaders(other.m_shader.getVertexShader(), other.m_shader.getFragmentShader());
		m_fragment = m_shader.getFragmentShader();
		m_vertex = m_shader.getVertexShader();
		m_objectColor = other.m_objectColor;
		m_translate = other.m_translate;
		m_lightPos = other.m_lightPos;
		m_lightColor = other.m_lightColor;
		m_center = other.m_center;
	}

	int Viewport3D::getType()
	{
		return 1;
	}

	Viewport* Viewport3D::clone()
	{
		return new Viewport3D(*this);
	}

	void Viewport3D::handleInput()
	{
		setSelected(true);
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
		{
			ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Viewport deleted" });
			setRunning(false);
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
		{
			ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Viewport hidden" });
			setOpened(false);
		}
		if (ImGui::IsKeyDown(ImGuiKey_MouseRight))
			ImGui::OpenPopup("my_select_popup");

		// mouse positions & click
		if (ImGui::IsMouseDown(2))
		{
			ImVec2 currPos = ImGui::GetMousePos();

			if (!m_firstMouse)
			{
				m_camera.Rotate(glm::vec2(m_lastMouse.x, m_lastMouse.y), glm::vec2(currPos.x, currPos.y), glm::vec2(m_width, m_height));
			}
			m_lastMouse.x = currPos.x;
			m_lastMouse.y = currPos.y;
			m_firstMouse = false;
		}
		else
		{
			m_firstMouse = true;
		}

		// wheel scroll
		ImGuiIO& io = ImGui::GetIO();

		// Check if the mouse wheel was scrolled
		if (io.MouseWheel != 0.0f)
		{
			// Call your Zoom function with the mouse wheel delta
			m_camera.Zoom(io.MouseWheel);
		}
	}


	void Viewport3D::genBuffers()
	{
		glGenFramebuffers(1, &m_fbo);
		glGenTextures(1, &m_texture);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);  // width and height
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "FRAMEBUFFER NOT DONE" << std::endl; // error handling
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Viewport3D::deleteBuffers()
	{
		glDeleteTextures(1, &m_texture);
		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteFramebuffers(1, &m_fbo);
	}

	void Viewport3D::renderImGuiModelSpecs()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x+10, ImGui::GetMousePos().y + 10), ImGuiCond_FirstUseEver);
		if (ImGui::Begin((this->getName() + " settings###settings" + std::to_string(getID().get())).c_str(), &m_showSettings, windowFlags))
		{
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
				setRunning(false);
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
				setOpened(false);

			ImGui::SeparatorText("color");
			ImGui::ColorEdit3("Light color", &m_lightColor.x);
			ImGui::ColorEdit3("Object color", &m_objectColor.x);
			ImGui::SeparatorText("light position");
			ImGui::SliderFloat("LightPosX", &m_lightPos.x, -10.f, 10.f, "%.2f");
			ImGui::SliderFloat("LightPosY", &m_lightPos.y, -10.f, 10.f, "%.2f");
			ImGui::SliderFloat("LightPosZ", &m_lightPos.z, -10.f, 10.f, "%.2f");
			ImGui::SeparatorText("object position");
			ImGui::SliderFloat("ObjectPosX", &m_translate.x, -10.f, 10.f, "%.2f");
			ImGui::SliderFloat("ObjectPosY", &m_translate.y, -10.f, 10.f, "%.2f");
			ImGui::SliderFloat("ObjectPosZ", &m_translate.z, -10.f, 10.f, "%.2f");
			ImGui::SeparatorText("scaling");
			ImGui::SliderFloat("Scale Object", &m_scale, 0.f, 10.f, "%.3f");
			ImGui::BeginDisabled(!m_hasTexture);
			ImGui::SliderFloat("Material vs Texture", &m_interpolation, 0, 1.f, "%.3f");
			ImGui::EndDisabled();

			
		}
		ImGui::End();

		

	}

	void Viewport3D::renderImGuiModel()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoScrollbar;// todo: handle it normalne by si scrollbar ani scrolling zakazovat nemusel
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);

		if (ImGui::Begin((this->getName() + "###" + std::to_string(getID().get())).c_str(), &isRunning(), windowFlags))
		{
			if (ImGui::IsWindowHovered())
				handleInput();
			else
				setSelected(false);
				
			if (m_width != ImGui::GetWindowSize().x || m_height != ImGui::GetWindowSize().y)
			{
				m_width = ImGui::GetWindowSize().x;
				m_height = ImGui::GetWindowSize().y;

				// resize
				//deleteBuffers();
				//genBuffers();
			}

			ImGui::Image((void*)(intptr_t)m_texture, ImVec2{ (float)m_width, (float)m_height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.5f, 7.5f));
			if (ImGui::BeginPopup("my_select_popup"))
			{
				ImGui::MenuItem("Opened", "Del", &isRunning());
				ImGui::MenuItem("Visible", "H", &isOpened());
				ImGui::MenuItem("Show settings", "idk", &m_showSettings);
				ImGui::MenuItem("Show shader editor", "idk", &m_showShaderEditor);
				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
			
			
		}
		ImGui::End();
		ImGui::PopStyleVar();
		
	}

	void Viewport3D::renderModel()
	{

		glViewport(0, 0, m_width, m_height);
		m_shader.bind();

		// view/projection transformations
		glm::vec3 cameraPosition = m_camera.GetEye();

		glm::mat4 projection = glm::perspective(glm::radians(30.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
		glm::mat4 view = m_camera.GetViewMatrix();
		
		//camera & control
		m_shader.setMat4("projection", projection);
		m_shader.setMat4("view", view);
		m_shader.setUniform3f("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		m_shader.setUniform1f("interpolation", m_interpolation);


		// all values below to material struct + make light class with rendering, position, color, etc.
		// light
		m_shader.setUniform3f("light.ambient", m_lightColor[0] * 0.2, m_lightColor[1] * 0.2, m_lightColor[2] * 0.2);
		m_shader.setUniform3f("light.diffuse", m_lightColor[0] * 0.5f, m_lightColor[1] * 0.5f, m_lightColor[2] * 0.5f); // darken diffuse light a bit
		m_shader.setUniform3f("light.specular", m_lightColor[0], m_lightColor[1], m_lightColor[2]);
		m_shader.setUniform3f("light.position", m_lightPos[0], m_lightPos[1], m_lightPos[2]);

		//material
		m_shader.setUniform3f("material.ambient", m_objectColor[0], m_objectColor[1], m_objectColor[2]);
		m_shader.setUniform3f("material.diffuse", m_objectColor[0], m_objectColor[1], m_objectColor[2]);
		m_shader.setUniform3f("material.specular", m_objectColor[0] * 0.6f, m_objectColor[1] * 0.6f, m_objectColor[2] * 0.6f);
		m_shader.setUniform1f("material.shininess", 32.0f);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, -m_center);
		model = glm::translate(model, glm::vec3(m_translate[0], m_translate[1], m_translate[2])); // translate it down
		model = glm::scale(model, glm::vec3(m_scale));	// it's a bit too big for our scene, so scale it down
		m_shader.setMat4("model", model);
		m_model.Draw(m_shader);

		m_shader.unBind();
	}

	void Viewport3D::onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom)
	{
		// fuj ale funguje
		deleteBuffers();
		genBuffers();
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderModel();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (m_showSettings)
			renderImGuiModelSpecs();

		if (m_showShaderEditor)
			renderShaderEditor();
		renderImGuiModel();
	}

	void Viewport3D::onEvent(Event& e)
	{
	}

	Viewport3D::~Viewport3D()
	{
		deleteBuffers();
	}

	void Viewport3D::computeLightPos(float distance, glm::vec2 deltaAngleRadians)
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), deltaAngleRadians.x, glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, deltaAngleRadians.y, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 newLightPosition = glm::vec3(rotationMatrix * glm::vec4(m_lightPos[0], m_lightPos[1], m_lightPos[2], 1.0f));

		// Ensure the light maintains its distance from the center
		glm::vec3 lightPos = glm::normalize(newLightPosition) * distance;
		m_lightPos[0] = lightPos.x;
		m_lightPos[1] = lightPos.y;
		m_lightPos[2] = lightPos.z;
	}

	void Viewport3D::renderShaderEditor()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x + 10, ImGui::GetMousePos().y + 10), ImGuiCond_FirstUseEver);
		if (ImGui::Begin((this->getName() + " shader editor###shaderEditor" + std::to_string(getID().get())).c_str(), &m_showShaderEditor, windowFlags))
		{
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
				setRunning(false);
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)))
				setOpened(false);

			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F5)))
			{
				ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Shader compiled" });
				m_shader.setShaders("", m_fragment);
			}

			// this will be shader editor
			ImGui::InputTextMultiline("##", m_fragment.data(), m_fragment.capacity() + 1, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AutoSelectAll, TextEditCallback, &m_fragment);

		}
		ImGui::End();
	}

	int Viewport3D::TextEditCallback(ImGuiInputTextCallbackData* data)
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



}