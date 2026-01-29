# Roadmap: Phase 1 - Foundation (Months 1-3)
**Goal:** An engine that can be launched and shows the first code visualization.


## Week 0: Preparation (Lead's Preliminary Work)

- [x] **Architectural Analysis:** Finalize technology choices (ECS, graphics backend, windowing library).
- [x] **Development Environment:** Create and verify a reproducible dev environment (Docker/WSL/scripts).
- [x] **Repository Template:** Set up the main repository with README, LICENSE, .gitignore, and basic folder structure.

## Month 1: Engine Core

**Goal:** Build a minimal, runnable engine skeleton with ECS and basic systems.

### Month 1 Tasks:

- [x] **M1.1: Project Initialization and Build System**
    - Configure root `CMakeLists.txt` with C++17 and Python 3.11 support.
    - Create the basic directory structure (`engine/`, `editor/`, `python/`, `tests/`, `docs/`).
    - Integrate basic dependencies (package manager, e.g., vcpkg/conan) or write setup instructions.
    - Ensure the "empty" project builds on Linux (primary platform) and, if possible, Windows.

- [x] **M1.2: Platform Layer**
    - Implement window abstraction (creation, event loop, closing).
    - Implement basic input handling (keyboard, mouse, window events).
    - Implement main application loop management (Game Loop).
    - File I/O for reading resources.

- [x] **M1.3: Basic ECS System**
    - Define core structures: `Entity` (ID), `Component` (templated container), `System`.
    - Implement `World` to register and manage entities and components.
    - Implement a simple update system (e.g., `MovementSystem` for testing).
    - Enable adding/removing components at runtime.

- [x] **M1.4: Core Utilities**
	- Mathematical utilities with GLM integration
	- Structured logging system with educational context
	- Time management with fixed and variable timesteps
	- Type definitions and aliases for engine-wide use

### Month 2: Game Systems & Visualization

**Goal:** Add graphics, physics, and create a complete demo game.

- [ ] **M2.1: 2D Graphics System**
	- Create `Renderer` abstract interface
	- Implement SDL2-based 2D renderer:
	    - Basic shape rendering (rectangles, circles, lines)
	    - Texture loading and sprite rendering
	    - Simple text rendering for debugging
	- Create `RenderSystem` for ECS integration
	- Add visualization for ECS component data
- [ ] **M2.2: Simple Physics System**
	- Implement 2D physics components:
	    - `Collider` - AABB and circle colliders
	    - `RigidBody` - mass, velocity, forces
	- Create `PhysicsSystem`:
	    - Basic collision detection
	    - Simple impulse resolution
	    - Gravity and basic forces
	- Add physics visualization (collider outlines, force vectors)
- [ ] **M2.3: Scene Management**
	 - Implement `Scene` class for level management
	- Create scene loading/saving system
	- Add `Camera` component and system
	- Implement viewport and camera controls
	- Add simple asset management system
- [ ] **M2.4: Complete Demo Game**
- Create "Physics Playground" demo:
    - Interactive physics objects (balls, boxes)
    - Mouse interaction (drag, throw)
    - Basic UI for controls
    - Educational annotations and explanations
- Implement game state management
- Add simple audio feedback