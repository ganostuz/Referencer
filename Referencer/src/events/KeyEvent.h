#pragma once
#include "Event.h"

namespace Referencer {
	class KeyEvent : public Event // toto sa da dat prec aby potom tahali iba z event classy
	{
	protected:
		int m_keycode;
		KeyEvent(int keycode)
			: m_keycode(keycode) {}
	public:
		int getEventCategory() const override { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; };
		inline int getKeyCode() const { return m_keycode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeyEvent] keyCode: " << m_keycode;
			return ss.str();
		}
	};

	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_repeatcount;
	public:
		KeyPressedEvent(int keycode, int repeatCount) // implement repeat count
			: KeyEvent(keycode), m_repeatcount(repeatCount) {}

		inline int getRepeatCount() { return m_repeatcount; }

		static EventType getStaticType() { return EventType::Keypressed; }
		EventType getEventType() const override { return getStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ KeyPressedEvent ] keyCode: " << m_keycode << " repeatCount: " << m_repeatcount;
			return ss.str();

		}
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		static EventType getStaticType() { return EventType::KeyReleased; }
		EventType getEventType() const override { return getStaticType(); }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeyReleasedEvent] keyCode: " << m_keycode;
			return ss.str();
		}
	};
}
