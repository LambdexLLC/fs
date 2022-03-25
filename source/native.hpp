#pragma once
#ifndef LBX_FS_NATIVE_HPP
#define LBX_FS_NATIVE_HPP

/** @file */

// Includes native file system implementation headers

#include "windows/native_file.hpp"


#include <cstdlib>

namespace lbx
{
	namespace fs
	{
		inline void on_unhandled_error(const error_code& _errc)
		{
			::abort();
		};
	};
};

#endif // LBX_FS_NATIVE_HPP