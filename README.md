# Sourcebound

Sourcebound is a lightweight, modular game engine built with an Entity-Component-System (ECS) architecture in modern C++. It is designed for flexibility, performance, and ease of extension.

This project began as a proof of concept to explore ECS architecture, showcase my coding process, and deepen my understanding of modern C++ systems design.

The initial goal is to implement an Arkanoid clone, with plans to extend the engine through additional simple games (such as Pong) as the architecture matures.

(This repo is intended as a personal architecture playground, not a generalized engine or public library.)

---

## [Getting Started](#-getting-started)

To get up and running with Sourcebound:

1. Clone the repository:
   ```bash
   git clone https://github.com/yourname/sourcebound.git
   cd sourcebound
   ```
2. Build using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

---

## [Documentation](#-documentation)

Sourcebound is built with developer clarity in mind. Explore the following documents to understand how the engine works and how to contribute:

- [Architecture Overview](docs/architecture/project-sourcebound-overview-v2.jpg) - Explore the ECS architecture and module breakdown.
- [Design Decisions](docs/decisions.md) - Rationale behind key design choices.
- [Research Notes](docs/research.md) - Background research informing the engine design.
- [Development Journal](docs/journal.md) - A log of my development progress and goals.

### Project Management 

As I am using this as an example of how I work, I am doing thorough task management via Git projects.  I am including periodic snapshots of this to represent project state over time, and so these can be reviewed as part of a zip of the core code base.

* [Project Board](https://github.com/users/spaceofjace/projects/4) - the live board of this project.
* [Project Snapshots](docs/project-management/) - periodic snapshots of the project as I go.

---

## [Tests](#-tests)

To run the test suite:
```bash
cd build
ctest
```

---

## [Contributing](#-contributing)

I am currently not seeking collaborators on this project as I largely intend it as an example of how I approach building code and systems; that said, please feel free to reach out if you’re interested in collaborating on a substantial expansion in the future.

---

## [License](#-license)

This project is currently unlicensed, and subject to copyright (for now), though as it represents common ways of implementing common systems, I don't expect there to be any real enforcement of that copyright (unless someone is blatantly plagiarizing my code for profit.)  

There may come some future stage at which I decide on a license (MIT?) for this project, but I want to simplify that process as much as possible, which is why I am leaving it open-ended for now.
