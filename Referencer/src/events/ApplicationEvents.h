#pragma once

#include "Event.h"

namespace Referencer {
	class WindowResizedEvent : public Event
	{
	private:
		unsigned int m_width, m_height;
	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height) {}

		unsigned int getWidth() { return m_width; }
		unsigned int getHeight() { return m_height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ WindowResizedEvent ] width: " << m_width << " height: " << m_height;
			return ss.str();
		}

		int getEventCategory() const override { return  EventCategory::EventCategoryApplication; };
		static EventType getStaticType() { return EventType::WindowResize; } // if you want to know which category class is without object
		EventType getEventType() const override { return getStaticType(); }
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}

		std::string ToString() const override
		{
			return "[ WindowCloseEvent ]";
		}


		int getEventCategory() const override { return  EventCategory::EventCategoryApplication; };
		static EventType getStaticType() { return EventType::WindowClose; }
		EventType getEventType() const override { return getStaticType(); }
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent() {}

		std::string ToString() const override
		{
			return "[ WindowMovedEvent ]";
		}

		int getEventCategory() const override { return  EventCategory::EventCategoryApplication; };
		static EventType getStaticType() { return EventType::WindowMoved; }
		EventType getEventType() const override { return getStaticType(); }
	};

}
