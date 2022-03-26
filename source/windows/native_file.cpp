#include "native_file.hpp"
#ifdef LBX_FS_WINDOWS_NATIVE_FILE_HPP

#include "native.hpp"

#include <lbx/fs/path.hpp>
#include <lbx/fs/detail/assert.hpp>


#include <iostream>
#include <string_view>



namespace lbx
{
	namespace fs
	{
		namespace native
		{
			error_code get_last_error()
			{
				return error_code(GetLastError());
			};

			bool close(native_file_handle& _file, error_code& _errc)
			{
				if (_file && !CloseHandle(_file))
				{
					_errc = native::get_last_error();
					return false;
				};
				_file = NULL;
				return true;
			};
		
			bool open_directory(native_file_handle& _file, const char* _path, const open_info& _info, error_code& _errc)
			{
				if (_file != NULL)
				{
					if (!native::close(_file, _errc))
					{
						return false;
					};
				};

				HANDLE _handle = NULL;
				DWORD _dwAccess = GENERIC_READ;
				DWORD _dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
				DWORD _dwCreationDisposition = OPEN_EXISTING;
				DWORD _dwFlagsAndAttributes = FILE_FLAG_BACKUP_SEMANTICS;

				if (_info.overlapped_io)
				{
					_dwFlagsAndAttributes |= FILE_FLAG_OVERLAPPED;
				};

				_handle = CreateFileA(_path, _dwAccess, _dwShareMode, NULL, _dwCreationDisposition, _dwFlagsAndAttributes, NULL);
				if (_handle == INVALID_HANDLE_VALUE)
				{
					_errc = get_last_error();
					return NULL;
				};

				_file = _handle;
				return _file != NULL;
			};
			bool open_file(native_file_handle& _file, const char* _path, const open_info& _info, error_code& _errc)
			{
				if (_file != NULL)
				{
					if (!native::close(_file, _errc))
					{
						return false;
					};
				};
				
				HANDLE _handle = NULL;
				DWORD _dwAccess = 0;
				DWORD _dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
				DWORD _dwCreationDisposition{};
				DWORD _dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

				if (_info.overlapped_io)
				{
					_dwFlagsAndAttributes |= FILE_FLAG_OVERLAPPED;
				};

				const auto _flags = _info.flags;
				const auto _mode = _info.mode;

				bool _truncate = (unsigned)(_flags & openflag::truncate) != 0;
				bool _append = (unsigned)(_flags & openflag::append) != 0;
				bool _binary = (unsigned)(_flags & openflag::binary) != 0;

				switch (_mode)
				{
				case openmode::write:
					_dwAccess |= GENERIC_WRITE;
					if (_truncate)
					{
						_dwCreationDisposition = CREATE_ALWAYS;
					}
					else
					{
						_dwCreationDisposition = OPEN_ALWAYS;
					};
					break;
				case openmode::read:
					_dwAccess |= GENERIC_READ;
					if (_truncate)
					{
						_dwCreationDisposition = TRUNCATE_EXISTING;
					}
					else
					{
						_dwCreationDisposition = OPEN_EXISTING;
					};
					break;
				case openmode::readwrite:
					_dwAccess = GENERIC_READ | GENERIC_WRITE;
					if (_truncate)
					{
						_dwCreationDisposition = TRUNCATE_EXISTING;
					}
					else
					{
						_dwCreationDisposition = OPEN_ALWAYS;
					};
					break;
				default:
					// Bad / Unhandled openmode
					return false;
				};

				_handle = CreateFileA(_path, _dwAccess, _dwShareMode, NULL, _dwCreationDisposition, _dwFlagsAndAttributes, NULL);
				if (_handle == INVALID_HANDLE_VALUE)
				{
					_errc = get_last_error();
					return NULL;
				};
				
				_file = _handle;
				return _file != NULL;
			};
			bool open(native_file_handle& _file, const char* _path, const open_info& _info, error_code& _errc)
			{
				const auto _isDir = fs::is_directory(_path, _errc);
				if (_errc)
				{
					return false;
				};

				if (_isDir)
				{
					return open_directory(_file, _path, _info, _errc);
				}
				else
				{
					return open_file(_file, _path, _info, _errc);
				};
			};

