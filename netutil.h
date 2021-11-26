///////////////////////////////////////////////////////////////////////////////
// Helper functions for networking.
// https://github.com/PotatoMaster101/netutil
///////////////////////////////////////////////////////////////////////////////

#ifndef NETUTIL_H
#define NETUTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Converts the given pointer to a sockaddr.
#define TOSADDR(PTR) ((struct sockaddr *)(PTR))
// Converts the given pointer to a sockaddr_in.
#define TOSADDRIN4(PTR) ((struct sockaddr_in *)(PTR))
// Converts the given pointer to a sockaddr_in6.
#define TOSADDRIN6(PTR) ((struct sockaddr_in6 *)(PTR))

// String representations of both the IP and port.
typedef struct addrstr_t {
    char ip[NI_MAXHOST];        // IP string
    char port[NI_MAXSERV];      // port string
} addrstr_t;

// Validates the given IPv4 address.
//
// PARAMS:
// ip - the IPv4 address to validate
//
// RETURN:
// 1 if the IPv4 address is valid, 0 otherwise.
_Bool valid4(const char *ip);

// Validates the given IPv6 address.
//
// PARAMS:
// ip - the IPv6 address to validate
//
// RETURN:
// 1 if the IPv6 address is valid, 0 otherwise.
_Bool valid6(const char *ip);

// Sends all the content from the buffer using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// buf    - the buffer to send
// n      - size of the buffer
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t sendall(int sockfd, const char *buf, size_t n);

// Sends all the content from the file stream using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// fs     - the file stream to send
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t sendfs(int sockfd, FILE *fs);

// Sends a long integer (converted to string) using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// n      - the number to send
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t sendlong(int sockfd, long n);

// Sends a double (converted to string) using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// n      - the number to send
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t senddouble(int sockfd, double n);

// Returns an addrinfo using the given settings.
//
// PARAMS:
// fam   - the address family, use AF_UNSPEC for both IPv4/IPv6
// stype - the socket type
// flags - extra flags
//
// RETURN:
// The addrinfo created using the given settings.
struct addrinfo gethints(int fam, int stype, int flags);

// Returns a general addrinfo for TCP connections. This call is equivalent to
// gethints(fam, SOCK_STREAM, flags).
//
// PARAMS:
// fam   - the address family, use AF_UNSPEC for both IPv4/IPv6
// flags - extra flags
//
// RETURN:
// The addrinfo created using the given setting.
struct addrinfo tcphints(int fam, int flags);

// Returns a general addrinfo for UDP connections. This call is equivalent to
// gethints(fam, SOCK_DGRAM, flags).
//
// PARAMS:
// fam   - the address family, use AF_UNSPEC for both IPv4/IPv6
// flags - extra flags
//
// RETURN:
// The addrinfo created using the given setting.
struct addrinfo udphints(int fam, int flags);

// Creates a socket using the addrinfo and returns the socket file descriptor.
//
// PARAMS:
// addr - the addrinfo to create the socket
//
// RETURN:
// The socket file descriptor, or -1 on error.
int addrsock(struct addrinfo *addr);

// Binds the given socket to the addrinfo.
//
// PARAMS:
// sockfd - the socket to bind
// addr   - the addrinfo to bind to
//
// RETURN:
// 0 on success, -1 on error.
int addrbind(int sockfd, struct addrinfo *addr);

// Connects to the given socket to the addrinfo.
//
// PARAMS:
// sockfd - the socket to connect
// addr   - the addrinfo to connect to
//
// RETURN:
// 0 on success, -1 on error.
int addrconn(int sockfd, struct addrinfo *addr);

// Allows the given socket to reuse addresses.
//
// PARAMS:
// sockfd - the socket to allow address reuse
//
// RETURN:
// 0 on success, -1 on error.
int addrreuse(int sockfd);

// Returns the IP and port strings for the given address.
//
// PARAMS:
// addr - the address to convert to string
// len  - the length of the address
//
// RETURN:
// The string representations of IP and port of the given address.
addrstr_t getaddrstr(struct sockaddr *addr, socklen_t len);

// Returns a hostent with information about the current host.
//
// RETURN:
// The host entry, or NULL on error.
struct hostent *gethostentry();

// Returns a fd_set with the given file descriptors set.
//
// PARAMS:
// fds - the file descriptors to set
// n   - the size of the file descriptors
//
// RETURN:
// A new fd_set with the file descriptors set.
fd_set newfds(const int *fds, size_t n);

// Resets the given fd_set.
//
// PARAMS:
// fdset - the fd_set to reset
// fds   - the file descriptors to set
// n     - size of the file descriptors
void clrfds(fd_set *fdset, const int *fds, size_t n);

#endif
