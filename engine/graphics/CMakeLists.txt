# -------------------------
# Graphics layer static library
# -------------------------
add_library(EduGameEngineGraphics STATIC
    src/sdl_renderer.cpp
)

# -------------------------
# Create namespace alias for the Graphics library
# -------------------------
add_library(EduGameEngine::Graphics ALIAS EduGameEngineGraphics)

# -------------------------
# Include directories configuration for Graphics library
# -------------------------
target_include_directories(EduGameEngineGraphics
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# ECS library dependencies
# -------------------------
target_link_libraries(EduGameEngineGraphics
    PUBLIC
    EduGameEngine::Core
    EduGameEngine::Platform
    SDL2::SDL2
    SDL2_image::SDL2_image 
)

target_compile_definitions(EduGameEngineGraphics
    PUBLIC
        $<$<PLATFORM_ID:Windows>:SDL_MAIN_HANDLED>
)