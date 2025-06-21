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
## Intent Component Architecture for Input

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
## TEMPLATE

### Decision:  
Text here.
### Reasoning:  
- Text here.
### Tradeoffs:  
- Text here.