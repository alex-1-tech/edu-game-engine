# -------------------------
# Fetch and configure fmt library
# Modern C++ formatting library, used for logging
# -------------------------
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.0
)
FetchContent_MakeAvailable(fmt)

# -------------------------
# Core static library
# Contains engine foundation systems
# -------------------------
add_library(core STATIC
    src/logging.cpp
    src/time.cpp
)

# -------------------------
# Include directories configuration
# Public headers are exposed to users of this library
# -------------------------
target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# Library dependencies
# fmt::fmt - for string formatting in logging
# engine_glm - for math utilities (inherited from parent)
# -------------------------
target_link_libraries(core PUBLIC fmt::fmt engine_glm)

# -------------------------
# Compile definitions
# FMT_HEADER_ONLY=1 - Use header-only mode for fmt (smaller binaries)
# -------------------------
target_compile_definitions(core PRIVATE
    FMT_HEADER_ONLY=1
)

# -------------------------
# Export include directories to parent scope
# Allows parent CMakeLists.txt to know our include paths
# -------------------------
set(CORE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)


# -------------------------
# Create namespace alias for the core library
# EduGameEngine::core::core - follows CMake best practices for namespacing
# -------------------------
add_library(EduGameEngine::core::core ALIAS core)
