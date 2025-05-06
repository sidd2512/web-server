# 🚀 High-Performance C++ Web Server with Load Balancing and LRU Cache

A multi-threaded web server written in modern C++ featuring:

- Round-robin load balancing
- Thread-safe LRU caching
- Static file serving
- Docker support
- Built from scratch using POSIX sockets and threads

---

## 📦 Project Structure

```
web-server/
├── src/                  # Core source files
│   ├── main.cpp          # Entry point with server setup
│   ├── web_server.hpp    # Backend HTTP server
│   ├── proxy_server.hpp  # Caching proxy & load balancer
│   ├── lru_cache.hpp     # Thread-safe LRU implementation
│   └── load_balancer.hpp # Round-robin dispatcher
│── index.html            # Example homepage(html/css)
├── Makefile              # Build configuration
├── Dockerfile            # Container setup
└── README.md             # This file
```

---

## 🚀 Features

✅ **Multi-threaded Architecture**  
✅ **Intelligent Load Balancing**  
✅ **LRU Caching (5,000+ entries)**  
✅ **Static File Support**  
✅ **26,000+ req/sec throughput**  
✅ **Docker-ready Deployment**

---

## 🔧 Quick Start

### Local Build (Manually)

```bash
git clone https://github.com/sidd2512/web-server.git
cd web-server
make && ./web-server
```

### Run with Docker

Pull the prebuilt Docker image directly from Docker Hub:

```bash
docker pull sid2512/web-server
```

Run the container

```bash
docker run -p 8080:8080 sid2512/web-server
```

### Test Endpoint

Using curl:

```bash
curl http://localhost:8080/
```

Or open in your web browser:

```bash
http://localhost:8080/
```

---

## 📈 Benchmark

Tested with `wrk`:

```bash
wrk -t4 -c100 -d10s http://localhost:8080/
```

| Metric         | Result        |
| -------------- | ------------- |
| Requests/sec   | 26,422        |
| Avg Latency    | ~3.5 ms       |
| Backends       | 3             |
| Cache Size     | 5,000 entries |
| Transfer Rate  | 16.08 MB/s    |
| Cache Hit Rate | 92% (warm)    |

---

## 🛠️ Technology Stack

- C++17 (STL containers, threading)
- POSIX sockets (TCP networking)
- LRU cache (std::unordered_map + list)
- Round-robin load balancing
- Docker for containerization

---

## 📜 License

MIT License © 2025 Siddharth Kumar
