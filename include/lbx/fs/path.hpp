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
		/**
		 * @brief Categorization of files
		*/
		enum class file_type
		{
			/**
			 * @brief Just a regular old file.
			*/
			file = 1,

			/**
			 * @brief Directory type file.
			*/
			directory,
		};

		bool is_directory(const char* _path, error_code& _errc);
		bool is_directory(const char* _path);

		/**
		 * @brief Creates a file.
		 * @param _path Path to the file.
		 * @param _type The type of file to create.
		 * @param _recursive Whether or not to create all parent paths to the given (if they don't exist).
		 * @return True if the directory now exists, false otherwise.
		*/
		bool create_file(const char* _path, file_type _type, bool _recursive = false);

		/**
		 * @brief Creates a regular file.
		 * @param _path Path to the file to create.
		 * @param _recursive Whether or not to create all parent paths to the given (if they don't exist).
		 * @return True if the directory now exists, false otherwise.
		*/
		bool create_file(const char* _path, bool _recursive = false);

		/**
		 * @brief Creates a directory.
		 * @param _path Path to the directory.
		 * @param _recursive Whether or not to create all parent paths to the given (if they don't exist).
		 * @return True if the directory now exists, false otherwise.
		*/
		bool create_directory(const char* _path, bool _recursive = false);


		std::string_view parent(const char* _path);
		
		bool exists(const char* _path);
	};
};


#endif // LBX_FS_PATH_HPP