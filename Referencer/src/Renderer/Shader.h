#pragma once
#include "glad\glad.h"
#include <glm/glm.hpp>

namespace Referencer {

    class Shader
    {
    private:
        GLuint m_programID = 0;
        std::string m_vertex, m_fragment;
        std::unordered_map<std::string, int> m_uniformLocationCache;

        int getUniformLocation(const std::string& name);
    public:
        Shader(const std::string& vertex, const std::string& fragment, bool isPath);
        Shader();
        ~Shader();

        void bind() const;
        void unBind() const;

        inline const std::string& getVertexShader() const { return m_vertex; }
        inline const std::string& getFragmentShader() const { return m_fragment; }

        // uniforms, add glm
        void setUniform1f(const std::string& name, float value);
        void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
        void setUniform3f(const std::string& name, float f0, float f1, float f2);
        void setMat4(const std::string& name, const glm::mat4& mat);
        void setShaders(const std::string& vertex = "", const std::string& fragment = "", bool isPath = false);
        //void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

        unsigned int getID() { return m_programID; }
    };
}