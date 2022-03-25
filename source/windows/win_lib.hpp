#pragma once
#include <lbx/fs/detail/os.hpp>
#if LBX_WINDOWS_V

// Windows includes :(

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fileapi.h>
#include <synchapi.h>


#endif 