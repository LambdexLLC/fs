#pragma once
#ifndef LBX_FS_PATH_HPP
#define LBX_FS_PATH_HPP

/** @file */

#include <lbx/fs/error.hpp>

#include <string_view>

namespace lbx
{
	namespace fs
	{
		bool is_directory(const char* _path, error_code& _errc);
		bool is_directory(const char* _path);

		std::string_view parent(const char* _path);

	};
};


#endif // LBX_FS_PATH_HPP