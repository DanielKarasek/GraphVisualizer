cmake_minimum_required(VERSION 3.21)
project(GraphVizualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/pako/Qt/6.2.2/gcc_64")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM "/usr/include/graphviz/")

add_compile_options(-Wall -Wextra -Werror -Wpedantic -Wno-psabi)

set(sou)

set(SOURCES
        src/GVGraph.cpp
        src/GVNode.cpp
        src/GraphUtilities.cpp
        src/GVEdge.cpp)
set(HEADERS
        include/GraphVisualizer/GVGraph.h
        include/GraphVisualizer/GVNode.h
        include/GraphVisualizer/GVEdge.h
        include/GraphVisualizer/GraphUtilities.h
        )


add_library(
        GraphVisualizer
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(GraphVisualizer PUBLIC src include)
target_include_directories(GraphVisualizer PRIVATE include/GraphVisualizer)

target_link_libraries(GraphVisualizer PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        cgraph
        gvc
        )

export(TARGETS GraphVisualizer FILE GraphVisualizer.cmake)
