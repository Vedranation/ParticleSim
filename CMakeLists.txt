cmake_minimum_required(VERSION 3.29)
project(ParticleSim)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.2)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 20)

add_executable(ParticleSim main.cpp
        Particle.cpp
        Particle.h
        utilities.h
        utilities.cpp)
target_link_libraries(ParticleSim sfml-graphics)