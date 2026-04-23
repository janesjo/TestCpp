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

SocketHandle openUdpSocket(unsigned short bindPort = 0);
void closeSocket(SocketHandle s);

