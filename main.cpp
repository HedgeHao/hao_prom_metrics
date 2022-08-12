#include <iostream>
#include <thread>
#include <chrono>

#include "collector.hpp"

std::string getNextArg(int argc, char **argv, int target)
{
    return target >= argc ? "" : std::string(argv[target]);
}

void parseArgument(int argc, char **argv, Config *config)
{
    int index = 1;
    bool printHelp = false;
    for (; index < argc; index++)
    {
        std::string arg = argv[index];
        if (arg == "-h" || arg == "--host")
        {
            config->host = getNextArg(argc, argv, ++index);
        }
        else if (arg == "-p" || arg == "--port")
        {
            config->port = std::atoi(getNextArg(argc, argv, ++index).c_str());
        }
        else if (arg == "-j" || arg == "--job")
        {
            config->job = getNextArg(argc, argv, ++index);
        }
        else if (arg == "-n" || arg == "--instance")
        {
            config->instance = getNextArg(argc, argv, ++index);
        }
        else if (arg == "-i" || arg == "--interval")
        {
            config->interval = std::atoi(getNextArg(argc, argv, ++index).c_str());
        }
        else if (arg == "-l" || arg == "--log-level")
        {
            config->logLevel = std::atoi(getNextArg(argc, argv, ++index).c_str());
        }
        else
        {
            printHelp = true;
        }
    }

    if (printHelp)
    {
        printf("[Help]\n");
    }

    config->toString();
}

int main(int argc, char **argv)
{
    Config *config = new Config();

    parseArgument(argc, argv, config);

    while (true)
    {
        Collector *collector = new Collector(config);
        collector->reset();
        collector->collectLiveness();
        collector->push();

        std::this_thread::sleep_for(std::chrono::milliseconds(config->interval));
    }

    return 0;
}