cmake_minimum_required(VERSION 3.21.1)

#add_compile_options(-Wall)

project(ADVENTO_CONSOLE_ENGINE)

add_executable(Example ParticlesDemoMain.cpp)

add_subdirectory(engine)
target_link_libraries(Example engine)
