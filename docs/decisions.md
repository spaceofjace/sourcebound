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
## TEMPLATE

### Decision:  
Text here.
### Reasoning:  
- Text here.
### Tradeoffs:  
- Text here.