cmake_minimum_required(VERSION 3.30)
project(sourcebound)

set(SDL3_DIR "${CMAKE_SOURCE_DIR}/external/SDL3/install/cmake/")
find_package(SDL3 REQUIRED CONFIG)

include_directories("${CMAKE_SOURCE_DIR}/external/SDL3/install/include")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/googletest)
add_executable(sourcebound
        src/main.cpp
        include/ecs/IEntityManager.h
        include/ecs/Entity.h
        src/ecs/EntityManager.cpp
        include/ecs/EntityManager.h
        include/ecs/components/Components.h
        include/ecs/IComponentManager.h
        include/ecs/IComponentArray.h
        include/ecs/DenseComponentArray.h
        include/logger/Logger.h
        include/logger/sinks/ConsoleSink.h
        src/logger/Logger.cpp
        include/logger/sinks/ISink.h
        tests/mocks/MockSink.h
        include/ecs/ecs_config.h
        include/ecs/ISystemManager.h
        include/ecs/SystemManager.h
        include/ecs/ISystem.h
        src/ecs/SystemManager.cpp
        include/gamestate/IGameWorld.h
        include/gamestate/GameWorld.h
        tests/mocks/MockSystemManager.h
        include/input/InputCode.h
        include/input/IInputHandler.h
        src/input/KeyboardHandler.cpp
        include/input/KeyboardHandler.h
        include/input/IBindingMap.h
        include/input/IInputNormalizer.h
        include/input/InputNormalizer.h
        include/input/InputAction.h
        include/gamestate/ICommand.h
        include/input/HardcodedBindingMap.h
        src/input/HardCodedBindingMap.cpp
        src/input/InputNormalizer.cpp
        include/gamestate/PlayerMoveCommand.h
        tests/mocks/MockCommandQueue.h
        include/gamestate/ICommandQueue.h
        src/gamestate/PlayerMoveCommand.cpp
        src/gamestate/CommandQueue.cpp
        include/gamestate/CommandQueue.h
        include/input/IEventSource.h
        include/input/SDLEventSource.h
        include/input/BaseInputHandler.h
)

add_executable(unit_tests
        tests/test_dummy.cpp
        tests/entity_manager_tests.cpp
        src/ecs/EntityManager.cpp
        include/ecs/components/Components.h
        include/ecs/IComponentArray.h
        include/ecs/IComponentManager.h
        tests/component_manager_tests.cpp
        tests/logger_tests.cpp
        src/logger/Logger.cpp
        include/logger/sinks/ISink.h
        include/ecs/ISystemManager.h
        include/ecs/SystemManager.h
        include/ecs/ISystem.h
        src/ecs/SystemManager.cpp
        tests/system_manager_tests.cpp
        tests/ecs_integration_tests.cpp
        include/gamestate/IGameWorld.h
        tests/mocks/MockComponentManager.h
        tests/mocks/MockEntityManager.h
        tests/mocks/MockSystem.h
        tests/game_world_tests.cpp
        include/input/InputCode.h
        tests/input_code_tests.cpp
        include/input/IInputHandler.h
        src/input/KeyboardHandler.cpp
        include/input/KeyboardHandler.h
        include/input/IBindingMap.h
        include/input/IInputNormalizer.h
        include/input/InputNormalizer.h
        include/input/InputAction.h
        include/gamestate/ICommand.h
        include/input/HardcodedBindingMap.h
        src/input/HardCodedBindingMap.cpp
        src/input/InputNormalizer.cpp
        include/gamestate/PlayerMoveCommand.h
        tests/mocks/MockCommandQueue.h
        include/gamestate/ICommandQueue.h
        src/gamestate/PlayerMoveCommand.cpp
        src/gamestate/CommandQueue.cpp
        include/gamestate/CommandQueue.h
        include/input/IEventSource.h
        tests/keyboard_handler_tests.cpp
        tests/mocks/MockInputNormalizer.h
        tests/mocks/MockEventSource.h
        include/input/BaseInputHandler.h
        tests/player_move_command_tests.cpp
        tests/mocks/MockBindingMap.h
        tests/input_normalizer_tests.cpp
        tests/mocks/SimpleMockCommand.h
        tests/command_queue_tests.cpp
        tests/hardcoded_bindings_tests.cpp
)

target_include_directories(sourcebound PUBLIC
        "${CMAKE_SOURCE_DIR}/external/SDL3/install/include"
)

target_link_libraries(sourcebound PUBLIC
        SDL3::SDL3-shared
)

add_custom_command(TARGET sourcebound POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL3/install/bin/SDL3.dll"
        $<TARGET_FILE_DIR:sourcebound>
)

target_include_directories(unit_tests PUBLIC
        "${CMAKE_SOURCE_DIR}/external/SDL3/install/include"
)

target_link_libraries(unit_tests
        SDL3::SDL3-shared
        gtest
        gmock
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)