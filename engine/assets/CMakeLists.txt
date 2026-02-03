# -------------------------
# Assets layer static library
# -------------------------
add_library(EduGameEngineAssets STATIC
    src/texture_loader.cpp
)

# -------------------------
# Create namespace alias for the Assets library
# -------------------------
add_library(EduGameEngine::Assets ALIAS EduGameEngineAssets)

# -------------------------
# Include directories configuration for Assets library
# -------------------------
target_include_directories(EduGameEngineAssets
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# Assets library dependencies
# -------------------------
target_link_libraries(EduGameEngineAssets
    PUBLIC
    EduGameEngine::Core
    EduGameEngine::Graphics
)
