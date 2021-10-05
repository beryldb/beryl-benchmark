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
#include "engine.h"

Daemon::Daemon() : ValidLogin(&LoginValidator)
{

}

std::string Daemon::Format(va_list& vaList, const char* formatString)
{
        static std::vector<char> formatBuffer(1024);

        while (true)
        {
                va_list dst;
                va_copy(dst, vaList);

                int vsnret = vsnprintf(&formatBuffer[0], formatBuffer.size(), formatString, dst);
                va_end(dst);

                if (vsnret > 0 && static_cast<unsigned>(vsnret) < formatBuffer.size())
                {
                        break;
                }

                formatBuffer.resize(formatBuffer.size() * 2);
        }

        return std::string(&formatBuffer[0]);
}

std::string Daemon::Format(const char* formatString, ...)
{
        std::string ret;
        SCHEME(ret, formatString, formatString);
        return ret;
}


void Daemon::serv_sprint(const int type, const char *fmt, ...)
{
       std::string buff;
       SCHEME(buff, fmt, fmt);
       Daemon::serv_sprint(type, buff);
}

void Daemon::serv_sprint(const int type, const std::string& buff)
{
    if (!Kernel->display_select)
    {
        std::cout << Daemon::Format("%s> %s", Kernel->displayserver.c_str(), buff.c_str()) << std::endl;
    }
    else
    {
        std::cout << Daemon::Format("%s[%s]> %s", Kernel->displayserver.c_str(), Kernel->select.c_str(), buff.c_str()) << std::endl;
    }
        
    if (type == DTYPE_R)
    {
         printf("\x1b[0m\r");
    }
    else if (type == DTYPE_RN)
    {
        printf("\x1b[0m\r\n");
    }
}

void Daemon::sprint(const int type, const char *fmt, ...)
{
       std::string buff;
       SCHEME(buff, fmt, fmt);
       Daemon::sprint(type, buff);
}

void Daemon::sprint(const int type, const std::string& buff)
{
    if (!Kernel->display_select)
    {
         std::cout << buff << std::endl;
    }
    else
    {
        std::cout << buff << std::endl;
    }
    
    if (type == DTYPE_R)
    {
         printf("\x1b[0m\r");
    }
    else if (type == DTYPE_RN)
    {
        printf("\x1b[0m\r\n");
    }
}

void Daemon::csprint(const int type, const char *fmt, ...)
{
       std::string buff;
       SCHEME(buff, fmt, fmt);
       Daemon::csprint(type, buff);
}

void Daemon::csprint(const int type, const std::string& buff)
{
    std::cout << Daemon::Format("%s", buff.c_str()) << std::endl;
    
    if (type == DTYPE_R)
    {
         printf("\x1b[0m\r");
    }
    else if (type == DTYPE_RN)
    {
        printf("\x1b[0m\r\n");
    }

}

void Daemon::printb(const int type, const char *fmt, ...)
{
       std::string buff;
       SCHEME(buff, fmt, fmt);
       this->printb(type, buff);
}

void Daemon::printb(const int type, const std::string& buff)
{
       switch (type)
       {
            case DONE:
                   
                    std::cout << "[ " << green << "OK" << reset << " ] " << buff << reset << "\r\t" << std::endl;
                    break;

            case CONNECT:
                   
                    std::cout << "[ " << blue << "CONNECT" << reset << " ] " << buff << reset << "\r\t" << std::endl;
                    break;
           
            case NOTIF:
                 
                  std::cout << "[ " << blue << "INFO" << reset << " ] " << buff  << reset << "\r\t" << std::endl;
                  printf("\x1b[0m\r");
                  
                  break;

            case INFO:
                   
                    std::cout << "[ " << blue << "INFO" << reset << " ] " << buff  << reset << "\r\t" << std::endl;
                    break;

            case ERROR:
                   
                    std::cout << "[ " << red << "ERROR" << reset << " ] " << buff << reset << "\r\t" << std::endl;
                    break;
                    

            default:
                   
                    std::cout << buff << std::endl;
       }
}

void Daemon::CheckRoot()
{
                if (getegid() != 0 && geteuid() != 0)
                {
                        return;
                }

                bprint(INFO, "You should never run Emerald as root.");
                exit(0);

}

bool Daemon::LoginValidator(const std::string& login)
{
        /* Basic length validators. */
        
        if (login.empty() || login.length() < 3 || login.length() > 15)
        {
                return false;
        }

        for (std::string::const_iterator i = login.begin(); i != login.end(); ++i)
        {
                if ((*i >= 'A') && (*i <= '}'))
                {
                        continue;
                }

                if ((((*i >= '0') && (*i <= '9')) || (*i == '-')) && (i != login.begin()))
                {
                        continue;
                }

                return false;
        }

        return true;
}

std::string Daemon::Repeat(const std::string& str, unsigned int times)
{       
        std::string result;
        
        for (unsigned int i = 0; i < times; i++)
        {
                result.append(str);     
        }
        
        return result;
}
