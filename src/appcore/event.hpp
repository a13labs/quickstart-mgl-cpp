/*
   Copyright 2020 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#include "base.hpp"
#include "input.hpp"
#include "instrumentor.hpp"
#include "log.hpp"

namespace AppCore
{
	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled,
		GamepadButtonPressed,
		GamepadButtonReleased,
		GamepadAxisMoved
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryGamepad = BIT(5),
		EventCategoryGamepadButton = BIT(6)
	};

#define EVENT_CLASS_TYPE(type)                                                                                                                       \
	static EventType GetStaticType()                                                                                                                 \
	{                                                                                                                                                \
		return EventType::type;                                                                                                                      \
	}                                                                                                                                                \
	virtual EventType GetEventType() const override                                                                                                  \
	{                                                                                                                                                \
		return GetStaticType();                                                                                                                      \
	}                                                                                                                                                \
	virtual const char* GetName() const override                                                                                                     \
	{                                                                                                                                                \
		return #type;                                                                                                                                \
	}

#define EVENT_CLASS_CATEGORY(category)                                                                                                               \
	virtual int GetCategoryFlags() const override                                                                                                    \
	{                                                                                                                                                \
		return category;                                                                                                                             \
	}

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsCategory(EventCategory category) { return GetCategoryFlags() & category; }

		inline bool IsHandled() const { return mHandled; }

		inline void SetHandled(bool handled) { mHandled = handled; }

	protected:
		bool mHandled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: mEvent(event)
		{ }

		// F will be deduced by the compiler
		template <typename T, typename F>
		bool Dispatch(const F& func)
		{
			if(mEvent.GetEventType() == T::GetStaticType())
			{
				mEvent.SetHandled(func(static_cast<T&>(mEvent)));
				return true;
			}
			return false;
		}

	private:
		Event& mEvent;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: mWidth(width)
			, mHeight(height)
		{ }

		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32_t mWidth, mHeight;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: mMouseX(x)
			, mMouseY(y)
		{ }

		float GetX() const { return mMouseX; }
		float GetY() const { return mMouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mMouseX, mMouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: mXOffset(xOffset)
			, mYOffset(yOffset)
		{ }

		float GetXOffset() const { return mXOffset; }
		float GetYOffset() const { return mYOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mXOffset, mYOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton::Enum GetMouseButton() const { return mButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
	protected:
		MouseButtonEvent(MouseButton::Enum button)
			: mButton(button)
		{ }

		MouseButton::Enum mButton;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton::Enum button)
			: MouseButtonEvent(button)
		{ }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton::Enum button)
			: MouseButtonEvent(button)
		{ }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class KeyEvent : public Event
	{
	public:
		Key::Enum GetKeyCode() const { return mKeyCode; }
		uint8_t GetKeyModifiers() const { return mModifiers; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(Key::Enum keycode, uint8_t modifiers)
			: mKeyCode(keycode)
			, mModifiers(modifiers)
		{ }

		Key::Enum mKeyCode;
		uint8_t mModifiers;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(Key::Enum keycode, uint8_t modifiers, int repeat)
			: KeyEvent(keycode, modifiers)
			, mRepeat(repeat)
		{ }

		int GetRepeatCount() const { return mRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " ( repeat:" << mRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool mRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key::Enum keycode, uint8_t modifiers)
			: KeyEvent(keycode, modifiers)
		{ }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(Key::Enum keycode, uint8_t modifiers)
			: KeyEvent(keycode, modifiers)
		{ }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
} // namespace AppCore