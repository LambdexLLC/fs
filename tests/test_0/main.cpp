#include <lbx/fs.hpp>

int main()
{
	namespace fs = lbx::fs;

	fs::error_code _errc{};
	auto _file = fs::open(TEST_SOURCE_ROOT, fs::openmode::read, _errc);
	
	if (!fs::is_directory(TEST_SOURCE_ROOT, _errc) || _errc)
	{
		return 1;
	};

	if (!_file || _errc)
	{
		return 1;
	};
	fs::close(_file, _errc);

	return 0;
};