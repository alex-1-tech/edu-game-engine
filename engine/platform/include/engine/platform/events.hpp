#pragma once

/**
 * @file    events.hpp
 * @brief   Event system for handling window, input, and application events
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/base.hpp"
#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

/**
 * @brief Event types enumeration
 *
 * Defines all possible event types that can be handled by the event system.
 * Events are categorized into window events, input events, and other application events.
 */
enum class EventType : u8 {
  WINDOW_CLOSE = 0, ///< Window close request event
  WINDOW_RESIZE,    ///< Window resize event
  KEY_PRESSED,      ///< Key pressed event
  KEY_RELEASED      ///< Key released event
};

/**
 * @brief Base event class
 *
 * All specific event types inherit from this base class.
 * Provides common functionality for event handling and identification.
 */
class Event
{
public:
  /**
   * @brief Construct a new Event object
   * @param event_type Type of the event
   */
  Event(EventType event_type)
      : type(event_type)
  {
  }

  Event(const Event&) = default;                    ///< Copy constructor
  Event(Event&&) = default;                         ///< Move constructor
  auto operator=(const Event&) -> Event& = default; ///< Copy assignment operator
  auto operator=(Event&&) -> Event& = default;      ///< Move assignment operator

  /**
   * @brief Destroy the Event object (virtual for proper polymorphism)
   */
  virtual ~Event() = default;

  /**
   * @brief Get the event type
   * @return EventType Type of the event
   */
  [[nodiscard]] auto getType() const -> EventType { return type; }

  /**
   * @brief Check if event has been handled
   * @return true Event has been handled
   * @return false Event has not been handled yet
   */
  [[nodiscard]] auto isHandled() const -> bool { return handled; }

  /**
   * @brief Set the handled status of the event
   * @param is_handled New handled status
   */
  void setHandled(bool is_handled) { handled = is_handled; }

  /**
   * @brief Convert event to string representation
   * @return String String representation of the event
   */
  [[nodiscard]] virtual auto toString() const -> String { return "Event"; }

private:
  EventType type;       ///< Type of the event
  bool handled = false; ///< Whether the event has been handled
};

/**
 * @brief Window close event
 *
 * Triggered when the window is requested to close (e.g., clicking X button).
 */
class WindowCloseEvent : public Event
{
public:
  /**
   * @brief Construct a new WindowCloseEvent object
   */
  WindowCloseEvent()
      : Event(EventType::WINDOW_CLOSE)
  {
  }

  /**
   * @brief Convert event to string representation
   * @return String "WindowCloseEvent"
   */
  [[nodiscard]] auto toString() const -> String override { return "WindowCloseEvent"; }
};

/**
 * @brief Key pressed event
 *
 * Triggered when a keyboard key is pressed down.
 * Includes information about the key and whether it's a repeat event.
 */
class KeyPressedEvent : public Event
{
public:
  /**
   * @brief Construct a new KeyPressedEvent object
   * @param key_code SDL keycode of the pressed key
   * @param is_repeat Whether this is a key repeat event
   */
  KeyPressedEvent(int32_t key_code, bool is_repeat = false)
      : Event(EventType::KEY_PRESSED)
      , keycode(key_code)
      , repeat(is_repeat)
  {
  }

  /**
   * @brief Get the keycode of the pressed key
   * @return i32 SDL keycode
   */
  [[nodiscard]] auto getKeycode() const -> i32 { return keycode; }

  /**
   * @brief Check if this is a repeat event
   * @return bool true if this is a key repeat event, false otherwise
   */
  [[nodiscard]] auto isRepeat() const -> bool { return repeat; }

  /**
   * @brief Convert event to string representation
   * @return String String representation including keycode and repeat status
   */
  [[nodiscard]] auto toString() const -> String override
  {
    return "KeyPressedEvent: key=" + std::to_string(keycode) + (repeat ? " (repeat)" : "");
  }

private:
  int32_t keycode; ///< SDL keycode of the pressed key
  bool repeat;     ///< Whether this is a key repeat event
};

EGE_NAMESPACE_END