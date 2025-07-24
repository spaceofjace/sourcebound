# Project Sourcebound Dev Journal
---
## 2025-07-24 - 7 hours
(includes ~2 hours from 7/23 +  ~1 hour from 7/21.)

### Objective(s)

1. Fix input bug (7/21)
2. Implement basic game data (7/23)
3. Get initial level loading setup
### Details
#### Key Accomplishments

* I now have a game window that is initialized by (hardcoded) configurable game data.
* This renders four walls, a "bottom wall" for out of bounds, the paddle and ball, all in correct starting positions based on the level data.
* Cleaned up some handling, as well as fixed a bug in `EntityManager`
* Also, unit tests run on pushes now, so I shouldn't have any sneaking failing tests anymore when I neglect to run them all.  
	* Less pressure to run them every time, especially as a solo developer
	* I may eventually try to get a pre-push trigger working, but as far as I know, it's not compatible with Github Desktop client, which I find easier to use.
	* Importantly, though, failing tests will block merge to main.
#### Challenges / Notes

* Templated methods are WAY more of a pain for testability than I would have initially guessed.  I am making a task to refactor my templated methods to use type_id for registration, etc, though I may keep the templated methods for syntactic sugar.  (When you know you have the concrete type.)
* I also forgot SDL3 renders from the corner rather than the center; I am managing this conversion within `SdlRenderer` since that is technically internal to how it is managed, and that will keep use of Transform simplified external to render systems. 
#### Next Steps

* Probably going to take on the refactor next, to better enable the init method in GameWorld to do what it needs to do. (Found work.)
* Then next is the `PhysicsSystem`, which will hopefully play well with the `MovementSystem` already...though until I have Collision online, it won't clamp to boundaries.  Yet. 

---
## 2025-07-16 - 3.5 hours

### Objective(s)

1. Complete simple rendering components and RenderSystem
2. Hook up RenderSystem in main for verification.
### Details
#### Key Accomplishments

* Circle and rectangle are correctly rendering! (See [/milestone-artifacts/m1.75-rendering/rendering_milestone_screenshot.png](/milestone-artifacts/m1.75-rendering/rendering_milestone_screenshot.png) )
#### Challenges / Notes

* It's unclear why, but I did retest that the input system was working, and found it was not.  I had never tested it on my desktop, as far as I remember.  (I believe I confirmed it on my laptop.)  This may have something to dowith how my keyboard is wired in (through a docking station) not feeding inputs.  Will need to do more investigation.  (And retest on laptop to see if it still works there.)
	* I will also run some tests before and after the render system changes to see if the new code did somehow bork the old code.
#### Next Steps

* Validate input system isn't completely broken.
	* If it works on my laptop, I will need to test my keyboard by plugging it directly into my PC instead of through the docking station. 
	* If it is how my system is wired...well, I'll need to figure out how to fix that problem.
	* As far as I can tell, there were NO keyboard events happening at all.
* After I get that back to functional, it's just a matter of starting to pull together some core systems and get the main game loop functional!
	* This is big...but also where the real fun begins.

---
## 2025-07-09 - 3 hours
(includes a little task setup time not captured from 6/20)
### Objective(s)

1. Get basic rendering interface built and a simple renderer implemented
### Details
#### Key Accomplishments

* So far, the rendering code is much simpler than I would have expected.
* Decided to add common colors to improve ease of use for rendering in practice.  (Component data can reference colors.)
#### Challenges / Notes

* Would love to see how a DX implementation would compare down the line.
	* This obviously could be improved and enhanced with things like textures and shaders.
	* Adding text rendering *might* be more complex, but we aren't there yet.
* SdlRenderer's dependence on SDL_Renderer essentially cuts off the ability to unit test code.  An integration test later might be worthwhile.  (And I will of course continue to ponder ways to make it more unit testable as I continue working.)
#### Next Steps

* Creating actual components.
* Making the first pass of a RenderingSystem.
* Adding rendering to the game loop.

---
## 2025-06-20 - 10 hours 

Includes time from 2025-06-19
### Objective(s)

1. Implement bindings support.
2. Implement an input normalizer to translate into commands
3. Implement a command queue and commands
4. Implement basic keyboard polling
### Details
#### Key Accomplishments

