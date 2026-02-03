#include "engine/platform/window_sdl.hpp"

#include <iostream>

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_video.h"
#include "engine/core/logging.hpp"
#include "engine/platform/events.hpp"

EGE_NAMESPACE_BEGIN

SDLWindow::SDLWindow(const String& title, u32 width, u32 height)
    : m_width(width)
    , m_height(height)
    , m_should_close(false)
{
  const auto window_width = static_cast<int>(width);
  const auto window_height = static_cast<int>(height);

  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (m_window == nullptr) {
    std::cerr << "Window Creation Error: " << SDL_GetError() << '\n';
    return;
  }

  EGE_DEBUG("Window created: {} ({}, {})", title, width, height);
}

SDLWindow::~SDLWindow()
{
  if (m_window != nullptr) {
    SDL_DestroyWindow(m_window);
  }
  EGE_DEBUG("Window destroyed");
}

void SDLWindow::update()
{
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    handleSdlEvent(event);
  }
}

void SDLWindow::handleSdlEvent(const SDL_Event& sdl_event)
{
  switch (sdl_event.type) {
    case SDL_QUIT: {
      m_should_close = true;
      if (m_event_callback) {
        WindowCloseEvent event;
        m_event_callback(event);
      }
      break;
    }

    case SDL_KEYDOWN: {
      if (m_event_callback) {
        const bool repeat = (sdl_event.key.repeat != 0);
        KeyPressedEvent event(sdl_event.key.keysym.sym, repeat);
        m_event_callback(event);

        if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
          m_should_close = true;
        }
      }
      break;
    }

    case SDL_WINDOWEVENT: {
      if (sdl_event.window.event == SDL_WINDOWEVENT_RESIZED) {
        m_width = static_cast<u32>(sdl_event.window.data1);
        m_height = static_cast<u32>(sdl_event.window.data2);
      }
      break;
    }

    default: {
      break;
    }
  }
}

auto Window::create(const String& title, u32 width, u32 height) -> std::unique_ptr<Window>
{
  return std::make_unique<SDLWindow>(title, width, height);
}

EGE_NAMESPACE_END