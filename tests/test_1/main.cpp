#include <lbx/fs.hpp>

#include <iostream>

int main()
{
	namespace fs = lbx::fs;

	fs::error_code _errc{};
	auto _monitor = fs::new_file_monitor();
	if (!_monitor)
	{
		std::cout << "failed to create file monitor\n";
		return 1;
	};

	auto _key = fs::monitor_directory(*_monitor, TEST_SOURCE_ROOT);


	fs::delete_file_monitor(_monitor);
	return 0;
};