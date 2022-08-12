#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "config.hpp"

uint64_t millis()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

std::string curlCustomErrorMessage(CURLcode code)
{
    switch (code)
    {
    case CURLcode::CURLE_COULDNT_RESOLVE_HOST:
        return "Could not resolve host";
    case CURLcode::CURLE_COULDNT_CONNECT:
    case CURLcode::CURLE_RECV_ERROR:
        return "Could not connect or receive from host";

    default:
        return "CURL ERROR" + code;
    }
}

class Collector
{
private:
    CURL *curl;
    CURLcode res;

public:
    Config *config;
    std::string metrics;

    Collector(Config *c) : config(c)
    {
        curl = curl_easy_init();
    }

    void reset() { metrics.clear(); }

    void collectLiveness()
    {
        std::ostringstream o;
        o << millis();
        metrics += "#HELP liveness check\n# TYPE hao_liveness gauge\nhao_liveness{instance=\"" + config->instance + "\"} " + o.str() + "\n";
    }

    int push()
    {
        if (!curl)
            return -1;

        std::string url = "http://" + config->host + ":" + std::to_string(config->port) + "/metrics/job/" + config->job;
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: text/plain");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, metrics.c_str());
        res = curl_easy_perform(curl);

        if (config->logLevel > 0)
        {
            printf("[INFO] Push Metrics (%d)\n", res);
        }

        if (config->logLevel > 1)
        {
            printf("%s\n", metrics.c_str());

            if (res != CURLcode::CURLE_OK)
            {
                printf("[ERROR] %s\n", curlCustomErrorMessage(res).c_str());
            }
        }

        return 0;
    }

    ~Collector()
    {
        curl_easy_cleanup(curl);
    }
};