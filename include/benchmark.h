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

#include <csignal>
#include <memory>
#include <vector>

#include "config.h"
#include "constants.h"
#include "forwards.h"

extern std::unique_ptr<Emerald> Kernel;

#include "flat_map.h"
#include "adaptables.h"
#include "configprocess.h"
#include "colors.h"
#include "converter.h"
#include "protocols.h"
#include "server.h"
#include "handlers.h"
#include "nodes.h"
#include "engine.h"

EmeraldStart;

class Emerald
{
    friend class Handlers;
    
    private:
      
        /* Current time, as updated in the mainloop. */

        struct timeval TIME;
        
        /* Startup time */

        time_t startup; 
        
        /* Command line parser. */
        
        void CommandLine();

        /* My assigned (by the server) instance id. */
        
        std::string myself;
        
        /* Real name of this server. */

        std::string realserver;
        
   public:

        /* Server version. */
        
        std::string server_version;
        
        /* Server time */
        
        time_t server_time;
        
        unsigned int total;
        
        /* Connected boolean. */
        
        bool Connected;
        
        /* > data to display. */

        std::string displayserver;

        /* Selected database */

        std::string select;

        /* Indicates whether we should display current select. */

        bool display_select;
        
        static sig_atomic_t s_signal;
        
        /* Server connection. */
        
        Server Link;

        /* Handles functions from server's protocol. */
        
        Handlers Handler;
        
        /* Internal functions. */
        
        Daemon Engine;

	static void Signalizers();

        /* Handles signals. */

        void SignalManager(int signal);
	
        /* Refreshes TIME */
        
        void Refresh();

        timeval GetTime()
        {
            return this->TIME;
        }
        
        /*
         * Emerald's main. This function will initialize Emerald
         * and will read the config file.
         * 
         * @parameters:
         *
         *         ?? User commandline arguments.
         * 
         * @return:
         *
         *         ?? 1: Emerald is exiting.
         */
        
        Emerald(int argc, char** argv);

        std::unique_ptr<Configuration> Config;

        /* Returns current time. */
        
        inline time_t Now() 
        { 
                return this->TIME.tv_sec; 
        }

        static void Signalizer(int signal);
        
        /* Returns current time, as expressed in microseconds. */
        
        inline long TimeStamp() 
        { 
                return this->TIME.tv_usec; 
        }

        /* Returns startup time. */

        time_t GetStartup()
        {
                return this->startup;
        }
        
        /* 
         * Exits Emerald.
         * 
         * @parameters:
         *
         *         ?? status: Exiting code.
         *         ?? skip: whether to skip exiting functions, such as history removal.
         *         ?? exitmsg: Exit message. 
         */            
         
        void Exit(int code = 0, bool skip = false, const std::string& exitmsg = "");
};

