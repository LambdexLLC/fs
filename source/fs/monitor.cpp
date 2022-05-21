#include <lbx/fs/monitor.hpp>

#include "native.hpp"



namespace lbx
{
	namespace fs
	{
		file_monitor* new_file_monitor()
		{
			auto f = new file_monitor{};
			f->create();
			return f;
		};
		void delete_file_monitor(file_monitor*& _ptr)
		{
			if (_ptr)
			{
				_ptr->destroy();
				delete _ptr;
				_ptr = nullptr;
			};
		};

		size_t monitor_directory(file_monitor& _monitor, const char* _path, file_change_flag _changes, bool _recursive)
		{
			return _monitor.monitor_directory(_path, _changes, _recursive);
		};
		size_t monitor_directory(file_monitor& _monitor, const char* _path, bool _recursive)
		{
			return monitor_directory(_monitor, _path, file_change_flag::all, _recursive);
		};
		void stop_monitoring(file_monitor& _monitor, size_t _key)
		{
			_monitor.erase(_key);
		};

		// Returns how many changes were written to the buffer.
		size_t get_changes(file_monitor& _monitor, file_change* _outBuffer, size_t _bufferLen)
		{
			return _monitor.get_changes(_outBuffer, _bufferLen);
		};

	};
};
