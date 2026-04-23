#include "network.h"

SocketHandle openUdpSocket(unsigned short bindPort) {
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
void closeSocket(SocketHandle s) {
#ifdef _WIN32
  if (s != INVALID_SOCKET) closesocket(s);
  WSACleanup();
#else
  if (s >= 0) ::close(s);
#endif
}