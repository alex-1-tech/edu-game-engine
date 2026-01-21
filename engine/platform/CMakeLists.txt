# -------------------------
# Platform layer static library
# Handles window creation, input, and platform-specific abstractions
# Uses SDL for cross-platform window and input management
# -------------------------
add_library(EduGameEnginePlatform STATIC
    src/window_sdl.cpp
    src/application_manager.cpp
)

# -------------------------
# Create namespace alias for the platform library
# EduGameEngine::Platform - follows CMake best practices for namespacing
# Provides clean, namespaced target for consumers
# -------------------------
add_library(EduGameEngine::Platform ALIAS EduGameEnginePlatform)

# -------------------------
# Include directories configuration for platform library
# Public headers are exposed to users of this library
# Allows including platform headers in dependent code
# -------------------------
target_include_directories(EduGameEnginePlatform
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# Platform library dependencies
# EduGameEngine::Core - depends on core engine systems
# SDL2::SDL2 - uses SDL for window and input handling
# -------------------------
target_link_libraries(EduGameEnginePlatform
    PUBLIC
        EduGameEngine::Core
        SDL2::SDL2
)

# -------------------------
# Platform-specific compile definitions
# Windows requires SDL_MAIN_HANDLED for proper entry point handling
# $<$<PLATFORM_ID:Windows>:...> - generator expression for platform-specific defines
# -------------------------
target_compile_definitions(EduGameEnginePlatform
    PUBLIC
        $<$<PLATFORM_ID:Windows>:SDL_MAIN_HANDLED>
)