* I mean I did all of the goals listed, including writing all the doxygen docs and tests (though maybe I missed something somewhere)
#### Challenges / Notes

* I think I jumped in too fast and implemented things in the wrong order; this ended up making it a massive changelist, which is not my preferred way to work.
* I walked back my decision to use intents -> commands because this does add too much indirection without much benefit at this stage.  It's probably a change I will eventually look into doing at some future stage, just to better understand how to properly support scripting, playback, AI...but I need to get to a working renderer and gameplay loop.  (This can be a future optimization.)
#### Next Steps

* Need to wire up input handling in the GameWorld.
* Maybe see if theres a way to run it all through a console and validate keyboard inputs are triggering commands.
* Need to update architecture and decisions docs to reflect reversal of decision.

---
## Jun 5, 2025 – 3.5 hours

### Objective(s)

1. Integrate SDL3 into Sourcebound as a linked runtime dependency
2. Establish normalized `InputCode` abstraction for keyboard, mouse, and gamepad inputs
### Details

#### Key Accomplishments

* Successfully configured and installed SDL3 as a local external dependency with working CMake linkage.
* Implemented `InputCode` struct using `std::variant` in preparation and support of multiple input sources (and configuration of key bindings)
#### Challenges / Notes

* Embedding SDL3 the way I did was _painful_ and required multiple attempts to get everything linking and usable in code.
#### Next Steps

- Consume input code in KeyboardInputHandler (which will require IKeyProvider or similar to allow proper unit testing)

---
## Jun 4, 2025 - 2.5 hours

(note: this includes some time spent on May 29, 2025)
### Objective(s)

1. Prepare repo for public viewing
2. Add README
3. Add Doxygen workflows as Github actions
### Details
#### Key Accomplishments

* Still much to do in terms of code documentation, but at least the first steps are there, and all the high-level file documentation is in place.
#### Challenges / Notes

* Will take some time to go back and document all the functions and members of the code.
* I probably should have formalized all this as tasks in the project, but wanted to get it all in place in case any prospective employers looked at my codebase.
#### Next Steps

* Pausing on code documentation for now, but will eventually go back and add more detail in passes.

---
## May 9, 2025 - 2 hours

### Objective(s)

1. Prepare for input system development.
### Details
#### Key Accomplishments

* Expanded and updated architecture diagram to reflect input system.
* Created new milestone to represent growing complexity of input system.
* Generated full task list for new input systems.
#### Challenges / Notes

* In thinking about rebinding and an extendable engine system, I realized the system was growing in complexity quickly.  
	* Recognized I needed to stop and think about this more expansively.
	* This resulted in an update to the architecture diagram AND the generation of a new milestone.
* I'm obviously adding a lot of work for myself, but I want to be sure this project is a good reflection of how I approach problems.
#### Next Steps

* Begin implementation of input mapping systems.

---
## May 8, 2025 - 3 hours

### Objective(s)

1. Implement the GameWorld facade
### Details
#### Key Accomplishments

* Added the facade, and built it in a way that it's truly "injectable" with mocks
* Extracted and added a bunch of mocks to use in future testing
#### Challenges / Notes

* Working with templates is obviously much more complex than C# generics or similar; this does add a bit of noise to the code, and though I considered other options to avoid templating the *class* (integration testing instead of unit testing, injecting the concrete implementation, a gross-feeling dynamic_cast), this seemed like the good middle ground to allow flexibility and correctly adhere to the open/closed principle.
#### Next Steps

* Inputs next, I think.

---
## Apr 24, 2025 - 3 hours

Note: also captures work from Apr 23; time spent reflects time from both days.
### Objective(s)

1. Update EntityManager to allow signature tracking (will be important in eventual system management).
2. Validate EntityManager and ComponentManager together in integration tests.
### Details
#### Key Accomplishments

* Found a couple bugs/missing functionality as I wrote the new integration tests.
	- `is_alive` did not check version
	- needed helpers for getting `ComponentType` for signature management
	- revised some error handling to be more "stable"
- Overall, was pretty solid code, functionally speaking.
#### Challenges / Notes

* As noted in my last entry, I want to be thoughtful about places where I am throwing exceptions vs. logging errors; I'll need to take a pass to be sure I adjust any old code I wrote.
* I want to move the templated code into `.impl` files, and even though there's only one function in `IComponentManager` that is not templated (for now), I want to add an appropriate `.cpp` file for it.  (This can catch any expanded growth.)
#### Next Steps

