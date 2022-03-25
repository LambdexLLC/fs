
add_executable(test0 "test0/main.cpp")
add_test(NAME test0 COMMAND test0)
target_link_libraries(test0 PUBLIC lbx-fs)
