cmake_minimum_required(VERSION 3.5)
<<<<<<< HEAD
project(Wildcard_search)
=======
project(Wildcard_search VERSION 0.1)
>>>>>>> origin/Development


add_executable(Wildcard_search main.cpp Wildcard.cpp Wildcard.h)

set_target_properties(
        Wildcard_search PROPERTIES
        CXX_STANDARD 14
)