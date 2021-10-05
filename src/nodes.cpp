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
 
#include "benchmark.h"

engine::token_node_stream::token_node_stream(const std::string& msg, size_t start, size_t end)
	: message(msg, start, end)
	, position(0)
{

}

bool engine::token_node_stream::get_middle(std::string& token)
{
	if (position >= message.length())
	{
		token.clear();
		return false;
	}
	
	size_t separator = message.find(' ', position);
	
	if (separator == std::string::npos)
	{
		token.assign(message, position, std::string::npos);
		position = message.length();
		return true;
	}

	token.assign(message, position, separator - position);
	position = message.find_first_not_of(' ', separator);
	return true;
}

bool engine::token_node_stream::get_trailing(std::string& token)
{
	if (position >= message.length())
	{
		token.clear();
		return false;
	}
	
	if (message[position] == ':')
	{
		token.assign(message, position + 1, std::string::npos);
		position = message.length();
		return true;
	}
	
	return get_middle(token);
}

engine::node_stream::node_stream(const std::string& source, char separator, bool allowempty)
	: tokens(source), sep(separator), pos(0), allow_empty(allowempty)
{

}

bool engine::node_stream::items_extract(std::string &token)
{
	if (this->stream_finish())
	{
		token.clear();
		return false;
	}

	if (!this->allow_empty)
	{
		this->pos = this->tokens.find_first_not_of(this->sep, this->pos);

		if (this->pos == std::string::npos)
		{
			this->pos = this->tokens.length() + 1;
			token.clear();
			return false;
		}
	}

	size_t p = this->tokens.find(this->sep, this->pos);
	
	if (p == std::string::npos)
	{
		p = this->tokens.length();
	}

	token.assign(tokens, this->pos, p - this->pos);
	this->pos = p + 1;

	return true;
}

const std::string engine::node_stream::get_remaining()
{
	return !this->stream_finish() ? this->tokens.substr(this->pos) : "";
}

bool engine::node_stream::stream_finish()
{
	return this->pos > this->tokens.length();
}

bool engine::node_stream::contains(const std::string& value)
{
	std::string token;

	while (items_extract(token))
	{
		if (value == token)
		{
			return true;
		}
	}

	return false;
}

