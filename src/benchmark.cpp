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
#include "engine.h"

std::unique_ptr<Emerald> Kernel = NULL;

EmeraldStart
{
        Kernel = std::make_unique<Emerald>(argc, argv);
        return 1;
}

Emerald::Emerald(int argc, char** argv)  : total(0)
{        
        Kernel = std::unique_ptr<Emerald>(this);
        
        /* Updates time. */
        
        this->Refresh();
        
        /* Startup time. */
        
        this->startup = this->Now();
        
        /* Configuration init. */
        
        this->Config = std::make_unique<Configuration>();

        /* Checks if we are running with a root */
        
        Daemon::CheckRoot();
        
        /* Keep a copy of arguments from main() */

        this->Config->usercmd.argv = argv;

        /* We must pass both, argv and argc */

        this->Config->usercmd.argc = argc;
        
        /* Reads command line. */
        
        this->CommandLine();

        /* We are not connected by default. */
        
        this->Connected = false;
       
        /* Set configuration vars */
        
        this->Config->SetAll();
        
        Emerald::Signalizers();

        /* Inits client connection. */
        
        Link->Initialize();
}


void Emerald::Signalizers()
{
        signal(SIGALRM, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        signal(SIGXFSZ, SIG_IGN);
        signal(SIGINT, Emerald::Signalizer);
        signal(SIGTERM, Emerald::Signalizer);
        signal(SIGKILL, Emerald::Signalizer);
}

void Emerald::SignalManager(int signal)
{
        if (signal == SIGTERM)
        {
                std::cout << std::endl;
                exit(0);
        }
        else if (signal == SIGINT || signal == SIGKILL)
        {
                std::cout << std::endl;
                exit(0);
        }
}

Configuration::Configuration()
{

}

void Configuration::SetAll()
{
        if (Kernel->Config->usercmd.times.empty())
        {
               Kernel->Config->times	=	1;
        }
        else
        {
               Kernel->Config->times = convto_num<int>(Kernel->Config->usercmd.times);
        }

        if (Kernel->Config->usercmd.length.empty())
        {
               Kernel->Config->length = DEFAULT_LENGTH;
        }
        else
        {
               Kernel->Config->length = convto_num<int>(Kernel->Config->usercmd.length);
        }

        if (Kernel->Config->usercmd.runs.empty())
        {
               Kernel->Config->runs = DEFAULT_CHECK;
        }
        else
        {
               Kernel->Config->runs = convto_num<int>(Kernel->Config->usercmd.runs);
        }
        
        if (Kernel->Config->usercmd.host.empty())
        {
              Kernel->Config->host = "localhost";
        }
        else
        {
              Kernel->Config->host = Kernel->Config->usercmd.host;
        }

        if (Kernel->Config->usercmd.port.empty())
        {
              Kernel->Config->port = "6378";
        }
        else
        {
              Kernel->Config->port = Kernel->Config->usercmd.port;
        }

        if (!Kernel->Config->usercmd.pass.empty())
        {
              Kernel->Config->pass = Kernel->Config->usercmd.pass;
        }
        else
        {
              Kernel->Config->pass = "default";
        }

        if (!Kernel->Config->usercmd.login.empty())
        {
              Kernel->Config->login = Kernel->Config->usercmd.login;
        }
        else
        {
              Kernel->Config->login = "root";
        }

        session   = "emerald";
}