#include <vector>
#include <string>
#include <atomic>
#include <curl/curl.h>

class LoadBalancer {
protected:
    std::vector<std::pair<std::string, int>> backends;
    std::atomic<size_t> current_index{0};

    static size_t curl_write(void* contents, size_t size, size_t nmemb, std::string* s) {
        s->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

public:
    void add_backend(const std::string& host, int port) {
        backends.emplace_back(host, port);
    }

    virtual std::string forward_request(const std::string& request) {
        auto& backend = get_next_backend();
        
        CURL* curl = curl_easy_init();
        std::string response;
        
        std::string url = "http://" + backend.first + ":" + std::to_string(backend.second);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // Ensure the response is properly formatted as an HTTP response
        if (response.find("HTTP/1.1") == std::string::npos) {
            response = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: " + std::to_string(response.size()) + "\r\n"
                       "\r\n" + response;
        }

        return response;
    }

protected:
    std::pair<std::string, int>& get_next_backend() {
        size_t index = current_index++ % backends.size();
        return backends[index];
    }
};