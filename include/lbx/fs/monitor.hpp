#pragma once
#ifndef LBX_FS_MONITOR_HPP
#define LBX_FS_MONITOR_HPP

/** @file */

#include <lbx/fs/error.hpp>

#include <string_view>

namespace lbx
{
	namespace fs
	{
		struct file_monitor;

		file_monitor* new_file_monitor();
		void delete_file_monitor(file_monitor*& _ptr);
		size_t monitor_directory(file_monitor& _monitor, const char* _path, bool _recursive = false);

	};
};


#endif // LBX_FS_MONITOR_HPP