#pragma once


namespace Referencer {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowMoved,
		Keypressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{ // for multiple categories
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput       = 1 << 1,
		EventCategoryKeyboard    = 1 << 2,
		EventCategoryMouse       = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

	class Event
	{
	private:
		bool m_handled = false;
	public:
		virtual EventType getEventType() const = 0;
		virtual int getEventCategory() const = 0; // probably return full int and searching for bits is on client
		virtual std::string ToString() const = 0; // debug only
		bool isInCategory(EventCategory e) { return getEventCategory() & e; }

		inline bool isHandled() { return m_handled; }
		inline void setHandled(bool handled) { m_handled = handled; }

	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	class EventDispatcher
	{
	private:
		Event& m_Event;
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (m_Event.getEventType() == T::getStaticType())
			{
				m_Event.setHandled(func(*(T*)&m_Event)); // nie som si isty ci bude dobre ukazovat
				return true;
			}
			return false;
		}
	};



}