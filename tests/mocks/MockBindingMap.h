//
// Created by shult on 6/20/2025.
//

#ifndef MOCKBINDINGMAP_H
#define MOCKBINDINGMAP_H

#include <gmock/gmock.h>
#include "../../../include/input/IBindingMap.h"

class MockBindingMap : public sb::input::IBindingMap {
public:
  MOCK_METHOD(sb::input::InputAction, resolve, (const sb::input::InputCode& input), (const, override));
  MOCK_METHOD(void, bind, (const sb::input::InputCode& input, sb::input::InputAction action), (override));
  MOCK_METHOD(void, load_bindings, (const std::string& path), (override));
  MOCK_METHOD(void, save_bindings, (const std::string& path), (const, override));
};
#endif //MOCKBINDINGMAP_H
