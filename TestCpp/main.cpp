//
//  main.cpp
//  TestCpp
//
//  Created by Jonathan Janesjo on 2026-04-07.
//

#include <future>
#include <iostream>
#include <thread>
#include <utility>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <cerrno>
  #include <cstring>
#endif

// Opens an IPv4 UDP socket. If bindPort > 0, binds to INADDR_ANY:bindPort.
// Returns a socket handle (int on POSIX, SOCKET on Windows). On error, returns -1 on POSIX or INVALID_SOCKET on Windows.
#ifdef _WIN32
using SocketHandle = SOCKET;
#else
using SocketHandle = int;
#endif

SocketHandle openUdpSocket(unsigned short bindPort = 0) {
#ifdef _WIN32
  // Initialize Winsock once per process in real apps; minimal init here
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
    return INVALID_SOCKET;
  }
#endif

  SocketHandle sock =
#ifdef _WIN32
    ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
    ::socket(AF_INET, SOCK_DGRAM, 0);
#endif

#ifdef _WIN32
  if (sock == INVALID_SOCKET) {
    return INVALID_SOCKET;
  }
#else
  if (sock < 0) {
    return -1;
  }
#endif

  if (bindPort > 0) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(bindPort);

    int enable = 1;
    (void)setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                     reinterpret_cast<const char*>(&enable), sizeof(enable));

#ifdef _WIN32
    if (bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
      closesocket(sock);
      return INVALID_SOCKET;
    }
#else
    if (bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
      ::close(sock);
      return -1;
    }
#endif
  }

  return sock;
}

// Helper to close a socket handle
inline void closeSocket(SocketHandle s) {
#ifdef _WIN32
  if (s != INVALID_SOCKET) closesocket(s);
  WSACleanup();
#else
  if (s >= 0) ::close(s);
#endif
}

void product(std::promise<int>&& intPromise, int a, int b){
  intPromise.set_value(a*b);
}

struct Div{

  void operator() (std::promise<int>&& intPromise, int a, int b) const {
    intPromise.set_value(a/b);
  }

};

int main(){

  int a= 20;
  int b= 10;

  std::cout << std::endl;

  // define the promises
  std::promise<int> prodPromise;
  std::promise<int> divPromise;

  // get the futures
  std::future<int> prodResult= prodPromise.get_future();
  std::future<int> divResult= divPromise.get_future();

  // calculate the result in a separat thread
  std::thread prodThread(product,std::move(prodPromise),a,b);
  Div div;
  std::thread divThread(div,std::move(divPromise),a,b);

  // get the result
  std::cout << "20*10= " << prodResult.get() << std::endl;
  std::cout << "20/10= " << divResult.get() << std::endl;

  prodThread.join();
  
  divThread.join();

  std::cout << std::endl;

}

