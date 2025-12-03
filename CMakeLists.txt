cmake_minimum_required(VERSION 3.10)
project(lab_06 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_NO_PROGRESS TRUE
    DOWNLOAD_TIMEOUT 600
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Основное приложение
add_executable(${PROJECT_NAME}
    main.cpp
    src/npc.cpp
    src/slaver.cpp
    src/knight.cpp
    src/squirrel.cpp
    src/factory.cpp
    src/observer.cpp
    src/visitor.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Тесты
enable_testing()
add_executable(test_all
    test/test_all.cpp
    src/npc.cpp
    src/slaver.cpp
    src/knight.cpp
    src/squirrel.cpp
    src/factory.cpp
    src/observer.cpp
    src/visitor.cpp
)

target_include_directories(test_all PRIVATE include)
target_link_libraries(test_all PRIVATE GTest::gtest_main)

add_test(NAME AllTests COMMAND test_all)
