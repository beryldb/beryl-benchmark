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

#include <iostream>

#include "benchmark.h"
#include "handlers.h"
#include "nodes.h"
#include "server.h"
#include "methods.h"

std::string Handlers::rval = "";

std::vector<std::string> explode(std::string const & s, char delim)
{
      std::vector<std::string> result;
      std::istringstream iss(s);

      for (std::string token; std::getline(iss, token, delim); )
      {
          result.push_back(std::move(token));
      }

      return result;
}

Handlers::Handlers()
{

}

void Handlers::Run()
{
        Kernel->Link.started = std::chrono::steady_clock::now();

        Methods::FlushAll();
 
        for (int i = 0; i < Kernel->Config->times; i++)
        {
               if  (!Kernel->Config->usercmd.tests.empty())
               { 
                    Handlers::RunDivided();
               }
               else
               {
                    Handlers::RunDefault();
               }
        }

        Server::Write("QUIT");
}

void Handlers::RunDivided()
{
        engine::comma_node_stream CMD(Kernel->Config->usercmd.tests);
        std::string server;

        unsigned int counter = 0;

        while (CMD.items_extract(server))
        {
              if (server == "incr")
              {
                    Handlers::DoIncr();
              }
              
              if (server == "set")
              {
                    Handlers::DoSet();
              }
              
              if (server == "ldel")
              {
                    Handlers::DoLDel();
              }
              
              if (server == "hdel")
              {
                    Handlers::DoHDel();
              }
              
              if (server == "vpush")
              {
                    Handlers::DoVPush();
              }
              
              if (server == "get")
              {
                    Handlers::DoGet();
              }
              
              if (server == "decr")
              {
                    Handlers::DoDecr();
              }
              
              counter++;
        }
}

void Handlers::Rand()
{
    std::string tmp_s;
    
    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";
    
    tmp_s.reserve(Kernel->Config->length);

    for (int i = 0; i < Kernel->Config->length; ++i) 
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    Handlers::rval = tmp_s;
}

void Handlers::DoIncr()
{
        Handlers::Rand();
        
        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::Incr(Handlers::rval);
        }
}

void Handlers::DoGet()
{
        Handlers::Rand();

        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::Get(Handlers::rval);
        }
}

void Handlers::DoSet()
{
        Handlers::Rand();

        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::Set(Handlers::rval, Handlers::rval);
        }
}

void Handlers::DoVPush()
{
        Handlers::Rand();

        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::VPush(Handlers::rval, Handlers::rval);
        }
}

void Handlers::DoHSet()
{
        Handlers::Rand();
        
        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::HSet(Handlers::rval, Handlers::rval, Handlers::rval);
        }
}

void Handlers::DoLDel()
{
        Handlers::Rand();

        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::LDel(Handlers::rval, Handlers::rval);
        }
}

void Handlers::DoDecr()
{
        Handlers::Rand();

        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::Decr(Handlers::rval);
        }
}

void Handlers::DoHDel()
{
        Handlers::Rand();

        for (int i = 0; i < Kernel->Config->runs; i++)
        {
                Methods::HDel(Handlers::rval, Handlers::rval);
        }
}

void Handlers::RunDefault()
{
        Handlers::DoIncr();
        Handlers::DoGet();
        Handlers::DoSet();
        Handlers::DoVPush();
        Handlers::DoHSet();
        Handlers::DoLDel();
        Handlers::DoDecr();
        Handlers::DoHDel();
}

void Handlers::OnError(std::vector<std::string>& cmd, std::string& original)
{
    if (cmd.empty())
    {
        return;
    }
    
    bprint(INFO, Daemon::Format("Disconnected: %s", original.c_str()));
    
    printf("\x1b[0m\r");
    
    exit(0);
}


void Handlers::OnQuit(std::vector<std::string>& cmd)
{
        std::vector<std::string> vec = explode(cmd[0], '!');
        std::string user = vec[0];
        std::string output = "QUIT> " + user;
        Daemon::csprint(DTYPE_R, "%s", output.c_str());
        
}

void Handlers::OnMyUse(std::vector<std::string>& cmd)
{
     const std::string select = cmd[3];
     Kernel->select = select;
     
     /* Only display select if different than 1 */
     
     if (select != "1")
     {
             Kernel->display_select = true;
     }
     else
     {
             Kernel->display_select = false;
     }
}

void Handlers::OnYourFlags(std::vector<std::string>& cmd)
{
     std::string mine = cmd[4].erase(0, 1);
     mine.erase(std::remove(mine.begin(), mine.end(), '\n'), mine.end());
    
     if (Kernel->Config->notifyflags)
     {
          Daemon::sprint(DTYPE_R, "Your flags are now: %s", mine.c_str());
    }
}

void Handlers::OnConnected(std::vector<std::string>& cmd)
{          
        Kernel->Connected = true;
        
        Kernel->server_time = convto_num<time_t>(cmd[4]);
        
        std::string version = cmd[3];
        Kernel->server_version = version.erase(0, 1);
        
        if (!Kernel->Config->select.empty() && Kernel->Config->select != "1")
        {
                /* Nothing to do. */
        }
        else
        {
             Kernel->select = "1";
             Kernel->display_select = false;
        }
        
        std::string server = cmd[0];
        Kernel->realserver = server;
        Kernel->myself = cmd[2];
        
        if (!Kernel->Config->customdisplay.empty())
        {
            Kernel->displayserver = Kernel->Config->customdisplay;
        }
        else
        {
            if (Kernel->Config->changeserv)
            {
                 Kernel->displayserver = server + ":" + Kernel->Config->port;
            }
            else
            {
                 Kernel->displayserver = Kernel->Config->host;
            }
        }

        std::cout << Daemon::Format("%-30s | %-10s | %-10s", "Benchmark", "Operations", "Time elapsed (s)") << std::endl;
        std::cout << Daemon::Format("%-30s | %-10s | %-10s", Daemon::Repeat("―", 30).c_str(), Daemon::Repeat("―", 10).c_str(), Daemon::Repeat("―", 10).c_str()) << std::endl;
}

void Handlers::MyInfo(std::vector<std::string>& cmd)
{
    Kernel->myself = cmd[2];
}

void Handlers::OnWrongPass()
{
    bprint(ERROR, "%s: Incorrect login.", Kernel->Config->host.c_str());    
    printf("\x1b[0m\r");

    /* User has been disconnected at this point, we may exit. */
    
    exit(0);
}

std::string Handlers::OnNotFnd(std::vector<std::string>& cmd)
{
    Daemon::sprint(DTYPE_R, "%s is not a command", cmd[3].c_str());
    return "";   
}

