#pragma once
#ifndef LBX_FS_MONITOR_HPP
#define LBX_FS_MONITOR_HPP

/** @file */

#include <lbx/fs/error.hpp>

#include <string>
#include <variant>
#include <string_view>

namespace lbx
{
	namespace fs
	{
		struct file_monitor;

		file_monitor* new_file_monitor();
		void delete_file_monitor(file_monitor*& _ptr);



		/**
		 * @brief Bitflags for change types
		*/
		enum class file_change_flag : uint8_t
		{
			add		= 0x01,
			remove	= 0x02,
			rename	= 0x04,
			modify	= 0x08,
			all		= 0x0F
		};

		constexpr file_change_flag operator&(file_change_flag lhs, file_change_flag rhs)
		{
			return file_change_flag((uint8_t)lhs & (uint8_t)rhs);
		};
		constexpr file_change_flag operator|(file_change_flag lhs, file_change_flag rhs)
		{
			return file_change_flag((uint8_t)lhs | (uint8_t)rhs);
		};
		constexpr file_change_flag operator^(file_change_flag lhs, file_change_flag rhs)
		{
			return file_change_flag((uint8_t)lhs ^ (uint8_t)rhs);
		};
		constexpr file_change_flag operator~(file_change_flag rhs)
		{
			return file_change_flag(~(uint8_t)rhs);
		};


		// TODO : Doc
		size_t monitor_directory(file_monitor& _monitor, const char* _path, file_change_flag _changes, bool _recursive = false);

		// TODO : Doc
		// flags are defaulted to "all"
		size_t monitor_directory(file_monitor& _monitor, const char* _path, bool _recursive = false);
		
		// TODO : Doc
		void stop_monitoring(file_monitor& _monitor, size_t _key);

		enum class file_change_type
		{
			add = 1,
			remove,
			rename,
			change,
		};

		struct file_change
		{
		private:
			struct change_base
			{
			public:
				size_t key() const noexcept { return this->key_; };
			protected:
				change_base(size_t _key) :
					key_(_key)
				{};
			private:
				size_t key_;
			};
		public:
			struct add : change_base
			{
			public:
				std::string& path() noexcept { return this->path_; };
				const std::string& path() const noexcept { return this->path_; };
				add(size_t _key, std::string _path) :
					change_base(_key), path_(std::move(_path))
				{};
			private:
				std::string path_;
			};
			struct remove : change_base
			{
			public:
				std::string& path() noexcept { return this->path_; };
				const std::string& path() const noexcept { return this->path_; };				
				remove(size_t _key, std::string _path) :
					change_base(_key), path_(std::move(_path))
				{};
			private:
				std::string path_;
			};
			struct rename : change_base
			{
			public:
				std::string& old_path() noexcept { return this->old_path_; };
				const std::string& old_path() const noexcept { return this->old_path_; };

				std::string& new_path() noexcept { return this->new_path_; };
				const std::string& new_path() const noexcept { return this->new_path_; };

				rename(size_t _key, std::string _oldPath, std::string _newPath) :
					change_base(_key),
					old_path_(std::move(_oldPath)),
					new_path_(std::move(_newPath))
				{};
			private:
				std::string old_path_;
				std::string new_path_;
			};
			struct change : change_base
			{
			public:
				std::string& path() noexcept { return this->path_; };
				const std::string& path() const noexcept { return this->path_; };

				change(size_t _key, std::string _path) :
					change_base(_key), path_(std::move(_path))
				{};
			private:
				std::string path_;
			};
		private:
			using vt = std::variant<std::nullptr_t, add, remove, rename, change>;
		public:
			
			file_change_type type() const noexcept { return file_change_type(this->vt_.index()); };
			
			template <typename T>
			T& get() { return std::get<T>(this->vt_); };
			template <typename T>
			const T& get() const { return std::get<T>(this->vt_); };

			template <file_change_type T>
			auto& get() { return std::get<(size_t)T>(this->vt_); };
			template <file_change_type T>
			const auto& get() const { return std::get<(size_t)T>(this->vt_); };



			template <typename T>
			file_change(T&& _value) :
				vt_(std::forward<T>(_value))
			{};
		
			file_change() :
				vt_(nullptr)
			{};

		private:
			vt vt_;
		};



		// Returns how many changes were written to the buffer.
		size_t get_changes(file_monitor& _monitor, file_change* _outBuffer, size_t _bufferLen);

	};
};


#endif // LBX_FS_MONITOR_HPP