#pragma once
#ifndef LBX_FS_FILE_HPP
#define LBX_FS_FILE_HPP

#include <lbx/fs/error.hpp>
#include <lbx/fs/detail/os.hpp>

namespace lbx
{
	namespace fs
	{
#if LBX_WINDOWS_V
		using native_file_handle = void*;
#elif LBX_UNIX_V
		using native_file_handle = int;
#endif

		enum class openmode : unsigned
		{
			read = 1,
			r = read,

			write,
			w = write,

			readwrite,
			rw = readwrite,
		};
		enum class openflag : unsigned
		{
			truncate = 0x1,
			trunc = truncate,

			binary = 0x2,
			bin = binary,

			append = 0x4,
			app = append,
		};

		constexpr openflag operator~(openflag rhs)
		{
			return openflag(~(unsigned)rhs);
		};
		constexpr openflag operator|(openflag lhs, openflag rhs)
		{
			return openflag((unsigned)lhs | (unsigned)rhs);
		};
		constexpr openflag operator&(openflag lhs, openflag rhs)
		{
			return openflag((unsigned)lhs & (unsigned)rhs);
		};
		constexpr openflag operator^(openflag lhs, openflag rhs)
		{
			return openflag((unsigned)lhs ^ (unsigned)rhs);
		};
		constexpr openflag& operator|=(openflag& lhs, openflag rhs)
		{
			lhs = lhs | rhs;
			return lhs;
		};
		constexpr openflag& operator&=(openflag& lhs, openflag rhs)
		{
			lhs = lhs & rhs;
			return lhs;
		};
		constexpr openflag& operator^=(openflag& lhs, openflag rhs)
		{
			lhs = lhs ^ rhs;
			return lhs;
		};

		native_file_handle open_native_file(const char* _path, openmode _mode, openflag _flags, error_code& _errc);
		native_file_handle open_native_file(const char* _path, openmode _mode, openflag _flags);

		native_file_handle open_native_file(const char* _path, openmode _mode, error_code& _errc);
		native_file_handle open_native_file(const char* _path, openmode _mode);

		void close_native_file(native_file_handle _file, error_code& _errc);
		void close_native_file(native_file_handle _file);

		struct file
		{
		public:

			native_file_handle get_native_handle() const { return this->file_; };
			native_file_handle release() noexcept
			{
				auto o = this->get_native_handle();
				this->file_ = 0;
				return o;
			};

			bool is_open() const noexcept { return this->get_native_handle() != 0; };
			explicit operator bool() const noexcept { return this->is_open(); };

			void close(error_code& _errc) noexcept
			{
				if (this->is_open())
				{
					close_native_file(this->get_native_handle(), _errc);
					this->release();
				};
			};
			void close()
			{
				if (this->is_open())
				{
					close_native_file(this->get_native_handle());
					this->release();
				};
			};


			file() noexcept :
				file_(0)
			{};
			explicit file(native_file_handle _file) noexcept :
				file_(_file)
			{};
			
			file(const file&) = delete;
			file& operator=(const file&) = delete;

			file(file&& other) noexcept :
				file_(other.release())
			{};
			file& operator=(file&& other) noexcept
			{
				// TODO : Maybe handle reset error?
				error_code _errc{};
				this->close(_errc);
				this->file_ = other.release();
				return *this;
			};

			~file()
			{
				this->close();
			};

		private:
			native_file_handle file_;
		};

		file open(const char* _path, openmode _mode, openflag _flags, error_code& _errc);
		file open(const char* _path, openmode _mode, openflag _flags);
		file open(const char* _path, openmode _mode, error_code& _errc);
		file open(const char* _path, openmode _mode);

		void close(file& _file, error_code& _errc);
		void close(file& _file);

	};
};

#endif 