			bool is_directory(const char* _path, error_code& _errc)
			{
				DWORD _attribs = GetFileAttributesA(_path);
				if (_attribs == INVALID_FILE_ATTRIBUTES)
				{
					_errc = get_last_error();
					return false;
				}
				else
				{
					return (_attribs & FILE_ATTRIBUTE_DIRECTORY) != 0;
				};
			};
		};
	};

	namespace fs
	{
		inline std::string wtos(const std::wstring_view _wstr)
		{
			std::string o{};
			for (auto& v : _wstr)
			{
				if (v < 255)
				{
					o += (char)v;
				}
				else
				{
					o += '?';
				};
			};
			return o;
		};


		void file_monitor_data::monitor_data::on_change(file_monitor_data& _monitor, DWORD _bytesTransferred)
		{
			BYTE* _buffer = reinterpret_cast<BYTE*>(this->notify_buffer_.data());
			FILE_NOTIFY_INFORMATION* _info = nullptr; //reinterpret_cast<FILE_NOTIFY_INFORMATION*>(_buffer);

			while (true)
			{
				_info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(_buffer);

				auto _action = _info->Action;
				auto _path = wtos(std::wstring_view(_info->FileName, _info->FileNameLength));

				switch (_action)
				{
				case FILE_ACTION_ADDED:
				{
					_monitor.write_change(file_change::add(this->key(), _path));
				};
				break;
				case FILE_ACTION_REMOVED:
					break;
				case FILE_ACTION_MODIFIED:
				{
					_monitor.write_change(file_change::change(this->key(), _path));
				};
				break;
				case FILE_ACTION_RENAMED_OLD_NAME:

					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					break;
				default:
					break;
				};

				if (const auto _offset = _info->NextEntryOffset; _offset == 0)
				{
					break;
				}
				else
				{
					_buffer += _offset;
				};
			};

			this->needs_refresh_ = true;
		};

		void file_monitor_data::monitor_data::refresh(error_code& _errc)
		{
			if (this->needs_refresh_)
			{
				const auto r = ReadDirectoryChangesW(this->file_.get(),
					this->notify_buffer_.data(),
					this->notify_buffer_.size() * sizeof(DWORD),
					this->recursive_,
					FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
					NULL,
					&this->overlapped_,
					NULL);
				
				_errc = native::get_last_error();
				if (_errc.code() == WAIT_TIMEOUT)
				{
					_errc = error_code();
				};

				this->needs_refresh_ = false;
			};
		};
		void file_monitor_data::monitor_data::refresh()
		{
			error_code _errc{};
			this->refresh(_errc);
			if (_errc) { on_unhandled_error(_errc); };
		};



		file_monitor_data::monitor_data* file_monitor_data::find(monitor_key _key)
		{
			for (auto& v : this->monitoring_)
			{
				if (v.first == _key)
				{
					return v.second.get();
				};
			};
			return nullptr;
		};
		void file_monitor_data::insert(std::unique_ptr<monitor_data> _data)
		{
			//_data->refresh();
			this->monitoring_.push_back({ _data->key(), std::move(_data) });
		};



		void file_monitor_data::create()
		{
			this->port_ = native::new_io_port();
		};
		void file_monitor_data::destroy()
		{
			this->port_.reset();
		};



		void file_monitor_data::wait(DWORD _timeoutMS)
		{
			auto& _port = this->port_;

			for (auto& v : this->monitoring_)
			{
				v.second->refresh();
			};

			std::array<OVERLAPPED_ENTRY, 32> _entries{};
			ULONG _entriesRemoved = 0;
			auto r = GetQueuedCompletionStatusEx(_port.get(), _entries.data(), _entries.size(),
				&_entriesRemoved, _timeoutMS, false);

			if (r)
			{
				for (ULONG n = 0; n != _entriesRemoved; ++n)
				{
					auto& _entry = _entries[n];
					const auto& _bytesTransferred = _entry.dwNumberOfBytesTransferred;
					const auto& _key = _entry.lpCompletionKey;
					const auto& _overlappedPtr = _entry.lpOverlapped;

					auto _data = this->find(_key);
					if (_data)
					{
						_data->on_change(*this, _bytesTransferred);
					};
				};
			}
			else
			{
				auto _err = GetLastError();
				if (_err != WAIT_TIMEOUT)
				{
					on_unhandled_error(error_code(_err));
				};
			};

		};

