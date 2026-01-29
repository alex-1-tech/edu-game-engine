## High-Level Architecture
```
┌───────────────────────────────────────────────────────────┐
│ Educational Game (Lesson Game)                            │
│ (Built on the engine - C++ Core)                          │
│ • Core game logic (C++)                                   │
│ • Python scripting for educational tasks                  │
│ • Student code sandbox integration                        │
└───────────────────────────────────────────────────────────┘
                           │ (uses C++ Engine API)
                           ▼
┌───────────────────────────────────────────────────────────┐
│ Game Engine Core (C++ Foundation)                         │
│                                                           │
├───────────────────────────────────────────────────────────┤
│ 1. Runtime Systems (C++ Only)                             │
│    • Entity Component System (ECS)                        │
│    • Scene Graph / World Management                       │
│    • Physics (2D/Simple 3D)                               │
│    • Rendering (2D/3D via SDL/OpenGL)                     │
│    • Audio, Input, Resource Management                    │
├───────────────────────────────────────────────────────────┤
│ 2. Education Support Layer                                │
│    • Secure Python Sandbox (isolation, limits)            │
│    • Task/Quest Management System                         │
│    • Solution Validation Engine                           │
│    • Hint and Feedback Generation                         │
│    • Learning Metrics Collection                          │
│    • Code Execution Visualizer (step-by-step)             │
└───────────────────────────────────────────────────────────┘
                           │ (Python bindings via pybind11)
                           ▼
┌───────────────────────────────────────────────────────────┐
│ Python Scripting Layer                                    │
│ • Safe API for student code                               │
│ • Sandboxed execution environment                         │
│ • Limited access to game objects                          │
└───────────────────────────────────────────────────────────┘
                           │
                           ▼
┌───────────────────────────────────────────────────────────┐
│ Development Tools                                         │
│ • Game Editor (C++/ImGui or Python/Qt)                    │
│ • Educational Content Creator                             │
│ • Student Progress Analytics                              │
└───────────────────────────────────────────────────────────┘
```

## Directory Structure

```
edu-game-engine/
├── README.md                      # Project overview, getting started guide
├── LICENSE                        # MIT or other open-source license
├── .gitignore                     # Standard git ignore patterns
│
├── docs/                          # ALL documentation
│   ├── vision.md                  # Why this project exists, goals, philosophy
│   ├── architecture.md            # System architecture (as we described)
│   ├── roadmap.md                 # 3 / 6 / 12 month development plan
│   ├── onboarding.md              # How new developers can contribute
│   ├── coding-style.md            # C++ and Python style guidelines
│   ├── git-workflow.md            # Branching strategy, commit rules, PR process
│   └── tasks/                     # RFCs and task design documents
│
├── engine/                        # Engine Core (C++)
│   ├── core/                      # Math, utilities, files, time
│   ├── ecs/                       # Entity Component System
│   ├── platform/                  # Platform Layer
│   ├── scene/                     # Scene management, hierarchy
│   ├── graphics/                  # Rendering (shaders, materials, camera)
│   ├── physics/                   # Physics (2D/3D)
│   ├── audio/, input/, resources/ # Other systems
│   ├── scripting/                 # Python integration
│   └── education/                 # Education support module
│       ├── sandbox/               # Student code isolation and execution
│       ├── tasks/                 # Task system and validation
│       ├── visualization/         # Code execution visualization
│       ├── feedback/              # Hint generation and error analysis
│       └── metrics/               # Learning data collection
│   └── CMakeLists.txt            # Engine build configuration
├── editor/                        # Game development editor
│   ├── ui/                        # User interface components
│   ├── scene_editor/              # Level editor
│   ├── task_editor/               # Educational task creator
│   ├── tools/                     # Development tools
│   └── CMakeLists.txt            # Editor build configuration
├── python/                        # Python components and bindings
│   ├── bindings/                  # pybind11 C++ bindings generation
│   ├── sdk/                       # Python SDK for game developers
│   ├── editor_tools/              # Python tools for the editor
│   └── pyproject.toml            # Python package configuration
├── games/                         # Example games built with the engine
│   ├── tutorial_platformer/       # Platformer with programming lessons
├── cmake/                         # CMake utilities and modules
│   ├── FindPython.cmake          # Python detection
│   ├── Findpybind11.cmake        # pybind11 integration
│   └── CompilerWarnings.cmake    # Warning configuration
├── tests/                         # Test suites
│   ├── cpp/                       # C++ unit and integration tests
│   └── python/                    # Python tests
├── tools/                         # Development tools and scripts
│   ├── format.sh                  # Code formatting script
│   ├── lint.sh                    # Linting scripts
│   └── precommit/                 # Git hooks for code quality
│
└── CMakeLists.txt                 # Root CMake configuration
```

**This allows:**
- Clean include paths (`#include <engine/core/base.hpp>`)
- Correct installation and export via CMake
- Future multi-module expansion
## Engine Modules Description

### 1. Core (`engine/core/`)
**Purpose:** Foundation utilities used throughout the engine

**Components:**
- **Math:** Vectors, matrices, quaternions, geometry
- **Time:** Deterministic timers, frame timing
- **Logging:** Structured, educational-focused logging
- **Memory:** Custom allocators with debugging support
- **Serialization:** Save/load for game state and rewinding

## Build System

### CMake Structure

```
CMakeLists.txt (root)  
├── engine/CMakeLists.txt  
├── editor/CMakeLists.txt  
├── python/CMakeLists.txt  
└── tests/CMakeLists.txt
```

### Dependencies

#### Required
- **VSCode**
- **C++17** compatible compiler (GCC 11+, Clang 14+, MSVC 2019+)
- **Python 3.11+** with development headers
- **CMake 3.20+**
#### Core Dependencies
- **pybind11** (for Python/C++ bindings)
- **glm** (math library)
- **entt** or custom ECS (TBD)
#### Optional/Platform-specific
- **OpenGL/Vulkan** (rendering backend TBD)
- **Qt6** or **Dear ImGui** (editor UI TBD)
- **SDL2** (window/input abstraction)
## Platform Support

### Primary Development Platform
- **Linux** (Ubuntu 22.04+, OpenSuse 16.0+, GCC/Clang)
### Secondary Platforms
- **Windows** (via MSVC or MinGW)
### Web/Educational Consideration
Future consideration: **WebAssembly** port for browser-based use in classrooms.

## Testing Strategy

### Test Categories
1. **Unit Tests** - Individual components
2. **Integration Tests** - C++/Python interaction
3. **Determinism Tests** - Reproducible execution
4. **Educational Tests** - Learning outcomes
5. **Performance Tests** - Frame time consistency
### Test Framework
- **C++:** Catch2 or Google Test
- **Python:** pytest
- **Integration:** Custom test runner
