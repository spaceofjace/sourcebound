//
// Created by shult on 6/20/2025.
//

#ifndef BASEINPUTHANDLER_H
#define BASEINPUTHANDLER_H
#include "IInputHandler.h"
namespace sb::input {
class BaseInputHandler : public IInputHandler {
public:
  BaseInputHandler(std::shared_ptr<IInputNormalizer> normalizer,
                   std::shared_ptr<IEventSource> eventSource)
      : normalizer_(std::move(normalizer)), event_source_(std::move(eventSource)) {}

protected:
  [[nodiscard]] IInputNormalizer& normalizer() const { return *normalizer_; }
  [[nodiscard]] IEventSource& event_source() const { return *event_source_; }

private:
  std::shared_ptr<IInputNormalizer> normalizer_;
  std::shared_ptr<IEventSource> event_source_;
};
}

#endif //BASEINPUTHANDLER_H
