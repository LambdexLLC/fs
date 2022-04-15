#pragma once
#include <lbx/fs/detail/os.hpp>
#if LBX_WINDOWS_V
#ifndef LBX_FS_WINDOWS_NATIVE_PATH_HPP
#define LBX_FS_WINDOWS_NATIVE_PATH_HPP

/** @file */

#include <lbx/fs/path.hpp>

#include <lbx/fs/detail/assert.hpp>

#include "io.hpp"
#include "win_lib.hpp"

#include <array>
#include <memory>
#include <vector>

namespace lbx
{
	namespace fs
	{
		namespace native
		{
			namespace win
			{
				/**
				 * @brief Modes for the creation_disposition parameter.
				*/
				enum class CreateMode : DWORD
				{
					/**
					 * @brief Creates a new file, always.
					 *
					 * If the specified file exists and is writable, the function overwrites the file, the function succeeds, and last-error code is set to ERROR_ALREADY_EXISTS (183).
					 * If the specified file does not exist and is a valid path, a new file is created, the function succeeds, and the last-error code is set to zero.
					 * For more information, see the Remarks section of this topic.
					*/
					create_always = CREATE_ALWAYS,

					/**
					 * @brief Creates a new file, only if it does not already exist.
					 *
					 * If the specified file exists, the function fails and the last-error code is set to ERROR_FILE_EXISTS (80).
					 * If the specified file does not exist and is a valid path to a writable location, a new file is created.
					*/
					create_new = CREATE_NEW,

					/**
					 * @brief Opens a file, always.
					 *
					 * If the specified file exists, the function succeeds and the last-error code is set to ERROR_ALREADY_EXISTS (183).
					 * If the specified file does not exist and is a valid path to a writable location, the function creates a file and the last-error code is set to zero.
					*/
					open_always = OPEN_ALWAYS,

					/**
					 * @brief Opens a file or device, only if it exists.
					 *
					 * If the specified file or device does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND (2).
					 * For more information about devices, see the Remarks section.
					*/
					open_existing = OPEN_EXISTING,

					/**
					 * @brief Opens a file and truncates it that its size is zero bytes, only if it exists
					 *
					 * Opens a file and truncates it so that its size is zero bytes, only if it exists.
					 * If the specified file does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND (2).
					 * The calling process must open the file with the GENERIC_WRITE bit set as part of the dwDesiredAccess parameter.
					*/
					truncate_existing = TRUNCATE_EXISTING,
				};

				struct CreateFileParams
				{
					const char* path{};

					DWORD desired_access =
						GENERIC_READ | GENERIC_WRITE;
					DWORD share_mode =
						FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE;

					CreateMode creation_disposition{};

					DWORD flags_and_attributes =
						FILE_ATTRIBUTE_NORMAL;

					HANDLE template_file = NULL;
					SECURITY_ATTRIBUTES* security_attributes = NULL;

					CreateFileParams() = default;

					CreateFileParams(const char* _path, CreateMode _createMode) :
						path(_path), creation_disposition(_createMode)
					{};
					CreateFileParams(const char* _path) :
						CreateFileParams(_path, CreateMode())
					{};

				};

				bool create_file(const CreateFileParams& _params);
				bool create_directory(const CreateFileParams& _params);
			};

			bool exists(const char* _path);
			bool create_file(const char* _path, file_type _type, bool _recursive);
		};
	};
};

#endif // LBX_FS_WINDOWS_NATIVE_PATH_HPP
#endif // LBX_WINDOWS_V