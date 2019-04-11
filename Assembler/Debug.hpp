#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifndef NDEBUG
#include "fmt/printf.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "fmt/ostream.h"
#pragma GCC diagnostic pop

#define DEBUG_HELPER(fmtstr, ...) fmt::print(fmtstr "\n", __VA_ARGS__)
#define DEBUG(...) DEBUG_HELPER(__VA_ARGS__)

#else
#define DEBUG(...)
#endif

#endif