cmake_minimum_required(VERSION 3.30)
project(sourcebound)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/googletest)
add_executable(sourcebound src/main.cpp
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
)

target_link_libraries(unit_tests
        gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)