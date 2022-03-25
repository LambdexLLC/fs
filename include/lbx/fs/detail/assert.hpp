#pragma once
#ifndef LBX_FS_DETAIL_ASSERT_HPP
#define LBX_FS_DETAIL_ASSERT_HPP

#include <cstdlib>
#define _LBX_FS_ASSERT(cond) if (!(cond)) { ::abort(); }

#endif // LBX_FS_DETAIL_ASSERT_HPP