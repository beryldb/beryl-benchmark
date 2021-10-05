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

#include <cfloat>
#include <climits>

#include "adaptables.h"

struct UserArguments
{
        std::string host;
    
        std::string login;
        
        std::string port;
    
        std::string pass;
        
        std::string tests;
        
        std::string times;
        
        std::string runs;
        
        std::string length;
        
        bool do_tests;
        
        bool forcedebug;
                
        int argc;

        char** argv;
};

class Configuration
{
    public:
        
        Configuration();
        
        int times;
        
        int length;
        
        int runs;
        
        std::string host;
        
        std::string session;
        
        std::string login;
        
        std::string port;
        
        std::string pass;

        UserArguments usercmd;

        std::stringstream errstr;
        
        std::string select;
        
        bool Ready;
        
        bool changeserv;
        
        bool notifyflags;
        
        bool lognotify;
        
        bool clear;

        std::string customdisplay;
        

        bool RawLog;
        
        void Load();
        
        void SetAll();
        
        void CheckCMD();
};

