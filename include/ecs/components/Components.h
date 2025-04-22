//
// Created by Jace Shultz on 4/21/2025.
// Copyright (c) 2025 by spaceofjace. All rights reserved.
//

// For simplicity, and because I don't expect my component list to grow too
// large (for now), I will keep all component definitions here. Later, as it
// grows, it may be pragmatic to split them into multiple files.

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>

// Physics components
struct Position { float x = 0.f, y = 0.f; };
struct Velocity { float x = 0.f, y = 0.f; };
struct Size     { float width = 1.f, height = 1.f; };
struct Rotation { float angle = 0.f; };

struct Transform {
  Position position;
  Velocity velocity;
  Size size;
  Rotation rotation;

  explicit Transform(const Position p = {}, const Velocity v = {},
    const Size s = {}, const Rotation r = {})
    : position(p), velocity(v), size(s), rotation(r) {}
};

struct Collider { bool isTrigger = false; };

//Graphics components
struct Color { float r, g, b, a; };
struct Sprite {
  std::string texturePath;
  Color color;
};

//Gameplay components
struct HitsRequired { int hits; };

//Gameplay tags
struct Ball { };
struct Paddle { };

#endif //COMPONENTS_H
