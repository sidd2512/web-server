#include "proxy_server.hpp"
#include "load_balancer.hpp"
#include "web_server.hpp"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

std::string read_html_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Properly format the HTTP response
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: " + std::to_string(content.length()) + "\r\n"
           "\r\n" + content;
}

std::string handle_request(const std::string& request) {
    // Simple router
    if (request.find("GET / ") != std::string::npos || 
        request.find("GET /index.html ") != std::string::npos) {
        return read_html_file("index.html");
    }
    
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: 18\r\n"
           "\r\n"
           "Hello from server";
}

int main() {
    // Start backend servers (ports 8081-8083)
    std::vector<std::unique_ptr<WebServer>> backends;
    for (int i = 1; i <= 3; i++) {
        auto backend = std::make_unique<WebServer>(8080 + i, handle_request);
        backend->start_in_thread();
        backends.push_back(std::move(backend));
        std::cout << "Started backend on port " << 8080 + i << std::endl;
    }

    // Create load balancer
    LoadBalancer lb;
    lb.add_backend("127.0.0.1", 8081);
    lb.add_backend("127.0.0.1", 8082);
    lb.add_backend("127.0.0.1", 8083);

    // Create and start proxy (port 8080)
    ProxyServer proxy(8080, [&lb](const std::string& req) { 
        return lb.forward_request(req); 
    }, 5000); // 5000-item cache

    std::cout << "Starting proxy server on port 8080" << std::endl;
    proxy.start();

    return 0;
}