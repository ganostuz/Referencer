#pragma once

#include "Event.h"

namespace Referencer {

	class MouseMovedEvent : public Event
	{
	private:
		float m_posX, m_posY;
	public:
		MouseMovedEvent(float posX, float posY)
			: m_posX(posX), m_posY(posY){}

		float getX() { return m_posX; }
		float getY() { return m_posY; }

		int getEventCategory() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; };
		static EventType getStaticType() { return EventType::MouseMoved; }
		EventType getEventType() const override { return getStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ MouseScrolledEvent ] posX: " << m_posX << " posY: " << m_posY;
			return ss.str();
		}
	};

	class MouseScrolledEvent : public Event
	{
	private:
		float m_offsetX, m_offsetY;
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_offsetX(offsetX), m_offsetY(offsetY){}

		float getOffsetX() { return m_offsetX; }
		float getOffsetY() { return m_offsetY; }

		int getEventCategory() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; };
		static EventType getStaticType() { return EventType::MouseScrolled; }
		EventType getEventType() const override { return getStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ MouseScrolledEvent ] xoffset: " << m_offsetX << " yoffset: " << m_offsetY;
			return ss.str();
		}
	};

	class MouseButtonPressed : public Event 
	{
	private:
		int m_button;
	public:
		MouseButtonPressed(int button)
			: m_button(button){}
		int getMouseButton() { return m_button; }

		int getEventCategory() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; };
		static EventType getStaticType() { return EventType::MouseButtonPressed; }
		EventType getEventType() const override { return getStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ MouseButtonPressed ] button: " << m_button;
			return ss.str();
		}

	};

	class MouseButtonReleased : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonReleased(int button)
			: m_button(button) {}
		int getMouseButton() { return m_button; }

		int getEventCategory() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; };
		static EventType getStaticType() { return EventType::MouseButtonReleased; }
		EventType getEventType() const override { return getStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ MouseButtonReleased ] button: " << m_button;
			return ss.str();
		}

	};

}
