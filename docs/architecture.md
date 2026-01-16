## Architectural Principles

### 1. Explicit Over Implicit
- No hidden magic or implicit behavior
- Everything that happens should be explainable
- Configuration over convention
### 2. Determinism
- Identical code produces identical results
- Frame-accurate execution for debugging
- Reproducible behavior for learning
### 3. Observability
- Every system can be inspected at runtime
- Full state capture for rewinding
- Execution tracing and visualization
### 4. Education-First Design
- Clarity and explainability over optimization
- Debugging support as a core feature
- Progressive complexity in API design
## High-Level Architecture
```
┌─────────────────────────────────────────────────────┐  
│ Editor Application                                  │  
│ (Python/Qt/ImGui + C++ backend)                     │  
└─────────────────────────────────────────────────────┘  
                           |
                           v
┌─────────────────────────────────────────────────────┐  
│ Learning Runtime                                    │  
│ (Python + Code Analysis + Visualization)            │  
│ • AST analysis                                      │  
│ • Execution instrumentation                         │  
│ • Constraint enforcement                            │  
│ • Explanation generation                            │  
└─────────────────────────────────────────────────────┘  
                           |
                           v
┌─────────────────────────────────────────────────────┐  
│ Engine Runtime                                      │  
│ (C++ Core Systems)                                  │  
│ • Entity Component System                           │  
│ • Scene management                                  │  
│ • Physics (simplified, visualizable)                │  
│ • Rendering (debug-first)                           │  
└─────────────────────────────────────────────────────┘  
                           |
                           v
┌─────────────────────────────────────────────────────┐  
│ Platform Layer                                      │  
│ (OS/Window/Input/File System)                       │  
└─────────────────────────────────────────────────────┘
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
├── engine/                        # C++ runtime core
│   ├── core/                      # Base types, math utilities, logging, timing
│   ├── ecs/                       # Entity-Component-System implementation
│   ├── scene/                     # World management and serialization
│   ├── systems/                   # Movement, logic, rendering, scripting 
│   ├── rendering/                 # Debug-first rendering backend
│   ├── physics/                   # Simple, visualizable physics simulation
│   ├── scripting/                 # Python integration and sandbox environment
│   ├── debug/                     # Inspection, rewind, execution tracing tools
│   └── CMakeLists.txt             # Engine build configuration
│
├── editor/                        # Editor application (Qt/ImGui based)
│   ├── ui/                        # User interface components
│   ├── tools/                     # Educational tools and visualizers
│   ├── project/                   # Project management functionality
│   └── CMakeLists.txt             # Editor build configuration
│
├── python/                        # Python components and bindings
│   ├── bindings/                  # pybind11 C++ bindings generation
│   ├── learning/                  # Learning runtime implementation
│   ├── editor_tools/              # Python tools for the editor
│   └── pyproject.toml             # Python package configuration
│
├── tests/                         # Test suites
│   ├── cpp/                       # C++ unit and integration tests
│   └── python/                    # Python tests
│
├── tools/                         # Development tools and scripts
│   ├── format.sh                  # Code formatting script (clang-format, black)
│   ├── lint.sh                    # Linting scripts (clang-tidy, flake8)
│   └── precommit/                 # Git hooks for code quality
│
├── cmake/                         # CMake utilities and modules
│   ├── FindPython.cmake           # Python detection
│   ├── Findpybind11.cmake         # pybind11 integration
│   └── CompilerWarnings.cmake     # Warning configuration
│
└── CMakeLists.txt                 # Root CMake configuration
```

## Core Modules Description

### 1. Core (`engine/core/`)
**Purpose:** Foundation utilities used throughout the engine

**Components:**
- **Math:** Vectors, matrices, quaternions, geometry
- **Time:** Deterministic timers, frame timing
- **Logging:** Structured, educational-focused logging
- **Memory:** Custom allocators with debugging support
- **Serialization:** Save/load for game state and rewinding

### 2. ECS (`engine/ecs/`)
**Purpose:** Entity-Component-System architecture core

**Design:**
- **Entities:** Simple integer IDs (no data, no logic)
- **Components:** Plain data structures (no logic)
- **Systems:** Logic that processes entities with specific components
- **World:** Manages entities, components, and systems

**Features:**
- Deterministic execution order
- Runtime inspection of component data
- Visual debugging tools

### 3. Scene (`engine/scene/`)
**Purpose:** High-level game world management

**Responsibilities:**
- Entity hierarchy and parenting
- Scene serialization/deserialization
- Resource management (textures, models, sounds)
- Scene switching and state management

### 4. Rendering (`engine/rendering/`)
**Purpose:** Debug-first visualization system

**Philosophy:** Clarity over graphical fidelity

