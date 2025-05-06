#include "lru_cache.hpp"
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>    // For read(), write(), close()
#include <thread>      // For std::thread


class ProxyServer {
private:
    int port;
    int server_fd;
    std::function<std::string(const std::string&)> request_handler;
    ThreadSafeLRUCache<std::string, std::string> cache;

    void handle_client(int client_socket) {
        char buffer[4096] = {0};
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
        
        if (bytes_read <= 0) {
            close(client_socket);
            return; // No data received or error occurred
        }

        std::string request(buffer, bytes_read);
        std::string response;

        if (auto cached = cache.get(request)) {
            response = *cached;
        } else {
            response = request_handler(request);
            cache.put(request, response);
        }

        // Ensure the response is properly formatted as an HTTP response
        if (response.find("HTTP/1.1") == std::string::npos) {
            response = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n" // Set Content-Type to text/html
                       "Content-Length: " + std::to_string(response.size()) + "\r\n"
                       "\r\n" + response;
        }

        ssize_t bytes_written = write(client_socket, response.c_str(), response.size());
        if (bytes_written < 0) {
            // Handle write error (optional logging or debugging)
        }

        close(client_socket);
    }

public:
    ProxyServer(int p, std::function<std::string(const std::string&)> handler, size_t cache_size = 1000)
        : port(p), request_handler(handler), cache(cache_size) {}

    void start() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        bind(server_fd, (struct sockaddr*)&address, sizeof(address));
        listen(server_fd, 100);

        while (true) {
            int client_socket = accept(server_fd, nullptr, nullptr);
            std::thread([this, client_socket]() {
                handle_client(client_socket);
            }).detach();
        }
    }
};