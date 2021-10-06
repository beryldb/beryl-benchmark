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

#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "benchmark.h"
#include "protocols.h"
#include "converter.h"
#include "typedefs.h"
#include "server.h"
#include "methods.h"
#include "nodes.h"

static int   conn;  /* connection socket */

void Deliver(const std::string& title, int *counter)
{
	*counter = *counter + 1;
	
        if (*counter == 1)
        {
               Kernel->Link.begin = std::chrono::steady_clock::now();
        }

        if (*counter < Kernel->Config->runs)
        {
		std::cout << Daemon::Format("%-30s | %-10s | %-10s", title.c_str(), convto_string(*counter).c_str(), "*");
                std::flush(std::cout);
        }
        else
        {
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                
                std::chrono::duration<double> tp = std::chrono::duration_cast<std::chrono::microseconds>(end - Kernel->Link.begin);

                std::cout << Daemon::Format("%-30s | %-10s | %-10f", title.c_str(), convto_string(*counter).c_str(), tp.count());
        	std::cout << std::endl;
        	*counter = 0;
	}
}

void Server::Flush()
{
        if (this->buffer.empty())
        {
            return;
        }
        
        std::string line = this->buffer.front();
        bool dirty = false;
        
        if (line.substr(0, 3) == "get")
        {
        	current = 1;
        	dirty = true;
        }
        
        if (!dirty && line.substr(0, 3) == "set")
        {
                current = 6;
                dirty = true;
        }
        
        if (!dirty && line.substr(0, 5) == "lpush")
        {
                current = 2;
                dirty = true;
        }
        
        if (!dirty && line.substr(0, 4) == "ldel")
        {
                current = 7;
                dirty = true;
        }
        
        if (!dirty && line.substr(0, 5) == "vpush")
        {
                current = 3;
                dirty = true;
        }
        
        if (!dirty && line.substr(0, 4) == "hset")
        {
                current = 4;
                dirty = true;
        }

        if (!dirty && line.substr(0, 4) == "hdel")
        {
                current = 9;
                dirty = true;
        }
        
        if (!dirty && line.substr(0, 4) == "incr")
        {
                current = 5;
                dirty = true;
        }
        
        if (!dirty && line.substr(0, 4) == "decr")
        {
                current = 8;
                dirty = true;
        }

        if (line == "QUIT")
        {
	       std::chrono::duration<double> tp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - Kernel->Link.started);
               std::cout << std::endl;

	       std::cout << Daemon::Format("Benchmark finished : %fs",tp.count());

               std::cout << std::endl;
               this->Shutdown();
               exit(0);
        }
        
       this->buffer.pop_front();
		 
       if (write(conn, line.c_str(), strnlen(line.c_str(), MSG_LIMIT)) < 0) 
       {
               perror("write");
               exit(0);
       }
}

void Server::Direct(char *fmt, ...) 
{
        va_list ap;
        char *cmd_str = (char*)malloc(MSG_LIMIT);

        if (!cmd_str) 
        {
                perror("malloc");
                exit(0);
        }

        va_start(ap, fmt);
        vsnprintf(cmd_str, MSG_LIMIT, fmt, ap);
        va_end(ap);

       if (write(conn, cmd_str, strnlen(cmd_str, MSG_LIMIT)) < 0) 
       {
               perror("write");
               exit(0);
       }

       free(cmd_str);
}

void Server::Write(char *fmt, ...) 
{
	va_list ap;
	char *cmd_str = (char*)malloc(MSG_LIMIT);

	if (!cmd_str) 
	{
		perror("malloc");
		exit(0);
	}

	va_start(ap, fmt);
	vsnprintf(cmd_str, MSG_LIMIT, fmt, ap);
	va_end(ap);
	
	Kernel->Link->buffer.push_back(cmd_str);
	
	free(cmd_str);
}

