cmake_minimum_required(VERSION 3.0)

project(thread_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
    add_compile_options(
        $<$<CONFIG:>:/MT>
        $<$<CONFIG:Debug>:/MTd>
        $<$<CONFIG:Release>:/MT>
    )
endif()

add_executable(main main.cpp constants.hpp Trafficlight.cpp Car.cpp RandomGenerator.cpp )

find_package(Threads REQUIRED)
find_package(SFML 2.5 COMPONENTS window system graphics REQUIRED)

target_link_libraries(main sfml-graphics sfml-window Threads::Threads)
