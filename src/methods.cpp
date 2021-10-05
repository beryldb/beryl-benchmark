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
#include "methods.h"
#include "server.h"

void Methods::FlushAll()
{
        Server::Write("flushall\r\n");
}

void Methods::Incr(const std::string& key)
{
        Server::Write("incr %s\r\n", key.c_str());
}

void Methods::Decr(const std::string& key)
{
        Server::Write("decr %s\r\n", key.c_str());
}

void Methods::LDel(const std::string& key, const std::string& value)
{
        Server::Write("ldel %s \"%s\"\r\n", key.c_str(), value.c_str());
}

void Methods::VPush(const std::string& key, const std::string& hesh)
{
        Server::Write("vpush %s \"%s\"\r\n", key.c_str(), hesh.c_str());
}

void Methods::HDel(const std::string& key, const std::string& hesh)
{
        Server::Write("hdel %s \"%s\"\r\n", key.c_str(), hesh.c_str());
}

void Methods::Set(const std::string& key, const std::string& value)
{
        Server::Write("set %s \"%s\"\r\n", key.c_str(), value.c_str());
}

void Methods::HSet(const std::string& key, const std::string& hesh, const std::string& value)
{
        Server::Write("hset %s %s \"%s\"\r\n", key.c_str(), hesh.c_str(), value.c_str());
}

void Methods::HGet(const std::string& key, const std::string& hesh)
{
        Server::Write("hget %s %s\r\n", key.c_str(), hesh.c_str());
}

void Methods::Get(const std::string& key)
{
        Server::Write("get %s\r\n", key.c_str());
}


void Methods::LGet(const std::string& key)
{
        Server::Write("lget %s\r\n", key.c_str());
}

void Methods::Search(const std::string& key)
{
        Server::Write("keys %s\r\n", key.c_str());
}

void Methods::Find(const std::string& key)
{
        Server::Write("find %s\r\n", key.c_str());
}

void Methods::use(const std::string& value)
{
        Server::Write("use %s\r\n", value.c_str());
}

void Methods::LogIn(const std::string& session, const std::string& login, const std::string& pass)
{
        Server::Write("ILOGIN %s %s %s\r\n", session.c_str(), pass.c_str(), login.c_str());

        if (!Kernel->Config->select.empty() && Kernel->Config->select != "1")
        {
             std::cout << std::endl;
             bprint(INFO, "Using select %s", Kernel->Config->select.c_str());
             Methods::use(Kernel->Config->select);
        }
}

void Methods::Command(const std::string& cmd)
{
    Server::Write("%s \r\n", cmd.c_str());
}

