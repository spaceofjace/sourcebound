# Project Sourcebound Dev Journal
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