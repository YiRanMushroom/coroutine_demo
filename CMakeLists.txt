cmake_minimum_required(VERSION 3.31)
project(coroutine_demo)

set(PROJECT_NAME coroutine_demo)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp")

file(GLOB_RECURSE MODULES "src/*.cppm" "src/*.ixx")

add_executable(${PROJECT_NAME}  ${SOURCES} ${MODULES})

include(FetchContent)

FetchContent_Declare(
        your_workbench_lib
        URL https://github.com/YiRanMushroom/Your_Workbench_Lib/archive/master.zip
)

FetchContent_MakeAvailable(your_workbench_lib)

target_link_libraries(${PROJECT_NAME}  your_workbench_lib)

target_sources(
        ${PROJECT_NAME}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        FILES
        ${MODULES}
)
