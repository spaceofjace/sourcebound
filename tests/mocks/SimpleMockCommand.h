//
// Created by shult on 6/20/2025.
//

#ifndef SIMPLEMOCKCOMMAND_H
#define SIMPLEMOCKCOMMAND_H
#include "../../include/gamestate/CommandQueue.h"
using namespace sb::gamestate;
using namespace sb::ecs;
class SimpleMockCommand : public ICommand {
 public:
  [[nodiscard]] std::string name() const override {return "SimpleMockCommand";}
  bool was_applied = false;
  void apply(std::shared_ptr<GameWorld>) override {
    was_applied = true;
  }
};
#endif //SIMPLEMOCKCOMMAND_H
