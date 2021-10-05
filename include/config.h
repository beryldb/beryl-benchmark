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

#include <string>

const unsigned int DEFAULT_LENGTH       =       2;

const unsigned int DEFAULT_CHECK        =       100;

/* Default session name */

const std::string SESSION_DEFAULT = "emerald";

/* Max. length of a line of text */

const unsigned int INPUT_LIMIT = 65530;

/* Number of processors available. */

const unsigned int CORE_COUNT = 12;

/* Default buffer size. */

const unsigned int BUFFERSIZE = 65536;

/* The branch version that is shown to unprivileged users. */

const std::string BRANCH = "Benchmark-1";

/* The full version that is shown to privileged users. */

const std::string VERSION = "Benchmark-1.0.0";

/* Determines whether this version of BerylDB is older than the requested version. */

#define VERSION_BEFORE(MAJOR, MINOR) (((1 << 8) | 0) < ((MAJOR << 8) | (MINOR)))

/* Determines whether this version of BerylDB is equal to or newer than the requested version. */

#define VERSION_SINCE(MAJOR, MINOR) (((1 << 8) | 0) >= ((MAJOR << 8) | (MINOR)))


/* Whether the clock_gettime() function was available at compile time. */

#define CLOCK_GETTIME_OK

