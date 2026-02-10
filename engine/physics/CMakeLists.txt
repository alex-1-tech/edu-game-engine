# -------------------------
# Physics layer static library
# -------------------------
add_library(EduGameEnginePhysics STATIC
    src/collision_detection.cpp
)

# -------------------------
# Create namespace alias for the Physics library
# -------------------------
add_library(EduGameEngine::Physics ALIAS EduGameEnginePhysics)

# -------------------------
# Include directories configuration for Physics library
# -------------------------
target_include_directories(EduGameEnginePhysics
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# Physics library dependencies
# -------------------------
target_link_libraries(EduGameEnginePhysics
    PUBLIC
    EduGameEngine::Core
    EduGameEngine::Graphics
    EduGameEngine::ECS
)
