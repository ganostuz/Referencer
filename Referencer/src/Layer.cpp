#include "rfpch.h"
#include "Layer.h"

namespace Referencer {
	Layer::Layer(const std::string& name, int width, int height, int posX, int posY, bool hidden)
		: m_name(name)
		//: m_name(name), m_hidden(hidden), m_width(width), m_height(height), m_posX(posX), m_posY()
	{
	}

	Layer::~Layer()
	{

	}
}