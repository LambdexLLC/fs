#pragma once
#ifndef LBX_FS_ERROR_HPP
#define LBX_FS_ERROR_HPP

/** @file */

namespace lbx
{
	namespace fs
	{
		struct error_code
		{
		public:
			using errc = int;

			constexpr errc code() const noexcept
			{
				return this->errc_;
			};
			constexpr bool is_error() const noexcept
			{
				return this->code() != 0;
			};
			constexpr explicit operator bool() const noexcept
			{
				return this->is_error();
			};

			constexpr error_code() = default;
			constexpr error_code(errc _errc) noexcept :
				errc_(_errc)
			{};
		private:
			errc errc_ = 0;
		};
	};
};

#endif // LBX_FS_ERROR_HPP