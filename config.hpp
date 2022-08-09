#include <iostream>

class Config
{
public:
    int interval = 5000;
    int port = 9091;
    std::string host = "localhost";
    std::string job = "default_job";
    std::string instance = "";

    void toString()
    {
        printf("[Arguments] >>>>>>>>>>>>>>>>>>>>>\nHost=%s\nPort=%d\nJob=%s\nInstance=%s\nInterval=%d\n[Arguments] <<<<<<<<<<<<<<<<<<<<\n",
               host.c_str(),
               port,
               job.c_str(),
               instance.c_str(),
               interval);
    }
};