		void file_monitor_data::poll()
		{
			this->wait(0);
		};
		void file_monitor_data::wait()
		{
			this->wait(INFINITE);
		};

		monitor_key file_monitor_data::monitor_directory(const char* _path, bool _recursive, error_code& _errc)
		{
			native::open_info _openInfo{};
			_openInfo.overlapped_io = true;
			_openInfo.mode = openmode::read;

			native_file_handle _rawFile{};
			auto _openResult = native::open(_rawFile, _path, _openInfo, _errc);
			if (_errc || !_openResult)
			{
				return 0;
			};
			auto _file = native::unique_file_handle(_rawFile);
			_rawFile = 0;

			const auto _key = ++this->key_counter_;
			native::add_to_port(this->port_, _file.get(), _key, _errc);
			if (_errc)
			{
				return 0;
			};
			this->insert(std::unique_ptr<monitor_data>(new monitor_data(std::move(_file), _key)));

			return _key;
		};
		monitor_key file_monitor_data::monitor_directory(const char* _path, bool _recursive)
		{
			error_code _errc{};
			auto o = this->monitor_directory(_path, _recursive, _errc);
			if (_errc) { on_unhandled_error(_errc); };
			return o;
		};

		void file_monitor_data::write_change(file_change _change)
		{
			this->changes_.push_back(std::move(_change));
		};
		size_t file_monitor_data::get_changes(file_change*& _outBuffer, size_t _bufferLen)
		{
			auto _count = std::min(this->changes_.size(), _bufferLen);
			for (size_t n = 0; n != _count; ++n)
			{
				_outBuffer[n] = this->changes_.at(n);
			};
			this->changes_.erase(this->changes_.begin(), this->changes_.begin() + _count);
			return _count;
		};

		void file_monitor_data::erase(monitor_key _key)
		{
			auto& _data = this->monitoring_;
			for (auto it = _data.begin(); it != _data.end(); ++it)
			{
				if (it->first == _key)
				{
					_data.erase(it);
					return;
				};
			};
		};

	};

	namespace fs
	{
		void file_monitor::thread_main(ThreadData* _data)
		{
			const auto _updateFrequency = std::chrono::milliseconds(1);

			auto& _stopFlag = _data->stop_flag;
			auto& _initFlag = _data->init_flag;
			auto& _mtx = _data->mtx;
			auto& _monitor = _data->monitor;

			{
				auto lck = std::unique_lock(_mtx);
				_monitor.create();
			};
			_initFlag = true;
			_initFlag.notify_all();

			while (!_stopFlag)
			{
				{
					auto lck = std::unique_lock(_mtx);
					_monitor.poll();
				};
				std::this_thread::sleep_for(_updateFrequency);
			};

			{
				auto lck = std::unique_lock(_mtx);
				_monitor.destroy();
				_stopFlag = true;
			};
		};

		void file_monitor::create()
		{
			this->data_ = std::unique_ptr<ThreadData>(new ThreadData());
			this->thread_ = std::thread(thread_main, this->data_.get());
			this->data_->init_flag.wait(false);
		};
		void file_monitor::destroy()
		{
			this->data_->stop_flag = true;
			this->thread_.join();
		};

		monitor_key file_monitor::monitor_directory(const char* _path, bool _recursive, error_code& _errc)
		{
			auto& _data = *this->data_;
			auto lck = std::unique_lock(_data.mtx);
			return _data.monitor.monitor_directory(_path, _recursive, _errc);
		};
		monitor_key file_monitor::monitor_directory(const char* _path, bool _recursive)
		{
			error_code _errc{};
			auto o = this->monitor_directory(_path, _recursive, _errc);
			if (_errc) { on_unhandled_error(_errc); };
			return o;
		};

		void file_monitor::erase(monitor_key _key)
		{
			auto& _data = *this->data_;
			auto lck = std::unique_lock(_data.mtx);
			_data.monitor.erase(_key);
		};

		size_t file_monitor::get_changes(file_change*& _outBuffer, size_t _bufferLen)
		{
			auto& _data = *this->data_;
			auto lck = std::unique_lock(_data.mtx);
			return _data.monitor.get_changes(_outBuffer, _bufferLen);
		};

	};

};

#endif