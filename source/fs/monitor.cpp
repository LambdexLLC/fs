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

		size_t monitor_directory(file_monitor& _monitor, const char* _path, bool _recursive)
		{
			return _monitor.monitor_directory(_path, _recursive);
		};
		void poll(file_monitor& _monitor)
		{
			
		};
		void wait(file_monitor& _monitor)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		};
	};
};
