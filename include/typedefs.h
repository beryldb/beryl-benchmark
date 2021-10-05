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

#define MSG_LIMIT 655633
#define CHA_MAX 200
#define INS_MAX 200
#define HIS_MAX 50

struct Param 
{
	char  *prefix;
	char  *suffix;
	char  *message;
	char  *nickname;
	char  *command;
	char  *channel;
	char  *params;
	size_t offset;
	size_t maxcols;
	int nicklen;
};

struct State 
{
	char  *prompt;     /* Prompt to display. */
	char  *buf;        /* Edited line buffer. */
	size_t buflen;     /* Edited line buffer size. */
	size_t plenb;       /* Prompt length. */
	size_t plenu8;       /* Prompt length. */
	size_t posb;        /* Current cursor position. */
	size_t reserverd_position;        /* Current cursor position. */
	size_t oldposb;     /* Previous refresh cursor position. */
	size_t oldreserverd_position;     /* Previous refresh cursor position. */
	size_t lenb;        /* Current edited line length. */
	size_t lenu8;        /* Current edited line length. */
	size_t cols;       /* Number of columns in terminal. */
	int history_id; /* Current line in the edit history */
};

struct abuf 
{
	char *b;
	int len;
};

