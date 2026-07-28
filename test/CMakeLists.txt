add_executable(avl_test
    test_avl.cpp
)

target_link_libraries(avl_test PRIVATE
    avl
)

add_test(NAME AvlTest COMMAND avl_test)