* Next is taking some of the logic represented in the ECS integration tests and putting it in a `GameWorld` API to centralize these behaviors.
	* I don't (yet) want to move into a heavily event-based architecture, but it may be cleaner for things like `EntityDestroyedEvent` (for cleaning up, etc.)
* I think this makes sense to do before input management, but that's next on the list.

---
## Apr 22, 2025 - 6 hours

### Objective(s)

1. Complete ComponentManager
2. Complete SystemManager
### Details
#### Key Accomplishments

* Very nearly completed all the baseline implementations for the three "bigs" for the ECS system.  I am sure I missed something, but we'll find out along the way.
* Added a simple logging system, with expandability or swappable with a library, if desired.
#### Challenges / Notes

* Logger was found work, though it probably shouldn't have been; logging is pretty much a given in any system. :P
* I want to think very carefully about error handling.  I'm currently throwing in some unrecoverable cases, but I need to ponder on that more...
#### Next Steps

* Going to add some simple components and entities.
* After that, going to tackle some input systems (hopefully.)

---
## Apr 21, 2025 - 2.5 hours

### Objective(s)

1. Implement EntityManager
### Details
#### Key Accomplishments

* Built out basic entity creation system, including some logic for reusing entities
* Basic test cases are in place for the interface
#### Challenges / Notes

* Possible the re-use logic is a bit overkill at this stage, but I am trying to walk the boundary between a custom implementation and a commercial implementation of an ECS, so at least some *basic* optimizations seem worthwhile. 
* My C++ definitely is a bit dated; slowly coming up to speed on some modern concepts, e.g., `nodiscard` and `std::optional`. (Fortunately all simple, I am just having to remember to use these modern semantics.)
#### Next Steps

* Build ComponentManager; this one will be a bit more substantial, for sure.

---
## Apr 7, 2025 - 3 hours

### Objective(s)

1. Get project and source folders setup
2. Configure IDE to ensure Visual Studio 2022 x64 compatibility
3. Setup Google Test and a single mock test
### Details
#### Key Accomplishments

* All the project stuff is set, and I should be ready to begin coding tomorrow.
#### Challenges / Notes

* Download and project setup was finnicky, which I _should_ have expected, but as always there were a few gotchas along the way.
* It's unclear if GoogleTests will be compatible with Visual Studio 2022, but I'll probably spend some time later to investigate.
#### Next Steps

* Begin tests and implementation of the core ECS pieces.

---
## Apr 5, 2025 - 2.5 hours

### Objective(s)

1. Finish majority of design and documentation
2. Setup project to begin first milestone
### Details
#### Key Accomplishments

* I knocked out the C1 and C2 diagrams here: https://miro.com/app/board/uXjVIG8i9E0=/
* I populated all the known tasks needed to complete Milestone 1 (including acceptance criteria)
#### Challenges / Notes

* I was sick the last couple of days, which set back my milestones; we're recovering, though, and are back on track (I think).  I may have to push a little Monday or Tuesday to knock out a chunk of work.  It definitely won't be ready by Wednesday, which is the technical due date.
* I also didn't quite map out the ECS C3 diagram, but it's the first thing on my agenda tomorrow, and probably the only thing keeping me from "closing" M0.
#### Next Steps

* Complete C3 diagram
* Get project and test harness setup
* Write some code!! (for basic foundational stuff, anyway)

---
## Apr 2, 2025 - 5 hours

### Objective(s)

1. To understand the game itself and prepare for designing my own implementation.
2. To setup repo and some initial documents for managing efforts.
### Details

#### Key Accomplishments

* Completed extensive reading of materials about Arkanoid
* Spent about 1-1.5 hours playing the original(?) and a clone online
* Got a project page and repo setup, and generated some initial documentation of my current research findings

#### Challenges / Notes

* My first time using Github Projects, but wanted to experiment with it, and have some tie between it and the repo itself.
* There's a LOT of research material out there, so I won't be able to absorb it all, but hoping I have a strong foundation for beginning design elements; not sure I will update the document more, but I dated it just in case.

#### Next Steps

* Add decisions.md file for detailing design and tool decisions.
* Begin work on high-level design documents.