# Roadmap: Phase 1 - Foundation (Months 1-3)
**Goal:** An engine that can be launched and shows the first code visualization.


## Week 0: Preparation (Lead's Preliminary Work)

- [x] **Architectural Analysis:** Finalize technology choices (ECS, graphics backend, windowing library).
- [x] **Development Environment:** Create and verify a reproducible dev environment (Docker/WSL/scripts).
- [x] **Repository Template:** Set up the main repository with README, LICENSE, .gitignore, and basic folder structure.

## Month 1: Skeleton and "Hello, Engine!"

**Goal:** Build a minimal, runnable engine skeleton with an empty window and core systems.

### Month 1 Tasks:

- [x] **M1.1: Project Initialization and Build System**
    - Configure root `CMakeLists.txt` with C++17 and Python 3.11 support.
    - Create the basic directory structure (`engine/`, `editor/`, `python/`, `tests/`, `docs/`).
    - Integrate basic dependencies (package manager, e.g., vcpkg/conan) or write setup instructions.
    - Ensure the "empty" project builds on Linux (primary platform) and, if possible, Windows.

- [ ] **M1.2: Platform Layer**
    - Implement window abstraction (creation, event loop, closing).
    - Implement basic input handling (keyboard, mouse, window events).
    - Implement main application loop management (Game Loop).
    - File I/O for reading resources.

- [ ] **M1.3: Basic ECS System**
    - Define core structures: `Entity` (ID), `Component` (templated container), `System`.
    - Implement `World` to register and manage entities and components.
    - Implement a simple update system (e.g., `MovementSystem` for testing).
    - Enable adding/removing components at runtime.

- [ ] **M1.4: Python Integration (Basic Bindings)**
    - Integrate `pybind11` into the build system.
    - Create basic bindings for key engine types (`Vec2`, `Entity`, `World`).
    - Enable creating an entity and adding a data component to it from Python.
    - Enable running a Python script from C++ code (e.g., via a button in the editor).

**Month Result:** A runnable application (engine or editor) that shows a window, creates entities via GUI, executes a simple Python script, and updates their position via a C++ system.

