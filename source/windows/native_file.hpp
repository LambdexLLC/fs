#pragma once
#include <lbx/fs/detail/os.hpp>
#if LBX_WINDOWS_V
#ifndef LBX_FS_WINDOWS_NATIVE_FILE_HPP
#define LBX_FS_WINDOWS_NATIVE_FILE_HPP

/** @file */

#include <lbx/fs/file.hpp>
#include <lbx/fs/monitor.hpp>
#include <lbx/fs/detail/assert.hpp>

#include <memory>
#include <vector>
#include <array>
#include <mutex>
#include <atomic>
#include <thread>

#include "io.hpp"
#include "win_lib.hpp"

namespace lbx
{
	namespace fs
	{
		namespace native
		{
			struct open_info
			{
				openmode mode;
				openflag flags;
				bool overlapped_io;
			};

			error_code get_last_error();

			bool open_directory(native_file_handle& _file, const char* _path, const open_info& _info, error_code& _errc);
			bool open_file(native_file_handle& _file, const char* _path, const open_info& _info, error_code& _errc);
			
			// May call open_file() or open_directory() depending on what _path points to
			bool open(native_file_handle& _file, const char* _path, const open_info& _info, error_code& _errc);


			bool close(native_file_handle& _file, error_code& _errc);
			bool is_directory(const char* _path, error_code& _errc);

		};
	};
};


namespace lbx
{
	namespace fs
	{
		namespace native
		{
			using unique_file_handle = basic_unique_handle<struct file_handle_tag>;
		};

		using monitor_key = size_t;

		struct file_monitor_data
		{
		private:
			
			struct monitor_data
			{
			public:

				void on_change(file_monitor_data& _monitor, DWORD _bytesTransferred);

				monitor_key key() const { return this->key_; };

				void refresh(error_code& _errc);
				void refresh();

				monitor_data(native::unique_file_handle _file, monitor_key _key, bool _recursive = false) :
					file_(std::move(_file)),
					key_(_key),
					recursive_(_recursive)
				{};

			private:
				std::array<DWORD, 64> notify_buffer_{};
				OVERLAPPED overlapped_{};
				native::unique_file_handle file_;
				monitor_key key_;
				bool recursive_ = false;
				bool needs_refresh_ = true;
			};

			monitor_data* find(monitor_key _key);
			void insert(std::unique_ptr<monitor_data> _data);

		public:

			void poll();
			void wait();
			void wait(DWORD _timeoutMS);

			void create();
			void destroy();

			monitor_key monitor_directory(const char* _path, bool _recursive, error_code& _errc);
			monitor_key monitor_directory(const char* _path, bool _recursive);

			void erase(monitor_key _key);

			void write_change(file_change _change);
			size_t get_changes(file_change*& _outBuffer, size_t _bufferLen);

			file_monitor_data() = default;

		private:

			native::unique_io_port port_;
			std::vector<std::pair<monitor_key, std::unique_ptr<monitor_data>>> monitoring_;
			std::vector<file_change> changes_{};

			monitor_key key_counter_ = 0;
		};


		struct file_monitor
		{
		private:

			struct ThreadData
			{
				std::mutex mtx{};
				std::atomic_bool stop_flag{ false };
				std::atomic_bool init_flag{ false };
				file_monitor_data monitor;

				ThreadData() = default;
			};

			static void thread_main(ThreadData* _data);

		public:

			void create();
			void destroy();

			monitor_key monitor_directory(const char* _path, bool _recursive, error_code& _errc);
			monitor_key monitor_directory(const char* _path, bool _recursive);

			void erase(monitor_key _key);
			
			size_t get_changes(file_change*& _outBuffer, size_t _bufferLen);

			file_monitor() = default;

		private:
			std::unique_ptr<ThreadData> data_;
			std::thread thread_;
		};




	};
};

#endif // LBX_FS_WINDOWS_NATIVE_FILE_HPP
#endif // LBX_WINDOWS_V