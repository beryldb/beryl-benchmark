/*
 * Benchmark - A benchmark client for BerylDB.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 - Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

#include "nodes.h"

#include <stdarg.h>

#define DONE 1
#define INFO 2
#define ERROR 3
#define WARNING 4
#define NOTIF 5
#define CONNECT 6


#if defined _LIBCPP_VERSION || defined _WIN32 || __cplusplus >= 201103L
# define STR1 std
# include <array>
# include <functional>
# include <unordered_map>
# include <type_traits>
#else
# define STR1 std::tr1
# include <tr1/array>
# include <tr1/functional>
# include <tr1/unordered_map>
# include <tr1/type_traits>
#endif


/* bprint is short for "beryl print", which prints out to terminal. */

#define bprint Kernel->Engine->printb

#if defined __clang__ || defined __GNUC__
# define BERYL_PRINTF(stringpos, firstpos) __attribute__((format(printf, stringpos, firstpos)))
#else
# define BERYL_PRINTF(stringpos, firstpos)
#endif

#define EmeraldStart int main(int argc, char** argv)

template <typename T> struct safecast
{
        T* operator->()
        {
                return static_cast<T*>(this);
        }
};

#define SCHEME(scheme, last, format) \
        do { \
        va_list _vaList; \
        va_start(_vaList, last); \
        scheme.assign(Daemon::Format(_vaList, format)); \
        va_end(_vaList); \
        } while (false);


