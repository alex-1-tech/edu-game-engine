# Roadmap: Phase 1 - Foundation (Months 1-3)
**Goal:** An engine that can be launched and shows the first code visualization.


## Week 0: Preparation (Lead's Preliminary Work)

- [ ] **Architectural Analysis:** Finalize technology choices (ECS, graphics backend, windowing library).
- [ ] **Development Environment:** Create and verify a reproducible dev environment (Docker/WSL/scripts).
- [ ] **Repository Template:** Set up the main repository with README, LICENSE, .gitignore, and basic folder structure.

## Month 1: Skeleton and "Hello, Engine!"

**Goal:** Build a minimal, runnable engine skeleton with an empty window and core systems.

### Month 1 Tasks:

- [ ] **M1.1: Project Initialization and Build System**
    - Configure root `CMakeLists.txt` with C++20 and Python 3.11 support.
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

- [ ] **M1.4: Editor Stub**
    - Create a separate `editor` application (possibly on the same engine).
    - Implement a basic interface using the chosen GUI library (Dear ImGui / Qt).
    - Display a list of entities and components from the `World` in the interface.
    - Implement "Create/Delete Entity" buttons to test GUI-ECS linkage.

- [ ] **M1.5: Python Integration (Basic Bindings)**
    - Integrate `pybind11` into the build system.
    - Create basic bindings for key engine types (`Vec2`, `Entity`, `World`).
    - Enable creating an entity and adding a data component to it from Python.
    - Enable running a Python script from C++ code (e.g., via a button in the editor).

**Month Result:** A runnable application (engine or editor) that shows a window, creates entities via GUI, executes a simple Python script, and updates their position via a C++ system.

## Month 2: Visualization and Determinism

**Goal:** Add rendering, state snapshotting, and lay the groundwork for the Learning Runtime.

### Month 2 Tasks:

- [ ] **M2.1: Simple Renderer (Debug-First)**
    - Initialize a graphics context (OpenGL / Vulkan / SDL Renderer).
    - Implement rendering of 2D primitives: points, lines, rectangles, circles.
    - Create `Transform` (position, rotation, scale) and `Renderable` (color, shape) components.
    - Implement a `RenderingSystem` that draws all entities with `Transform` and `Renderable`.
    - Add debugging: draw axes, bounds, and overlay debug information on the scene.

- [ ] **M2.2: State and Determinism**
    - Implement serialization/deserialization of the `World` state (to a simple format, e.g., JSON).
    - Create a `SnapshotManager` capable of saving and loading the entire engine state each frame (or every N frames).
    - Implement a "Rewind" mechanism: replaying saved snapshots backward.
    - Ensure that simulation with identical initial state and input produces identical results.

- [ ] **M2.3: Learning Runtime Core (Code Analysis)**
    - Create the `python/learning/` module with a basic API.
    - Implement parsing of Python code (string) into AST using the standard library (`ast`).
    - Create an analyzer that walks the AST and collects basic information (variables, loops, functions).
    - Implement a simple "Constraints" system: checking the AST for forbidden constructs (e.g., `while`).

- [ ] **M2.4: Execution Instrumentation**
    - Integrate Python's `sys.settrace` or similar to intercept code execution line-by-line.
    - Implement data collection at each step: line number, local variable values.
    - Send this data back to the C++ engine for future visualization.
    - Create a buffer for execution trace history (last N steps).

- [ ] **M2.5: First "Code -> Visualization" Integration**
    - Create a panel in the editor for Python code input.
    - Connect the flow: code input -> AST analysis -> instrumentation -> execution in sandbox.
    - Visualize the result: e.g., variable `x` in the code controls the `Transform` position of a square on screen.
    - Display current values of tracked variables in real-time next to the code.

**Month Result:** The engine renders objects on screen. Python code entered in the editor executes and affects object state. State can be saved, "rewound" back, and variable value changes can be seen step-by-step.

## Month 3: Educational Layer and First Prototype

**Goal:** Connect all parts into a working prototype demonstrating the core "code visualization" idea.

### Month 3 Tasks:

- [ ] **M3.1: Execution Visualizer**
    - Create a separate panel/view in the editor for execution visualization.
    - Visualize the function call stack as a column of blocks.
    - Visualize variable changes over time as a simple graph or value history.
    - Highlight the currently executing line of code in the editor.

- [ ] **M3.2: Educational Sandbox**
    - Implement safe code execution: time limits, memory limits, recursion limits.
    - Create an execution "context" that exposes only a safe API to the learner (e.g., `move(entity, x, y)`, `get_time()`).
    - Catch errors in user code and transform standard Python tracebacks into clear, visual explanations.

- [ ] **M3.3: "Task" or "Level" System**
    - Define a JSON/YAML format for describing a learning level: initial world state, goal, allowed Python constructs.
    - Implement a `TaskManager` that loads a level, checks code against constraints, and evaluates the result (was the goal achieved?).
    - Create 2-3 demonstration levels (e.g., "Make the square reach the point", "Avoid obstacles using conditionals").

- [ ] **M3.4: Debugging and Introspection**
    - Implement Pause and Step Over/Into execution for Python code.
    - Create an `Inspector` allowing real-time viewing and modification of any entity's component values.
    - Connect the inspector with the visualizer: click an entity in the world -> show its data; change data in inspector -> immediate on-screen update.

- [ ] **M3.5: Build and Demo First Prototype**
    - Write build scripts to create a standalone version of the editor (with all dependencies).
    - Create a short demo video showing the full cycle: open editor -> load level -> write code -> see execution visualization -> square reaches goal -> rewind time.
    - Prepare `docs/first_prototype.md` with instructions for running the demo and a feature overview.
