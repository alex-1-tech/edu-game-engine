# -------------------------
# Add core subdirectory which contains engine modules
# -------------------------
add_subdirectory(core)
add_subdirectory(platform)

# -------------------------
# Create the main executable for the engine demo
# Links together all engine modules into a runnable application
# -------------------------
add_executable(engine_demo
    ${CMAKE_SOURCE_DIR}/engine/src/main.cpp
)

# -------------------------
# Link the executable with required engine libraries
# PRIVATE linkage means dependencies aren't exposed to downstream targets
# -------------------------
target_link_libraries(engine_demo
    PRIVATE
        EduGameEngine::Core
        EduGameEngine::Platform
)

# -------------------------
# Output configuration
# All executables will be placed in /bin directory
# -------------------------
set_target_properties(engine_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# -------------------------
# Generate compile_commands.json for tools like clangd
# Available in CMake 3.13+
# -------------------------
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
