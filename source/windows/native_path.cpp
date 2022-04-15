#include "native_path.hpp"



namespace lbx
{
	namespace fs
	{
		namespace native
		{
			bool exists(const char* _path)
			{
				// Create find handle
				WIN32_FIND_DATA _data{};
				auto _findHandle = FindFirstFileA(_path, &_data);
				
				// Return false on failure
				if (_findHandle == INVALID_HANDLE_VALUE)
				{
					// Failure!
					return false;
				};

				// Close handle and return true
				FindClose(_findHandle);
				return true;
			};
		};

		namespace native::win
		{
			bool create_file(const CreateFileParams& _params)
			{
				auto _result = CreateFileA
				(
					_params.path,
					_params.desired_access,
					_params.share_mode,
					_params.security_attributes,
					DWORD(_params.creation_disposition),
					_params.flags_and_attributes,
					_params.template_file
				);
				if (_result)
				{
					CloseHandle(_result);
				};
				return _result != NULL;
			};
			bool create_directory(const CreateFileParams& _params)
			{
				auto _result = CreateDirectoryA
				(
					_params.path,
					_params.security_attributes
				);
				return _result;
			};
		};

		namespace native
		{
			inline bool create_file_impl_regular(const char* _path, bool _recursive)
			{
				auto _params = win::CreateFileParams(_path, win::CreateMode::create_new);
				return win::create_file(_params);
			};
			inline bool create_file_impl_directory(const char* _path, bool _recursive)
			{
				auto _params = win::CreateFileParams(_path);
				return win::create_directory(_params);
			};

			bool create_file(const char* _path, file_type _type, bool _recursive)
			{
				switch (_type)
				{
				case file_type::file:
					return create_file_impl_regular(_path, _recursive);
					break;
				case file_type::directory:
					return create_file_impl_directory(_path, _recursive);
					break;
				default:
					// Unhandled case!
					_LBX_FS_ASSERT(false);
					return false;
				};
			};
		};
	};
};
