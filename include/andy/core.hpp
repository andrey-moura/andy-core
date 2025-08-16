#pragma once

/* andy includes */
#if __has_include(<andy/format.hpp>)
#   include <andy/format.hpp>
#endif
#if __has_include(<andy/string.hpp>)
#   define __UVA_HAS_STRING__
#endif
#ifdef __UVA_HAS_STRING__
#   include <andy/string.hpp>
#endif
#include <andy.hpp>
#include <andy/color.hpp>
#include <andy/size.hpp>
#include <andy/point.hpp>
#include <andy/var.hpp>