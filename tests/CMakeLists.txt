
macro(MAKE_TEST in_name)
	set(__name lbx-fs_test_${in_name})

	add_executable(${__name} "test_${in_name}/main.cpp")
	add_test(NAME ${__name} COMMAND ${__name})
	target_link_libraries(${__name} PUBLIC lbx-fs)
	target_compile_definitions(${__name} PRIVATE TEST_SOURCE_ROOT="${CMAKE_CURRENT_LIST_DIR}/test_${in_name}/")
endmacro()

MAKE_TEST(0)
MAKE_TEST(1)
