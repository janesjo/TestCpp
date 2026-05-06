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

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
#ifdef __APPLE__
  addr.sin_len = sizeof(addr);
#endif
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

  return sock;
}

unsigned short getSocketPort(SocketHandle sock) {
  sockaddr_in addr{};
#ifdef __APPLE__
  addr.sin_len = sizeof(addr);
#endif

#ifdef _WIN32
  int addrSize = sizeof(addr);
  if (getsockname(sock, reinterpret_cast<sockaddr*>(&addr), &addrSize) == SOCKET_ERROR) {
    return 0;
  }
#else
  socklen_t addrSize = sizeof(addr);
  if (getsockname(sock, reinterpret_cast<sockaddr*>(&addr), &addrSize) < 0) {
    return 0;
  }
#endif

  return ntohs(addr.sin_port);
}

bool sendUdpMessage(SocketHandle sock, const std::string& address, unsigned short port, std::string_view message) {
  sockaddr_in destination{};
  destination.sin_family = AF_INET;
#ifdef __APPLE__
  destination.sin_len = sizeof(destination);
#endif
  destination.sin_port = htons(port);

  if (inet_pton(AF_INET, address.c_str(), &destination.sin_addr) != 1) {
    return false;
  }

#ifdef _WIN32
  const int sent = sendto(sock, message.data(), static_cast<int>(message.size()), 0,
                          reinterpret_cast<const sockaddr*>(&destination), sizeof(destination));
  return sent != SOCKET_ERROR && sent == static_cast<int>(message.size());
#else
  const ssize_t sent = sendto(sock, message.data(), message.size(), 0,
                              reinterpret_cast<const sockaddr*>(&destination), sizeof(destination));
  return sent >= 0 && static_cast<std::size_t>(sent) == message.size();
#endif
}

bool receiveUdpMessage(SocketHandle sock, std::string& message, std::size_t maxSize) {
  message.clear();

  if (maxSize == 0) {
    return true;
  }

  std::string buffer(maxSize, '\0');
  sockaddr_in source{};
#ifdef __APPLE__
  source.sin_len = sizeof(source);
#endif

#ifdef _WIN32
  int sourceSize = sizeof(source);
  const int received = recvfrom(sock, buffer.data(), static_cast<int>(buffer.size()), 0,
                                reinterpret_cast<sockaddr*>(&source), &sourceSize);
  if (received == SOCKET_ERROR) {
    return false;
  }
#else
  socklen_t sourceSize = sizeof(source);
  const ssize_t received = recvfrom(sock, buffer.data(), buffer.size(), 0, //MSG_DONTWAIT,
                                    reinterpret_cast<sockaddr*>(&source), &sourceSize);
  if (received < 0) {
    return false;
  }
#endif

  message.assign(buffer.data(), static_cast<std::size_t>(received));
  return true;
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
