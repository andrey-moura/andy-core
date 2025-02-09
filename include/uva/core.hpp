#pragma once

/* uva includes */
#if __has_include(<uva/format.hpp>)
#   include <uva/format.hpp>
#endif
#if __has_include(<uva/string.hpp>)
#   define __UVA_HAS_STRING__
#endif
#ifdef __UVA_HAS_STRING__
#   include <uva/string.hpp>
#endif
#include <uva.hpp>
#include <uva/color.hpp>
#include <uva/size.hpp>
#include <uva/point.hpp>
#include <uva/var.hpp>