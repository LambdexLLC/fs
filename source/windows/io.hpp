#pragma once
#include <lbx/fs/detail/os.hpp>
#if LBX_WINDOWS_V
#ifndef LBX_FS_WINDOWS_IO_HPP
#define LBX_FS_WINDOWS_IO_HPP

/** @file */

#include "handle.hpp"

#include <lbx/fs/file.hpp>

namespace lbx
{
	namespace fs
	{
		namespace native
		{
			using unique_io_port = basic_unique_handle<struct io_port_tag>;

			inline unique_io_port new_io_port()
			{
				return unique_io_port(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0));
			};
			
			inline void add_to_port(const unique_io_port& _port, const native_file_handle& _file, ULONG_PTR _completionKey, error_code& _errc)
			{
				const auto r = CreateIoCompletionPort(_file, _port.get(), _completionKey, 0);
				if (r != _port.get())
				{
					_errc = error_code(GetLastError());
				};
			};

		};
	};
};


#endif // LBX_FS_WINDOWS_IO_HPP
#endif // LBX_WINDOWS_V