#pragma once
#ifndef LBX_FS_DETAIL_OS_HPP
#define LBX_FS_DETAIL_OS_HPP

/** @file */


#define LBX_WINDOWS

#ifdef LBX_WINDOWS
#define LBX_WINDOWS_V true
#else
#define LBX_WINDOWS_V false
#endif

#ifdef LBX_UNIX
#define LBX_UNIX_V true
#else
#define LBX_UNIX_V false
#endif


#endif // LBX_FS_DETAIL_OS_HPP