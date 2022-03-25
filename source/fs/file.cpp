#include <lbx/fs/file.hpp>
#include <lbx/fs/detail/assert.hpp>

#include "native.hpp"

namespace lbx
{
	namespace fs
	{
		native_file_handle open_native_file(const char* _path, openmode _mode, openflag _flags, error_code& _errc)
		{
			native::open_info _info{};
			_info.flags = _flags;
			_info.mode = _mode;

			native_file_handle _file = 0;
			if (!native::open(_file, _path, _info, _errc))
			{
				// TODO : Error reporting
				_file = 0;
			};
			return _file;
		};
		native_file_handle open_native_file(const char* _path, openmode _mode, openflag _flags)
		{
			error_code _errc{};
			auto o = open_native_file(_path, _mode, _flags, _errc);
			if (_errc) { on_unhandled_error(_errc); };
			return o;
		};

		native_file_handle open_native_file(const char* _path, openmode _mode, error_code& _errc)
		{
			return open_native_file(_path, _mode, openflag{}, _errc);
		};
		native_file_handle open_native_file(const char* _path, openmode _mode)
		{
			error_code _errc{};
			auto o = open_native_file(_path, _mode, _errc);
			if (_errc) { on_unhandled_error(_errc); };
			return o;
		};

		void close_native_file(native_file_handle _file, error_code& _errc)
		{
			native::close(_file, _errc);
		};
		void close_native_file(native_file_handle _file)
		{
			error_code _errc{};
			close_native_file(_file, _errc);
			if (_errc) { on_unhandled_error(_errc); };
		};
	};

	namespace fs
	{
		file open(const char* _path, openmode _mode, openflag _flags, error_code& _errc)
		{
			return file(open_native_file(_path, _mode, _flags, _errc));
		};
		file open(const char* _path, openmode _mode, openflag _flags)
		{
			return file(open_native_file(_path, _mode, _flags));
		};
		file open(const char* _path, openmode _mode, error_code& _errc)
		{
			return file(open_native_file(_path, _mode, _errc));
		};
		file open(const char* _path, openmode _mode)
		{
			return file(open_native_file(_path, _mode));
		};

		void close(file& _file, error_code& _errc)
		{
			_file.close(_errc);
		};
		void close(file& _file)
		{
			_file.close();
		};
	};
};

