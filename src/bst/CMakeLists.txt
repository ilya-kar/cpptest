add_library(bst STATIC
        avl.h
        avl.cpp
)

target_include_directories(bst PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)