static int InitConnection(void) 
{
	int gai_status;

	struct addrinfo *res, hints = 
	{
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_STREAM
	};

	if ((gai_status = getaddrinfo(Kernel->Config->host.c_str(), Kernel->Config->port.c_str(), &hints, &res)) != 0) 
	{
		Kernel->Connected = false;
		std::string errmsg = "Unable to connect: " + convto_string(gai_strerror(gai_status));
		bprint(ERROR, "%s", errmsg.c_str());
		exit(0);
		return -1;
	}

	struct addrinfo *p;

	struct timeval timeout;      
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
    
	for (p = res; p != NULL; p = p->ai_next) 
	{
		if ((conn = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
		{
			exit(0);
			continue;
		}

		setsockopt(conn, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval));

		if (connect(conn, p->ai_addr, p->ai_addrlen) == -1) 
		{
			close(conn);
			Kernel->Connected = false;
			
			const std::string errmsg = Daemon::Format("Unable to connect to %s:%s", Kernel->Config->host.c_str(), Kernel->Config->port.c_str());
        	        bprint(ERROR, "%s", errmsg.c_str());
			exit(0);
			continue;
		}

		break;
	}

	freeaddrinfo(res);

	if (p == NULL) 
	{
		fputs("Failed to connect\n", stderr);
		return -1;
	}

	int flags = fcntl(conn, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(conn, F_SETFL, flags);

	return 0;
}

static void UnknownMessage(struct Param *p) 
{
	if (!p->message) 
	{
		return;
	}

	switch (Kernel->Link.current)
	{
		case 1: 
			Deliver("GET",   &Kernel->Link.total_get);		
		break;

		case 2:
	                Deliver("LPUSH", &Kernel->Link.total_lpush);
		break;
		
                case 3:
                        Deliver("VPUSH", &Kernel->Link.total_vpush);
                break;
		
                case 4:
                        Deliver("HSET", &Kernel->Link.total_hset);
                break;

                case 5:
                        Deliver("INCR", &Kernel->Link.total_incr);
                break;

                case 6: 
                        Deliver("SET", &Kernel->Link.total_set);           
                break;
                
                case 7: 
                        Deliver("LDEL", &Kernel->Link.total_ldel);
                break;
                
                case 8: 
                        Deliver("DECR", &Kernel->Link.total_decr);
                break;

                case 9:
                        Deliver("HDEL", &Kernel->Link.total_hdel);
                break;
                
		default:
			break;
	}
}

static void CommandParser(char *request) 
{
	const std::string modified = convto_string(request);
	
	if (!strncmp(request, convto_string(BRLD_PING).c_str(), 3)) 
	{	
		Kernel->Link.Write("PONG 1\r\n");
		return;
	}

	if (request[0] != ':' || (strnlen(request, MSG_LIMIT) < 4)) 
	{
		return;
	}

	printf("\r\x1b[0K");

	struct Param p;

	p.prefix =   strtok(request, " ") + 1;
	p.suffix =   strtok(NULL, ":");
	p.message =  strtok(NULL, "\r");
	p.nickname = strtok(p.prefix, "!");
	p.command =  strtok(p.suffix, "#& ");
	p.channel =  strtok(NULL, " \r");
	p.params =   strtok(NULL, ":\r");
	p.nicklen = (p.maxcols / 3 > INS_MAX ? INS_MAX : p.maxcols / 3);
	p.offset = 0;

	std::string cmd = convto_string(strtok(p.suffix, "#& "));
	std::string msg = convto_string(p.message);

	std::string protocol = cmd.substr(0, 3);
	unsigned int brld_protocol = 0;
	
	if (cmd == "PUBLISH")
	{
		brld_protocol = BRLD_PUBLISH;	
	}
	else 
	{
	
		if (!protocol.empty())
		{
				brld_protocol = convto_num<unsigned int>(protocol);
		}
	}

	std::string response;

	engine::space_node_stream CMD(modified);
	std::string server;
        std::vector<std::string> params;

	unsigned int counter = 0;


	while (CMD.items_extract(server))
	{   
		std::string param = server;

		if (counter == 0)
		{
			    param.erase(0, 1);
		}
		else 
		{

		}

		params.push_back(param);

		counter++;
	}
	
	switch (brld_protocol)
	{
			case BRLD_CONNECTED:

				Kernel->Handler.OnConnected(params);
				
			break;
			
			case ERR_CMD_NOFND:
				
				response = Kernel->Handler.OnNotFnd(params);
			break;
			
			case  ERR_WRONG_PASS:
			
				Kernel->Handler.OnWrongPass();
			break;

			case BRLD_MYINFO:
					
				Kernel->Handler.MyInfo(params);

			break;

			case BRLD_ERROR:
			
				Kernel->Handler.OnError(params, msg);
				return;
			break;
			
			case BRLD_QUITTING:
				
				exit(0);
			break;

			case BRLD_NEW_USE:
			
                                Kernel->Handler.OnMyUse(params);
                                return;
				
			break;


			case BRLD_QUIT:

				Kernel->Handler.OnQuit(params);

			break;

			default:
			{
			 	UnknownMessage(&p);
				return;
			}

	}

	if (!response.empty())
	{	
		Daemon::sprint(DTYPE_R, "%s", response.c_str());
	} 
	else 
	{
		return;
	}
	
}

static char buffered_message[MSG_LIMIT + 1];
static size_t message_end = 0;

static int HandleMessage(void) 
{
	for (;;) 
	{
		ssize_t stream_read = read(conn, &buffered_message[message_end], MSG_LIMIT - message_end);
		
		if (stream_read == -1) 
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
			{
				return 0;
			} 
			else 
			{
				exit(0);
				return -2;
			}
		}

		if (stream_read == 0) 
		{
			printf("\x1b[0m\r");

			bprint(INFO, "Server has exited.");
                        printf("\x1b[0m\r");

			exit(0);
			return -1;
		}

		size_t i, old_message_end = message_end;
		message_end += stream_read;

		for (i = old_message_end; i < message_end; ++i) 
		{
			if (i != 0 && buffered_message[i - 1] == '\r' && buffered_message[i] == '\n') 
			{
				char saved_char = buffered_message[i + 1];
				buffered_message[i + 1] = '\0';
				CommandParser(buffered_message);
				buffered_message[i + 1] = saved_char;
				memmove(&buffered_message, &buffered_message[i + 1], message_end - i - 1);
				message_end = message_end - i - 1;
				i = 0;
			}
		}

		if (message_end == MSG_LIMIT) 
		{
			message_end = 0;
		}
	}
}

Server::Server() :  current(0), total_get(0), total_set(0), total_lpush(0), total_vpush(0), total_hset(0), total_incr(0), total_ldel(0), total_decr(0), total_hdel(0), commands(0)
{

}

void Server::Shutdown()
{
	Kernel->Connected = false;
	shutdown(conn, 1);
}

void Server::ResetCache()
{

}

void Server::QuickExit()
{
        Kernel->Link->PrepareExit();
        Kernel->Link->Reset();
}

void Server::PrepareExit()
{

}

int Server::Initialize()
{
	if (InitConnection() != 0) 
	{
		return 1;
	}

	Methods::LogIn(SESSION_DEFAULT, Kernel->Config->login, Kernel->Config->pass);
	
	std::cout << std::endl;
		
	fds[0].fd = STDIN_FILENO;
	fds[1].fd = conn;
	fds[0].events = POLLIN;
	fds[1].events = POLLIN;

	int rc;

	Kernel->Refresh();
	
	Handlers::Run();

	while (true)
	{ 
                Kernel->Refresh();
                this->Flush();

 	        int r = poll(fds, 2, -1);

                if (Kernel->s_signal)
                {
                        Kernel->SignalManager(Kernel->s_signal);
                        Emerald::s_signal = 0;
                }

		if (r != -1)
		{
			if (fds[1].revents & POLLIN) 
			{
				rc = HandleMessage();

				if (rc != 0) 
				{
					if (rc == -2) 
					{
						return 1;
					}

					return 0;
				}

			}
		} 
		else 
		{
			if (errno == EAGAIN) 
			{
				continue;
			}

			exit(0);
			return 1;
		}
		
	}
	
        exit(0);
}

sig_atomic_t Emerald::s_signal = 0;

void Emerald::Signalizer(int signal)
{
        s_signal = signal;
}
