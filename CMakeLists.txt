cmake_minimum_required(VERSION 3.30)
project(sourcebound)
enable_testing()

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
        include/input/SdlEventSource.h
        include/input/BaseInputHandler.h
        include/rendering/IRenderer.h
        include/rendering/SdlRenderer.h
        src/rendering/SdlRenderer.cpp
        include/rendering/Color.h
        include/ecs/RenderSystem.h
        src/ecs/RenderSystem.cpp
        tests/mocks/MockRenderer.h
        include/gamestate/QuitCommand.h
        src/gamestate/QuitCommand.cpp
        include/data/LevelData.h
        src/gamestate/GameWorld.cpp
        include/math/Vec2.h
        include/data/IGameDataManager.h
        include/data/HardcodedGameDataManager.h
        src/data/HardcodedGameDataManager.cpp
        include/ecs/systems/PhysicsSystem.h
        src/ecs/systems/PhysicsSystem.cpp
        include/ecs/systems/CollisionSystem.h
        src/ecs/systems/CollisionSystem.cpp
        include/ecs/systems/FollowSystem.h
        src/ecs/systems/FollowSystem.cpp
        include/gamestate/LaunchBallCommand.h
        src/gamestate/LaunchBallCommand.cpp
        include/math/MathConsts.h
        include/data/BrickConfig.h
        include/data/ArenaDimensions.h
        include/data/BrickLayoutConfig.h
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
        include/rendering/IRenderer.h
        include/rendering/SdlRenderer.h
        src/rendering/SdlRenderer.cpp
        include/rendering/Color.h
        include/ecs/RenderSystem.h
        src/ecs/RenderSystem.cpp
        tests/mocks/MockRenderer.h
        tests/render_system_tests.cpp
        src/gamestate/QuitCommand.cpp
        tests/quit_command_test.cpp
        include/data/LevelData.h
        src/gamestate/GameWorld.cpp
        include/math/Vec2.h
        include/data/IGameDataManager.h
        include/data/HardcodedGameDataManager.h
        src/data/HardcodedGameDataManager.cpp
        tests/hardcoded_data_manager_tests.cpp
        tests/mocks/MockGameDataManager.h
        include/ecs/systems/PhysicsSystem.h
        src/ecs/systems/PhysicsSystem.cpp
        tests/physics_system_tests.cpp
        include/ecs/systems/CollisionSystem.h
        src/ecs/systems/CollisionSystem.cpp
        tests/collision_system_tests.cpp
        include/ecs/systems/FollowSystem.h
        src/ecs/systems/FollowSystem.cpp
        tests/follow_system_tests.cpp
        include/gamestate/LaunchBallCommand.h
        src/gamestate/LaunchBallCommand.cpp
        tests/launch_ball_command_tests.cpp
        include/math/MathConsts.h
        include/data/ArenaDimensions.h
        include/data/BrickLayoutConfig.h
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