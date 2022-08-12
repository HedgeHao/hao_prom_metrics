#include <iostream>

class Config
{
public:
    int interval = 5000;
    int port = 9091;
    int logLevel = 0;
    std::string host = "localhost";
    std::string job = "default_job";
    std::string instance = "";

    void toString()
    {
        printf("[Arguments] >>>>>>>>>>>>>>>>>>>>>\n"
               "Host=%s\n"
               "Port=%d\n"
               "Job=%s\n"
               "Instance=%s\n"
               "Interval=%d\n"
               "LogLevel=%d\n"
               "[Arguments] <<<<<<<<<<<<<<<<<<<<\n",
               host.c_str(),
               port,
               job.c_str(),
               instance.c_str(),
               interval,
               logLevel);
    }
};