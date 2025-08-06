# Project Sourcebound – Key Design Decisions

This document outlines major technical and architectural decisions made during the development of _Project Sourcebound_ in order to provide more detail on major decisions (or assumptions) made and the reasoning behind them.

---
## Custom ECS Architecture

### Decision:
I am starting with a simple, custom ECS architecture as this maintains simple code for the purposes of this project. 
### Reasoning:
- Demonstrates understanding of ECS architecture fundamentals.
- Supports modular design changes and increases room for expansion.
- Allows future flexibility: can be replaced by libraries like [Flecs](https://www.flecs.dev/flecs/).   
- Supports clean separation of data and behavior.
### Tradeoffs:
- A custom ECS will not be nearly as highly optimized, even if it could allow for better optimization later.
- Simplicity of system could lead to bugs, e.g., complex ordering may not be feasible.
---
## Rendering and Input System Separation

### Decision:  
Input System and Rendering System are external services to the Game Engine.
### Reasoning:  
- Promotes clear architectural boundaries.
- Allows independent testing and subsystem replacement.
- Aligns with modular engine practices.
- Could extend with additional input sources, or additional rendering tech (e.g., for non-PC platforms.)
### Tradeoffs:  
- Requires additional work to integrate subsystems cleanly.
- Adds slight overhead for coordinating between modules.
---
## Command Pattern for Input

### Decision:
I am choosing to use the command pattern (or variant thereof) as actions or inputs into the ECS.
### Reasoning:
- Decouples input devices from gameplay logic.
- Simplifies input rebinding and multi-device support.
- Enables input simulation for testing.
### Tradeoffs:
- Slight increase in input handling complexity.
---
## JSON as Data Format

### Decision:  
I will be using **JSON** for gameplay data and save/load files.
### Reasoning:  
- Easy human editing and debugging.
- Compatible with future migration to Protocol Buffers or similar data protocol languages.
### Tradeoffs:  
- Less efficient than binary formats for large datasets.
- Requires careful parsing and validation.
- While more mergeable than some datatypes, it requires consistent structure to be "merge-safe."
---
## Use GoogleTests as testing framework

### Decision:  
I will be using GoogleTests as my test harness for writing unit tests.
### Reasoning:  
- Supported internally by my chosen IDE (JetBrains' CLion).
- Extensive testing framework that should mirror typical frameworks used by studios in practice.
### Tradeoffs:  
- Adds a lot of source code to the base to include the framework manually.
- Probably more complex than I need it to be for a "simple" project.
---
## Templated GameWorld with ECS Injection

### Decision:
`GameWorld` is implemented as a templated class that accepts injected types for `EntityManager`, `ComponentManager`, and `SystemManager`.

### Reasoning:
- Allows full testability using mock ECS components.
- Cleanly separates core game logic from the ECS backend.
- Enables future replacement with an external ECS library (e.g., Flecs) by swapping template arguments.
### Tradeoffs:
- Increases compile-time complexity and binary size slightly.
- More verbose to instantiate in test and production code.

---
## (!! DEFERRED !!) Intent Component Architecture for Input

***UPDATE*** (Deferred) - 
Temporarily deferred for simplicity during early development. Current implementation allows `Command`s to directly affect world state, bypassing the intent layer.
### Decision:
Rather than having `Command`s mutate the world directly, I route input through a `CommandProcessor` that adds _Intent Components_, which are then consumed by ECS systems.
### Reasoning:
- Preserves ECS purity: systems, not commands, change world state.
- Enables delayed or interruptible behavior (e.g., collision or stun overriding movement).
- Simplifies testing and replay: commands are data-only.
- Makes game logic more inspectable and override-friendly.
- Could theoretically support AI behaviors in the future.
### Tradeoffs:
- Requires slightly more boilerplate to pass through intent layers.
- These actions may feel “indirect” during early implementation.  
- Debugging may require tracing across multiple systems and steps.

---
## Center-Based Spatial Model for Transform Components

### Decision:
Entity positions and sizes are treated as center-based in `Transform` and rendering systems.
### Reasoning:
- Simplifies rotation, collision math, and spatial reasoning.
- Matches ECS-based convention for spatial transforms.
- Cleanly maps to future physics or animation systems.
### Tradeoffs:
- Requires conversion to top-left–anchored coordinates for SDL rendering.
- May introduce confusion if some systems expect top-left anchors by default.

---
## Retaining Semantic Distinctions Between Position, Size, and Vec2

### Decision:
I chose to preserve distinct types (`Position`, `Size`, and `Vec2`) rather than overloading everything with `Vec2`.
### Reasoning:
- Preserves semantic clarity (e.g., velocity != position != extent).
- Improves debug readability and code self-documentation.
- Enables flexible conversion patterns without leaking meaning.
### Tradeoffs:
- Slightly more boilerplate (conversion functions or overloads).
- Requires developer discipline to avoid treating all 2D floats as interchangeable.

---
## Component-Based Collision Behavior Flags

### Decision:
Collision logic is governed by a `CollisionBehavior` enum with bitmaskable flags (e.g., `Clamp`, `Bounce`, `Destroy`, `Trigger`), stored in collider components.
### Reasoning:
- Allows a single system to interpret collision responses based on entity configuration.
- Supports flexible, declarative behaviors without requiring inheritance or script hooks.
- Enables layered logic: multiple behaviors can be active on a single collider.
### Tradeoffs:
- Requires discipline to avoid overly complex behavior combinations.
- Systems must interpret the flags correctly and consistently (no “default” behavior).
- Not as expressive as a full-on event or scripting system.

---
## Move Away from Templated Interfaces for Registration and instead use `type_index`

### Decision:
I am shifting away from using templated methods as the default mechanism for system/component registration and access and am shifting them to runtime safe methods using `std::type_index`.
### Reasoning:
- Improves testability: interfaces can now accept runtime `std::type_index` keys and injected instances without requiring template instantiation.
- Enables mocking and dynamic composition in unit tests and runtime environments.
- Allows future dependency contexts to be constructed from runtime configuration rather than compile-time binding.
- Templated versions are preserved for developer ergonomics and remain available when compile-time safety or brevity is preferred.
### Tradeoffs:
- Slightly more verbose code in core systems or registration logic.
- Relies on `std::type_index`, which introduces [RTTI overhead](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c146-use-dynamic_cast-where-class-hierarchy-navigation-is-unavoidable) (negligible in current scope).
- Introduces potential for type mismatches if used inconsistently (e.g., manual `typeid()` mismatches), though these are mitigated by centralized registration patterns.

---
## TEMPLATE

### Decision:  
Text here.
### Reasoning:  
- Text here.
### Tradeoffs:  
- Text here.