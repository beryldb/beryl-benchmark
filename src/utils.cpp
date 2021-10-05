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
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <iomanip>

#include "benchmark.h"
#include "converter.h"

static void ShowHelp()
{
        std::cout << std::endl;

        printf("Usage: ./beryl-bench <arguments>\n\n");

        printf ("%4s %-28s %29s\n", "", "--login <username>,", "Login with a different username.");
        printf ("%4s %-28s %29s\n", "", "--host <host>,", "Connect to provided host.");
        printf ("%4s %-28s %29s\n", "", "--password <password>,", "Use a different password than default.");
        printf ("%4s %-28s %29s\n", "", "--port <int>,", "Use a differnt port than 6378");
        printf ("%4s %-28s %29s\n", "", "--times <int>,", "Repeat test X times.");
        printf ("%4s %-28s %29s\n", "", "--use <1, 100>,", "Use a different select than 1.");
        printf ("%4s %-28s %29s\n", "", "--runs <int>,", "Cycles to run  tests on.");
        printf ("%4s %-28s %29s\n", "", "--length <int>,", "Length of keys to use");
        printf ("%4s %-28s %29s\n", "", "--tests <tests>,", "What tests to run (separated by comma): incr,get,set,lpush,ldel,hset,hdel,vpush");
        printf ("%4s %-28s %19s\n", "", "--version,", "Display version and exit.");
        printf("\n");
        exit(0);
}

void Emerald::CommandLine()
{
        int do_version = 0, do_debug = 0;
        
        struct option longopts[] =
        {
                        { "version",  no_argument,       &do_version,    1   },
                        { "debug",    no_argument,       &do_debug,      1   },
                        { "port",     required_argument, NULL,   	 'p' },
                        { "times",     required_argument, NULL,          'i' },
                        { "host",     required_argument, NULL,   	 'h' },
                        { "pass",     required_argument, NULL,   	 'd' },
                        { "login",    required_argument, NULL,           'l' },
                        { "use",      required_argument, NULL,           'u' },
                        { "runs",     required_argument, NULL,           'r' },
                        { "length",   required_argument, NULL,           'b' },
                        { "tests",    required_argument, NULL,		 't' },
                        { 0, 	      0,		 0,		 0   }
        };
        
        char** argv = this->Config->usercmd.argv;
        int value;

        while ((value = getopt_long(this->Config->usercmd.argc, argv, ":p:h:t:d:l:u:j:i:r:b:", longopts, NULL)) != -1)
        {
                      switch (value)
                      {
                           case 0:
                       
                                  break;
                                  
                           case 'h':
                           
                                  Kernel->Config->usercmd.host = optarg;
                                  break;
                              
                           case 'p':
                              
                                  Kernel->Config->usercmd.port = optarg;
                                  break;
                                  
                           case 'd':
                                  
                                  Kernel->Config->usercmd.pass = optarg;
                                  break;
                                
                           case 'i':

                                  Kernel->Config->usercmd.times = optarg;
                                  break;
                                
                          case 'l':
                                
                                  Kernel->Config->usercmd.login = optarg;
                                  break;
                                 
                          case 't':

                                  Kernel->Config->usercmd.tests = optarg;
                                  break;
                                
                          case 'r':

                                  Kernel->Config->usercmd.runs = optarg;
                                  break;
                                  
                          case 'b':

                                  Kernel->Config->usercmd.length = optarg;
                                  break;
                                  
                          case 'u':
                                         
                                  Kernel->Config->select = optarg;
                                  break;
                                
                           default:
                                
                                 ShowHelp();
                      }       
        }
        
        if (do_version)
        {
              std::cout << VERSION << std::endl;
              exit(0);
        }
        
        this->Config->usercmd.forcedebug = !!do_debug;
}

void Emerald::Refresh()
{

#if defined HAS_CLOCK_GETTIME

        clock_gettime(CLOCK_REALTIME, &TIME);

#else

        gettimeofday(&TIME, NULL);

#endif
}

