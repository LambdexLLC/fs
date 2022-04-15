#include <lbx/fs/path.hpp>

#include "native.hpp"

namespace lbx
{
	namespace fs
	{
		bool is_directory(const char* _path, error_code& _errc)
		{
			return native::is_directory(_path, _errc);
		};
		bool is_directory(const char* _path)
		{
			error_code _errc{};
			auto o = native::is_directory(_path, _errc);
			if (_errc) { on_unhandled_error(_errc); };
			return o;
		};


		
		bool create_file(const char* _path, file_type _type, bool _recursive)
		{
			return native::create_file(_path, _type, _recursive);
		};

		bool create_file(const char* _path, bool _recursive)
		{
			return create_file(_path, file_type::file, _recursive);
		};

		bool create_directory(const char* _path, bool _recursive)
		{
			return create_file(_path, file_type::directory, _recursive);
		};


		inline std::string_view parent(const std::string_view& _path)
		{
			auto _parent = _path;

			auto p = _parent.find_last_of("/\\");
			if (p == _parent.npos)
			{
				return std::string_view{};
			};

			_parent = _parent.substr(0, p);
			while (_parent.size() != 0)
			{
				auto _last = _parent.back();
				if (_last == '/' || _last == '\\')
				{
					_parent.remove_suffix(1);
				}
				else
				{
					break;
				};
			};

			return _parent;
		};
		std::string_view parent(const char* _path)
		{
			return parent(std::string_view(_path));
		};

		bool exists(const char* _path)
		{
			return native::exists(_path);
		};
	};
};
