#include <iostream>
#include <curl/curl.h>
#include "config.hpp"

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
        metrics += "# TYPE hao_liveness gauge\nhao_liveness{instance=\"" + config->instance + "\"} 1\n";
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

        return 0;
    }

    ~Collector()
    {
        curl_easy_cleanup(curl);
    }
};