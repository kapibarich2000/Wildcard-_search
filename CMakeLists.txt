cmake_minimum_required(VERSION 3.5)
project(WildCard_search)

add_executable(WildCard_search main.cpp WildCard.cpp WildCard.h)

set_target_properties(
        WildCard_search PROPERTIES
        CXX_STANDARD 14
)