**Features:**
- Debug drawing as first-class feature
- Visualizations for data structures and algorithms
- Code execution visualization
- Performance overlay for educational purposes
- Support for both 2D and simple 3D

### 5. Physics (`engine/physics/`)
**Purpose:** Simplified, visualizable physics simulation

**Design Choices:**
- Deterministic simulation
- Visual debugging of collisions and forces
- Step-by-step execution for learning
- Focus on explainability over complexity

### 6. Scripting (`engine/scripting/`)
**Purpose:** Python integration and sandboxing

**Components:**
- **Python VM:** Embedded CPython with custom extensions
- **Sandbox:** Security and resource limits for learner code
- **Bindings:** pybind11-based C++ to Python bindings
- **Debugging:** Python debugger integration

### 7. Debug (`engine/debug/`)
**Purpose:** Educational debugging and inspection tools

**Tools:**
- **State Inspector:** Browse and modify game state
- **Execution Tracer:** Record and replay execution
- **Rewind System:** Go back in time to debug
- **Visual Debugger:** See code execution visually

## Learning Runtime (Key Differentiator)

### Purpose
The Learning Runtime sits between the editor and engine runtime, transforming standard code execution into a learning experience.

### Components

#### 1. Code Analysis
- **AST Parser:** Parse Python code into abstract syntax trees
- **Complexity Analysis:** Measure code complexity for adaptive difficulty
- **Pattern Detection:** Identify common programming patterns and anti-patterns
- **Constraint Checking:** Enforce educational constraints (e.g., "no loops in this level")

#### 2. Execution Instrumentation
- **Line-by-line Tracing:** Track which lines execute and in what order
- **Variable Monitoring:** Watch variable values change over time
- **Function Call Tracking:** Visualize the call stack
- **Performance Metrics:** Measure execution time and memory usage

#### 3. Feedback Generation
- **Error Explanation:** Transform Python errors into educational explanations
- **Hint System:** Generate context-aware hints when learners are stuck
- **Progress Tracking:** Monitor learner progress through concepts
- **Achievement System:** Reward mastery of programming concepts

#### 4. Visualization
- **Code Visualization:** See code structure and flow
- **Data Structure Visualization:** Watch lists, dictionaries, objects evolve
- **Algorithm Visualization:** Step through sorts, searches, etc.
- **Memory Visualization:** See allocation and garbage collection

## Frame Lifecycle

### 1. INPUT PHASE  
- Collect user input (keyboard, mouse, code edits)  
- Process editor commands  
### 2. LEARNING RUNTIME PHASE  
- Parse and analyze learner's Python code  
- Apply educational constraints  
- Instrument code for tracing  
- Generate visualizations and hints  
### 3. ENGINE EXECUTION PHASE  
- Execute C++ systems (physics, AI, etc.)  
- Run instrumented Python code  
- Update game state  
### 4. OBSERVATION PHASE  
- Capture state snapshot (for rewinding)  
- Collect execution metrics  
- Update visualizations  
### 5. RENDERING PHASE  
- Render game visuals  
- Overlay debug information  
- Draw code visualizations  
### 6. DEBUG & VISUALIZATION PHASE  
- Update inspector tools  
- Process debug commands  
- Prepare educational feedback  
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
- **C++20** compatible compiler (GCC 11+, Clang 14+, MSVC 2019+)
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
- **Linux** (Ubuntu 22.04+, GCC/Clang)
### Secondary Platforms
- **Windows** (via MSVC or MinGW)
### Web/Educational Consideration
Future consideration: **WebAssembly** port for browser-based use in classrooms.
## Performance Considerations

### Design Trade-offs
- **Determinism** over maximum performance
- **Debugging support** over optimization
- **Explainability** over rendering fidelity
- **Educational value** over feature completeness
### Critical Performance Paths
1. **Python/C++ boundary** - Minimize marshaling overhead
2. **State snapshotting** - Efficient capture for rewinding
3. **Visualization updates** - Real-time code visualization
4. **Execution tracing** - Low-overhead instrumentation
### Optimization Strategy
- Profile educational use cases, not benchmarks
- Optimize for frame time consistency
- Memory efficiency for rewinding system
- Async operations for long-running analyses

## Security Considerations
### Sandbox Requirements
- Learner code must be isolated
- Resource limits (CPU, memory, time)
- Filesystem access restrictions
- Network access control
### Code Safety
- Validate Python code before execution
- Timeout long-running operations
- Memory limit enforcement
- Recursion depth limits
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

## Extension Points

### Plugin System (Future)
- Custom visualization tools
- Additional programming languages
- Alternative learning methodologies
- Integration with learning management systems
### Modding Support (Future)
- Custom educational games
- Community-created puzzles
- Shared visualization tools
- Template projects for educators