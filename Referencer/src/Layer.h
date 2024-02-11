#pragma once

#include "Events/Event.h"
#include "toml.hpp"

namespace Referencer {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() 
		{
		
		
		
		}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void serialize(toml::value& config) {}
		//virtual void deSerialize() {}
		virtual void onEvent(Event& event) {}
		inline const std::string& getName() const { return m_name; }
		inline const void setName(std::string name)  {  m_name = name; }
		//inline const bool getHidden() { return m_hidden; }
		//inline const void setHidden(bool hidden) { m_hidden = hidden; }
	protected:
		std::string m_name;
		//bool m_hidden;
		//int m_width, m_height, m_posX, m_posY; // implement getters and setters will be needed
		// impplement in image class
	};

}