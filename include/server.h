/*
 * BerylDB - A modular database.
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

/*
 * server.cpp is largely based in Michael Czigler's IRC client: kirc.
 *
 * Many credits goes to the kirc team (Copyright 2021 - MIT license),
 * Michael and his great work. 
 *
 * kirc is based on linenoise (BSD-2), so credits goes to the linenoise team as well.
 */

#pragma once

#include <termios.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <deque>
#include <chrono>

#include "typedefs.h"

class Server : public safecast<Server>
{
    private:
    
        /* Current state */
        
        struct pollfd fds[2];
    
        void Flush();

    public:
    
        std::chrono::steady_clock::time_point begin;

        std::chrono::steady_clock::time_point started;
        
        int current;
        
        int total_get;
        
        int total_set;
        
        int total_lpush;
        
        int total_vpush;
        
        int total_hset;
        
        int total_incr;

        int total_ldel;
        
        int total_decr;
        
        int total_hdel;
        
        int commands;
        
        /* Constructor. */
        
        Server();
        
        std::deque<std::string> buffer;
        
        /* Writes history file. */
        
        int HistoryWrite();
        
        /* Resets cache */
        
        void ResetCache();
        
        /* Inits Emerald's loop. */

        int Initialize();
        
        /* Raw socket write to remote server. */
        
        static void Write(char *fmt, ...);

        static void Direct(char *fmt, ...);

        /* 
         * This function is called before the 
         * user sends a new command to the server. 
         */
        
        bool CheckCmd(const std::vector<std::string>& CommandList);
    
        /* Closes socket. */
        
        void Shutdown();        
        
        void PrepareExit();
        
        /* Clears screen */
        
        static void ClearScreen(void);        
        
        /* Returns history counter */
        
        unsigned int CountHistory();
        
        void QuickExit();
        
        /* Resets buffer */
        
        void Reset()
        {
            this->buffer.clear();
        }
        
};