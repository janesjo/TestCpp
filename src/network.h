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

#include <cstddef>
#include <string>
#include <string_view>

// Opens an IPv4 UDP socket. If bindPort > 0, binds to INADDR_ANY:bindPort.
// Returns a socket handle (int on POSIX, SOCKET on Windows). On error, returns -1 on POSIX or INVALID_SOCKET on Windows.
#ifdef _WIN32
using SocketHandle = SOCKET;
#else
using SocketHandle = int;
#endif

// Size of the buffer used for receiving UDP messages, 65507 bytes payload! 65535 - 8 byte UDP header - 20 byte IP header
// constexpr std::size_t MAX_UDP_MESSAGE_SIZE = 65507;

SocketHandle openUdpSocket(unsigned short bindPort = 0);
unsigned short getSocketPort(SocketHandle sock);
bool sendUdpMessage(SocketHandle sock, const std::string& address, unsigned short port, std::string_view message);
bool receiveUdpMessage(SocketHandle sock, std::string& message, std::size_t maxSize = 1024);
void closeSocket(SocketHandle s);
