# -------------------------
# ECS layer static library
# -------------------------
add_library(EduGameEngineECS STATIC
    src/world.cpp
)

# -------------------------
# Create namespace alias for the ECS library
# -------------------------
add_library(EduGameEngine::ECS ALIAS EduGameEngineECS)

# -------------------------
# Include directories configuration for ECS library
# -------------------------
target_include_directories(EduGameEngineECS
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# ECS library dependencies
# EduGameEngine::Core - depends on core engine systems
# -------------------------
target_link_libraries(EduGameEngineECS
    PUBLIC
        EduGameEngine::Core
)
