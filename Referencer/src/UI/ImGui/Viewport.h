#pragma once
#include <string>
#include "events\Event.h"
#include "ID.h"

namespace Referencer {
	class Viewport
	{
	private:
		bool m_isOpen, m_running, m_selected; // proste na komunikaciu s layout managerom
		ID m_id;
		std::string m_name;
	public:
		 Viewport(std::string name, bool isOpen)
			 :m_isOpen(isOpen), m_name(name), m_running(true), m_selected(false) {}

		virtual void onUpdate(int offsetX, int offsetY, float instantZoom, float totalZoom) = 0;
		virtual void onEvent(Event& e) = 0;

		inline std::string& getName() { return m_name; }
		inline std::string getFullName() { return m_name + std::string("##") + std::to_string(m_id.get()); }
		inline bool& isOpened() { return m_isOpen; }
		inline void setOpened(bool open) { m_isOpen = open; }
		inline bool& isRunning() { return m_running; }
		inline bool setRunning(bool terminate) { return m_running = terminate; }
		inline bool& isSelected() { return m_selected; }
		inline void setSelected(bool select) { m_selected = select;  }
		inline ID getID() { return m_id; }

		//inline bool& isOpen() { return *m_isOpen; }


	};
}
