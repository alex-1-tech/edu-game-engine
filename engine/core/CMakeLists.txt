# -------------------------
# Adding a new macro for convenience
# -------------------------
add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# -------------------------
# Fetch and configure fmt library
# Modern C++ formatting library, used for logging
# Provides type-safe, fast string formatting capabilities
# -------------------------
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.0
)
FetchContent_MakeAvailable(fmt)

# -------------------------
# Contains engine foundation systems like logging and time management
# Forms the base layer of the engine architecture
# -------------------------
add_library(EduGameEngineCore STATIC
    src/logging.cpp
    src/time.cpp
    src/config.cpp
)

# -------------------------
# EduGameEngine::Core - follows CMake best practices for namespacing
# Provides clean, namespaced target for consumers
# -------------------------
add_library(EduGameEngine::Core ALIAS EduGameEngineCore)

# -------------------------
# Public headers are exposed to users of this library
# Allows including core engine headers in dependent code
# -------------------------
target_include_directories(EduGameEngineCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------------------------
# fmt::fmt - for string formatting in logging system
# engine_glm - for math utilities (inherited from parent scope)
# -------------------------
target_link_libraries(EduGameEngineCore
    PUBLIC
        fmt::fmt
        engine_glm
        engine_json
)

# -------------------------
# FMT_HEADER_ONLY=1 - Use header-only mode for fmt (smaller binaries)
# PRIVATE definition means it's only used during compilation of this target
# -------------------------
target_compile_definitions(EduGameEngineCore
    PRIVATE FMT_HEADER_ONLY=1
)