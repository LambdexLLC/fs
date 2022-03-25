#pragma once
#include <lbx/fs/detail/os.hpp>
#if LBX_WINDOWS_V
#ifndef LBX_FS_WINDOWS_HANDLE_HPP
#define LBX_FS_WINDOWS_HANDLE_HPP

#include "win_lib.hpp"

#include <lbx/fs/detail/assert.hpp>

#include <memory>

namespace lbx
{
	namespace fs
	{
		namespace native
		{
			template <typename TagT>
			struct basic_unique_handle
			{
			public:

				using tag = TagT;
				using pointer = HANDLE;

				pointer get() const { return this->ptr_; };
				bool good() const { return this->get() != 0 && this->get() != INVALID_HANDLE_VALUE; };
				explicit operator bool() const { return this->good(); };
				pointer release()
				{
					auto o = this->get();
					this->ptr_ = 0;
					return o;
				};
				void reset()
				{
					if (this->good())
					{
						const auto e = CloseHandle(this->get());
						_LBX_FS_ASSERT(e);
						this->ptr_ = 0;
					};
				};

				basic_unique_handle() = default;
				explicit basic_unique_handle(pointer _ptr) :
					ptr_(_ptr)
				{};

				basic_unique_handle(const basic_unique_handle&) = delete;
				basic_unique_handle& operator=(const basic_unique_handle&) = delete;

				basic_unique_handle(basic_unique_handle&& other) noexcept :
					ptr_(other.release())
				{};
				basic_unique_handle& operator=(basic_unique_handle&& other) noexcept
				{
					this->reset();
					this->ptr_ = other.release();
					return *this;
				};

				~basic_unique_handle() { this->reset(); };

			private:
				pointer ptr_ = 0;
			};
		};
	};
};


#endif // LBX_FS_WINDOWS_HANDLE_HPP
#endif // LBX_WINDOWS_V