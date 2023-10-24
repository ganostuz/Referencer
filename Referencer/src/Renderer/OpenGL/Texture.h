#pragma once

namespace Referencer {
    class Texture
    {
    private:
        unsigned int m_textureID;
        std::string m_filePath;// mozno sa zide idk
        int m_width, m_height, m_BPP; // m_BPP asi sa zide, neviem

    public:
        Texture(const std::string& path);
        ~Texture();

        void bind(unsigned int slot = 0) const;
        void unBind() const;

        inline int getWidth() const { return m_width; }
        inline int getHeight() const { return m_height; }
    };
}