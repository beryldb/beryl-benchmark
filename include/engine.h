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

#include <iostream>

enum dtype
{
    DTYPE_R = 1,
    DTYPE_RN = 2,
    DTYPE_NONE = 3
};
    
class Daemon : public safecast<Daemon>
{
    public:
    
        /* Constructor. */
        
        Daemon();
        
        void printb(const int type, const std::string& buff);
        
        void printb(const int type, const char *fmt, ...) BERYL_PRINTF(3, 4);        

        static std::string Format(const char* formatString, ...) BERYL_PRINTF(1, 2);

        static std::string Format(va_list& vaList, const char* formatString) BERYL_PRINTF(2, 0);

        static void CheckRoot();

        static void csprint(const int type, const char *fmt, ...);

        static void csprint(const int type, const std::string& buff);
        
        static void sprint(const int type, const char *fmt, ...);

        static void sprint(const int type, const std::string& buff);

        static void serv_sprint(const int type, const char *fmt, ...);

        static void serv_sprint(const int type, const std::string& buff);
        
        static bool LoginValidator(const std::string& login);        
        
        STR1::function<bool(const std::string&)> ValidLogin;

        static std::string Repeat(const std::string& str, unsigned int times);
        
};

